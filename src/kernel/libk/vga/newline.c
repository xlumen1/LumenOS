#include "vga.h"

void vga_newline() {
    vga_pos(0, vga_row + 1);
}