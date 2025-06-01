#include "../vga.h"

// Write a string to the VGA buffer
void vga_write(const char* a, unsigned char color, unsigned short length) {
	for (unsigned short i = 0; i < length; i++) {
		vga_put(a[i], color);
	}
}
