#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stdio.h>
#include <idt/isr.h>

struct mouse_info {
    union
    {
        struct
        {
            uint8_t yo:1;
            uint8_t xo:1;
            uint8_t ys:1;
            uint8_t xs:1;
            uint8_t ao:1;
            uint8_t bm:1;
            uint8_t br:1;
            uint8_t bl:1;
        } __attribute__ ((packed)) flags;
        uint8_t raw;
        
    } data;
    uint8_t x;
    uint8_t y;
} __attribute__ ((packed));

void keyboard_handler();
void keyboard_init();
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif