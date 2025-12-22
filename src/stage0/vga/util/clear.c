#include "../vga.h"

// Clear the VGA buffer
void vga_clear(unsigned char color) {
    tbuffer_fill(&vga_buffer, VGA_CHAR('\0', color));
}
