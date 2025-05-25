#include "../vga.h"

// Append a character to the VGA buffer
void vga_put(const char a, unsigned char color) {
    vga_set(a, color, vga_col, vga_row);
    if (++vga_col >= VGA_WIDTH) {
        vga_col = 0;
        vga_row++;
    }
}