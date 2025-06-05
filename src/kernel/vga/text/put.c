#include "../vga.h"

// Append a character to the VGA buffer
void vga_put(const char a, unsigned char color) {
    if (vga_col + 1 >= VGA_WIDTH)
        vga_newline();
    vga_set(a, color, vga_col, vga_row);
    vga_pos(vga_col + 1, vga_row);
    return;
}