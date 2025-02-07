#include "vga.h"
#include "../util/util.h"

void vga_print(char* a, unsigned char color) {
    vga_write(a, color, strlen(a));
}