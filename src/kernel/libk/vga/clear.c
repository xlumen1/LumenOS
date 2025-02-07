#include "vga.h"

void vga_clear(unsigned char color) {
    for (unsigned char x = 0; x < VGA_WIDTH; x++) {
        for (unsigned char y = 0; y < VGA_WIDTH; y++) {
            vga_set(' ', color, x, y);
        }
    }
}