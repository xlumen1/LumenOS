#include "vga.h"

void vga_init() {
    tbuffer_init(&vga_buffer, VGA_ADDRESS, VGA_WIDTH, VGA_HEIGHT);
}