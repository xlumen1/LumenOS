#include "libk/vga/vga.h"

#define DEFCOL VGA_COLOR(VGA_WHITE, VGA_BLACK)

void kmain() {
    vga_clear();
    for (unsigned char x = 0; x < 128; x++) {
        vga_put(x, DEFCOL);
    }
}