#include "../vga.h"

// Initialize the VGA buffer
void vga_init() {
    tbuffer_init(&vga_buffer, VGA_ADDRESS, VGA_WIDTH, VGA_HEIGHT);
}