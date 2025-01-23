#include "vga.h"

void vga_put(char a, unsigned char color, unsigned char col, unsigned char row) {
    vga_buffer[row * VGA_WIDTH + col] = (short) a | (short) color << 8;
}