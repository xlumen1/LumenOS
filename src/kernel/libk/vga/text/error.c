#include "../vga.h"

// Print an error message to the VGA buffer
void vga_error(char* a) {
    vga_print(a, VGA_COLOR(VGA_RED, VGA_BLACK));
}