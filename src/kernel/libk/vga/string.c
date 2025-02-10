#include "vga.h"

short* vga_string(char* a, unsigned char color) {
    short* out;
    for (unsigned short i = 0; i < strlen(a); i++) {
        out[i] = (short) a[i] | (short) color << 8;
    }
    return out;
}