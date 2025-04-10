#ifndef IDT_H
#define IDT_H

#include <util/util.h>

struct IDTEntry {
    unsigned short offset_low;  // Lower 16 bits of the handler function address
    unsigned short selector;    // Kernel segment selector
    unsigned char zero;         // Reserved, set to 0
    unsigned char type_attr;    // Type and attributes
    unsigned short offset_high; // Upper 16 bits of the handler function address
} __attribute__((packed));

struct IDTR {
    unsigned short limit;       // Size of the IDT - 1
    unsigned int base;          // Address of the IDT
} __attribute__((packed));

void idt_init();
void idt_set_entry(int index, unsigned int handler, unsigned short selector, unsigned char type_attr);

#endif