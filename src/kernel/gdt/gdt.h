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

void gdt_encode(unsigned char* target, struct GDT source);

#endif