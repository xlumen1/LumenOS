#include "../vga.h"

// Write a string to the VGA buffer
void vga_print(char* a, unsigned char color) {
    vga_write(a, color, strlen(a));
}