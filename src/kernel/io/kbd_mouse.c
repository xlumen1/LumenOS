#include "io.h"

void keyboard_handler(isr_regs_t* regs) {
    uint8_t scancode = inb(0x60);
    printf("Keyboard interrupt! Scancode: %x", scancode);

    outb(0x20, 0x20); // Send EOI to Master PIC
}

void timer_handler(isr_regs_t* regs) {
    
}