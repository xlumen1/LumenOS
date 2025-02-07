#include "vga.h"

void vga_pos(unsigned char col, unsigned char row) {
    vga_col = col;
    vga_row = row;
}