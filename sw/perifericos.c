// #include <stdio.h>
// #include <stdint.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <sys/mman.h>

// #define LWHPS2FPGA_Base 0xFF200000  // Address of the GPIO controller in LWHPS2FPGA bridge
// #define SWITCHES_OFFSET 0x00
// #define DISPLAY0_OFFSET 0X10


// // void seven_segment_decoder(uint32_t num){


// // }


// int main() {
//     int fd = open("/dev/mem", O_RDWR|O_SYNC);
//     if (fd < 0) {
//         perror("open");
//         return 1;
//     }

//     void* virtual_base = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, LWHPS2FPGA_Base);
//     if (virtual_base == MAP_FAILED) {
//         perror("mmap");
//         return 1;
//     }

//     // volatile uint32_t* switches_ptr = (volatile uint32_t*)((uint8_t*)virtual_base + SWITCHES_OFFSET);
//     volatile uint32_t* display_ptr = (volatile uint32_t*)((uint8_t*)virtual_base + DISPLAY0_OFFSET);

//     while (1) {
//         uint32_t num = 0xa;
//         uint32_t display_value;
//             switch(num){
//         case 0xb:
//             display_value = (uint32_t)0b0000011;
//         case 0xc:
//             display_value = (uint32_t)0b1000110;
//         case 0xd:
//             display_value = (uint32_t)0b0100001;
//         case 0xe:
//             display_value = (uint32_t)0b0000110;
//         case 0xf:
//             display_value = (uint32_t)0b0001110;
//         default:
//             display_value = (uint32_t)0b1000000;
//     }
//         *display_ptr = display_value;
//         printf("Valor al que apunta el puntero: %x \n", *display_ptr);
//         sleep(0.5); 
//     }

//     munmap(virtual_base, 4096);
//     close(fd);
//     return 0;
// }

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include<unistd.h>
#include <sys/mman.h>

#define LW_BASE 0xFF200000  // Address of the GPIO controller in LWHPS2FPGA bridge
#define LED_OFFSET 0x00
#define SWITCHES_OFFSET 0x10
#define NUM_OFFSET 0x20

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

    volatile uint32_t* led_ptr = (volatile uint32_t*)((uint8_t*)virtual_base + LED_OFFSET);
    volatile uint32_t* switches_ptr = (volatile uint32_t*)((uint8_t*)virtual_base + SWITCHES_OFFSET);
    volatile uint32_t* num_ptr = (volatile uint32_t*)((uint8_t*)virtual_base + NUM_OFFSET);

    while (1) {
        *led_ptr = 0x01; 
        // sleep(1); 
        // *led_ptr = 0x00; 
        // sleep(1); 
        int switches_value = *((volatile uint32_t *)switches_ptr);
        *num_ptr = num_to_display(switches_value);
        // printf("Valor de los switches: %x \n", switches_value);
        // printf("Valor de num: %x \n", *((volatile uint32_t *)num_ptr));
        // printf("---------------------- \n");
    }

    munmap(virtual_base, 4096);
    close(fd);
    return 0;
}