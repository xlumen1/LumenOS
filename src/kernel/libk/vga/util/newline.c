#include "../vga.h"

// Move the cursor to the next line
void vga_newline() {
    vga_pos(0, vga_row + 1);
}