#ifndef GDT_H
#define GDT_H

#include <log/log.h>
#include <stdint.h>

// Macros

#define GDT_ENTRIES 3

// Structs

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Functions

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void gdt_install();

#endif