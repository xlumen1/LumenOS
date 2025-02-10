#include "vga.h"

void vga_clear(unsigned char color) {
    tbuffer_fill(&vga_buffer, VGA_CHAR(' ', color));
}