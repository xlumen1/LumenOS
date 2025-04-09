#ifndef GDT_H
#define GDT_H

#include "../libk/vga/vga.h"

struct GDT
{
    unsigned int base;
    unsigned int limit;
    unsigned char access;
    unsigned char flags;
};

struct GDTR
{
    unsigned short size; // Change to 16 bits as GDTR size is 2 bytes
    unsigned int offset;
} __attribute__((packed));


void gdt_encode(unsigned char* target, struct GDT source);

#endif