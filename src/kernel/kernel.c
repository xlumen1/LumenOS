#include "libk/vga/vga.h"
void kmain() {
    vga_clear();
    vga_put('A', VGA_COLOR(VGA_WHITE, VGA_BLACK), 0, 0);
}