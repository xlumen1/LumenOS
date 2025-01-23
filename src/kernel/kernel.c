#include "libk/vga/vga.h"

#define DEFCOL VGA_COLOR(VGA_WHITE, VGA_BLACK)

void kmain() {
    vga_clear();
    for (char i = 0; i < 256; i++) {
        vga_put((char)i, DEFCOL);
    }
}