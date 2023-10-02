#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include<unistd.h>
#include <sys/mman.h>

#define LW_BASE 0xFF200000  // Address of the GPIO controller in LWHPS2FPGA bridge
#define ENTER_OFFSET 0x00
#define SWITCHES_OFFSET 0x10
#define EDGE_CAPTURE_OFFSET 0x03
#define DISPLAY_OFFSET 0x20

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

    volatile uint32_t* enter_ptr = (volatile uint32_t*)((uint8_t*)virtual_base + ENTER_OFFSET);
    volatile uint32_t* switch_ptr = (volatile uint32_t*)((uint8_t*)virtual_base + SWITCHES_OFFSET);
    volatile uint32_t* display_ptr = (volatile uint32_t*)((uint8_t*)virtual_base + DISPLAY_OFFSET);

    while (1) {
        volatile int switch_value = *((volatile uint32_t *)switch_ptr) & 0xf;
        *display_ptr = num_to_display(switch_value);
        
        volatile uint32_t* switch_edgecapture_ptr = switch_ptr + EDGE_CAPTURE_OFFSET;
        uint32_t edgecapture_value = (*(switch_edgecapture_ptr) >> 4) & 1;
        if(edgecapture_value == 1) {
            printf("Entró al if\n");
            *switch_edgecapture_ptr = 0x00;
        }
    }

    munmap(virtual_base, 4096);
    close(fd);
    return 0;
}