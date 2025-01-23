#include "vga.h"

void vga_put(char a, unsigned char color) {
    vga_buffer[vga_row * VGA_WIDTH + vga_col] = (short) a | (short) color << 8;
    if (++vga_col >= VGA_WIDTH) {
        vga_col == 0;
        vga_row++;
    }
}