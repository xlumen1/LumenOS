#include "../vga.h"

void vga_cursor_setmode(uint8_t enable) {
    if (enable) {
        outb(0x3D4, 0x0A);
        outb(0x3D5, (inb(0x3D5) & 0xC0) | 0);
        outb(0x3D4, 0x0B);
        outb(0x3D5, (inb(0x3D5) & 0xE0) | 15);

    } else {
        outb(0x3D4, 0x0A);
        outb(0x3D5, 0x20);
    }
}
void vga_cursor_pos(uint8_t x, uint8_t y) {
    uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}