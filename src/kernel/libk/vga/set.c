#include "vga.h"

void vga_set(char a, unsigned char color, unsigned char col, unsigned char row) {
    //vga_buffer.address[row * VGA_WIDTH + col] = (short) a | (short) color << 8;
    tbuffer_set(&vga_buffer, VGA_CHAR(a, color), col, row);
}