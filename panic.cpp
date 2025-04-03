/*
    testing piece of code for a "kernel panic" module
    - outputs a kernel panic message and halts all processes
    - still an early version of the actual module that I'll use
*/

#include <stdint.h>

// declares the VGA address, width and height of the VGA window

constexpr uintptr_t VGA_A = 0xB8000;        
constexpr int SCREEN_W = 80;         
constexpr int SCREEN_H = 25;         

constexpr uint8_t BG = 0x0F;    // "white on black" text constant


// string output system, I cut off the integer part

class VGA {
public:
    volatile uint16_t *vga_buffer;
    VGA() : vga_buffer(reinterpret_cast<uint16_t*>(VGA_A)) {}
    void outputString(const char* str, int col, int row, uint8_t attr) {     
        int offset = row * SCREEN_W + col;
        for (int i = 0; str[i] != '\0'; i++) {
            vga_buffer[offset + i] = (attr << 8) | str[i];
        }
    }
};

/*
kernel panic function
*/
extern "C" void kernelPanic(){
    VGA vga;
    vga.outputString("[!] Kernel panic", 1,1, BG);

    volatile bool halt = false;
    *(volatile int*)nullptr = 0;     // null pointer dereference, forces the system to a halt
    halt = true;

    /*
       backup function - if the null pointer failed,
       the system will enter an infinite loop
    */

    if (halt) {
        while (true) {}
    }   
}
