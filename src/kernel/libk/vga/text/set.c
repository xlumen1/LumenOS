#include "../vga.h"

// Set a character in the VGA buffer
void vga_set(char a, unsigned char color, unsigned char col, unsigned char row) {
    tbuffer_set(&vga_buffer, VGA_CHAR(a, color), col, row);
}