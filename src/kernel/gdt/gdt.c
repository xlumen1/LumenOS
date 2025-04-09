#include "gdt.h"
#include "../libk/log/log.h"

void gdt_encode(unsigned char* target, struct GDT source) {
    if (source.limit > 0xFFFFF) {
        log_error("Limit too big (Must Be At Most 0xFFFFF)");
        return;
    }
    log_info("Encoding GDT Entry");

    // Encode limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;

    // Encode base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;

    // Encode access and flags
    target[5] = source.access;
    target[6] |= (source.flags << 4);
}