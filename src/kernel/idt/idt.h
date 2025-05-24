#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include <log/log.h>
#include <util/util.h>

// Macros

#define IDT_ENTRIES 256

// Structs

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no;
    uint32_t err_code;
} __attribute__((packed)) isr_regs_t;

// Functions

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install();
void isr_install();

#endif