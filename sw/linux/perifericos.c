#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include<unistd.h>
#include <stdbool.h>
#include <sys/mman.h>

#define LW_BASE 0xFF200000  // Address of the GPIO controller in LWHPS2FPGA bridge
#define ENTER_OFFSET 0x00
#define SWITCHES_OFFSET 0x10
#define EDGE_CAPTURE_OFFSET 0x03
#define DISPLAY_OFFSET 0x20

volatile uint32_t* enter_ptr;
volatile uint32_t* switch_ptr;
volatile uint32_t* display_ptr;
uint32_t n = 0;


uint32_t num_to_display(int num){
    uint32_t display_value;
    if(num == 0)
        display_value = 0x40;
    else if(num == 1)
        display_value = 0x79;
    else if(num == 2)
        display_value = 0x24;
    else if(num == 3)
        display_value = 0x30;
    else if(num == 4)
        display_value = 0x19;
    else if(num == 5)
        display_value = 0x12;
    else if(num == 6)
        display_value = 0x02;
    else if(num == 7)
        display_value = 0x78;
    else if(num == 8)
        display_value = 0x00;
    else if(num == 9)
        display_value = 0x10;
    else if(num == 0xa)
        display_value = 0x08;
    else if(num == 0xb)
        display_value = 0x03;
    else if(num == 0xc)
        display_value = 0x46;
    else if(num == 0xd)
        display_value = 0x21;
    else if(num == 0xe)
        display_value = 0x06;
    else if(num == 0xf)
        display_value = 0x0e;
    else
        display_value = 0x0;
    return display_value;
}

uint32_t get_switch_value(){
    volatile uint32_t switch_value = *((volatile uint32_t *)switch_ptr) & 0xf;
    return switch_value;
}

void setup_per(void* virtual_base){
    enter_ptr = (volatile uint32_t*)((uint8_t*)virtual_base + ENTER_OFFSET);
    switch_ptr = (volatile uint32_t*)((uint8_t*)virtual_base + SWITCHES_OFFSET);
    display_ptr = (volatile uint32_t*)((uint8_t*)virtual_base + DISPLAY_OFFSET);
}

// void update_final_display(){
//     uint32_t digit = n & 0xf;
//     uint32_t display_value = num_to_display(digit);
//     final_display = ((final_display | 0xffff) | display_value) << 7;
// }

void display(){
    // int j = 7;
    // int bitmask = ~(1 << j) & ((1 << j) - 1);
    // final_display = (final_display & bitmask) | num_to_display(get_switch_value());
    // *display_ptr = final_display;
    uint32_t display0_value = num_to_display(get_switch_value());
    uint32_t display1_value = num_to_display((n) & 0xf);
    uint32_t display2_value = num_to_display((n >> 4) & 0xf);
    uint32_t display3_value = num_to_display((n >> 8) & 0xf);
    
    *display_ptr = display3_value << 21 | display2_value << 14 | display1_value << 7 | display0_value;
}

volatile uint32_t* get_edgecapture_ptr(volatile uint32_t* ptr){
    return ptr + EDGE_CAPTURE_OFFSET;
}

bool is_enter_pressed(){
    volatile uint32_t* ec_ptr = get_edgecapture_ptr(switch_ptr);
    uint32_t edgecapture_value = (*(ec_ptr) >> 4) & 1;
    return edgecapture_value == 1;
}

void reset_edgecapture(){
    volatile uint32_t* ec_ptr = get_edgecapture_ptr(switch_ptr);
    *ec_ptr = 0x00;
}

void set_new_digit(){
    n = n << 4; //Hacer campo al otro valor
    n = n | get_switch_value();
    printf("n = %x\n", n);
}

int main() {
    int fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    void* virtual_base = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, LW_BASE);
    if (virtual_base == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    setup_per(virtual_base);

    while (1) {
        display();
        
        if(is_enter_pressed()) {
            printf("Entró al if\n");
            set_new_digit();
            reset_edgecapture();
        }
    }

    munmap(virtual_base, 4096);
    close(fd);
    return 0;
}