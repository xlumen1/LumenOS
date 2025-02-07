#include "vga.h"

void vga_error(char* a) {
    vga_print(a, VGA_COLOR(VGA_RED, VGA_BLACK));
}