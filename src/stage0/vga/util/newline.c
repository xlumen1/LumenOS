#include "../vga.h"

// Move the cursor to the next line
void vga_newline() {
    vga_pos(0, vga_row + 1);
    if (vga_row > VGA_HEIGHT - 1) {
        vga_scroll(1);
	    vga_pos(0, vga_row - 1);
    }
}
