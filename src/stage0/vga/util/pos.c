#include "../vga.h"

// Set the cursor position
void vga_pos(unsigned char col, unsigned char row) {
    vga_col = col;
    vga_row = row;
    vga_cursor_pos(col, row);
}