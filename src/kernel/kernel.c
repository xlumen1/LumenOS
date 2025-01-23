#include "libk/vga/vga.h"

#define DEFCOL VGA_COLOR(VGA_WHITE, VGA_BLACK)

void kmain() {
    vga_clear();
    vga_put('L', DEFCOL);
    vga_put('u', DEFCOL);
    vga_put('m', DEFCOL);
    vga_put('e', DEFCOL);
    vga_put('n', DEFCOL);
}
