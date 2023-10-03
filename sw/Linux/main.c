#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include<unistd.h>
#include <stdbool.h>
#include <sys/mman.h>
#include "program.c"

#define LW_BASE 0xFF200000  // Address of the GPIO controller in LWHPS2FPGA bridge
#define ENTER_OFFSET 0x00
#define SWITCHES_OFFSET 0x10
#define EDGE_CAPTURE_OFFSET 0x03
#define DISPLAY_OFFSET 0x20

volatile uint32_t* enter_ptr;
volatile uint32_t* switch_ptr;
volatile uint32_t* display_ptr;
int press_count = 0;
uint32_t input = 0;
uint32_t n = 0;
uint32_t d = 0;


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


void display(){
    uint32_t display0_value = num_to_display(get_switch_value());
    uint32_t display1_value = num_to_display((input) & 0xf);
    uint32_t display2_value = num_to_display((input >> 4) & 0xf);
    uint32_t display3_value = num_to_display((input >> 8) & 0xf);
    
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
    press_count++;
    *ec_ptr = 0x00;
}

void conc_new_digit(){
    if(press_count < 8){
        input = input << 4; //Hacer campo al otro valor
        input = input | get_switch_value();
    }
}

void run(){
    printf("Running main program using d as %x and n as %x\n", d, n);

    run_program(d, n);
}

void if_complete_key_set_var(){
    if(press_count == 4){
        n = input;
        input = 0;
        printf("n = %x\n", n);
    }
    else if(press_count == 8){
        d = input;
        input = 0;
        printf("d = %x\n", d);
    }
}

bool is_key_set(){
    return press_count == 8;
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
            conc_new_digit();
            reset_edgecapture();
            if_complete_key_set_var();
        }
        if(is_key_set()){
            run();
            break;
        }
    }

    munmap(virtual_base, 4096);
    close(fd);
    return 0;
}