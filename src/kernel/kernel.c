#include "libk/vga/vga.h"

#define DEFCOL VGA_COLOR(VGA_WHITE, VGA_BLACK)

void kmain() {
    vga_clear();
    vga_write("Lumen OS", DEFCOL, 8);
}
