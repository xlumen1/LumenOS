#include "vga.h"

void vga_write(char* a, unsigned char color, unsigned short length) {
	for (unsigned short i = 0; i < length; i++) {
		vga_put(a[i], color);
	}
}
