#include "vga.h"

void vga_clear() {
    for (unsigned char x = 0; x < VGA_WIDTH; x++) {
        for (unsigned char y = 0; y < VGA_WIDTH; y++) {
            vga_put(' ', VGA_COLOR(VGA_LIGHT_GREY, VGA_BLACK), x, y);
        }
    }
}