#include "idt.h"

#define IDT_SIZE 256

struct IDTEntry idt[IDT_SIZE];
struct IDTR idtr;

void idt_set_entry(int index, unsigned int handler, unsigned short selector, unsigned char type_attr) {
    idt[index].offset_low = handler & 0xFFFF;
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].type_attr = type_attr;
    idt[index].offset_high = (handler >> 16) & 0xFFFF;
}

void idt_init() {
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (unsigned int)&idt;

    // Clear the IDT
    for (int i = 0; i < IDT_SIZE; i++) {
        idt_set_entry(i, 0, 0, 0);
    }

    // Load the IDT
    __asm__ volatile("lidt (%0)" : : "r"(&idtr));
}