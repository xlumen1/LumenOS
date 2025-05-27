#include "io.h"

uint8_t keyboard_shift;

char km1[256] = {0};

void keyboard_init() {
    km1[0x02] = '1';
    km1[0x03] = '2';
    km1[0x04] = '3';
    km1[0x05] = '4';
    km1[0x06] = '5';
    km1[0x07] = '6';
    km1[0x08] = '7';
    km1[0x09] = '8';
    km1[0x0a] = '9';
    km1[0x0b] = '0';
    km1[0x0c] = '-';
    km1[0x0d] = '=';
    km1[0x0e] = '\b';
    km1[0x0f] = '\t';
    km1[0x10] = 'q';
    km1[0x11] = 'w';
    km1[0x12] = 'e';
    km1[0x13] = 'r';
    km1[0x14] = 't';
    km1[0x15] = 'y';
    km1[0x16] = 'u';
    km1[0x17] = 'i';
    km1[0x18] = 'o';
    km1[0x19] = 'p';
    km1[0x1a] = '[';
    km1[0x1b] = ']';
    km1[0x1c] = '\0';
    km1[0x1d] = '\0';
    km1[0x1e] = 'a';
    km1[0x1f] = 's';
    km1[0x20] = 'd';
    km1[0x21] = 'f';
    km1[0x22] = 'g';
    km1[0x23] = 'h';
    km1[0x24] = 'j';
    km1[0x25] = 'k';
    km1[0x26] = 'l';
    km1[0x27] = ';';
    km1[0x28] = '\'';
    km1[0x29] = '`';
    km1[0x2a] = '\0';
    km1[0x2b] = '\\';
    km1[0x2c] = 'z';
    km1[0x2d] = 'x';
    km1[0x2e] = 'c';
    km1[0x2f] = 'v';
    km1[0x30] = 'b';
    km1[0x31] = 'n';
    km1[0x32] = 'm';
    km1[0x33] = ',';
    km1[0x34] = '.';
    km1[0x35] = '/';
    km1[0x36] = '\0';
    km1[0x37] = '*';
    km1[0x38] = '\0';
    km1[0x39] = ' ';
    km1[0x3a] = '\0';
    km1[0x3b] = '\0';
    km1[0x3c] = '\0';
    km1[0x3d] = '\0';
    km1[0x3e] = '\0';
    km1[0x3f] = '\0';
    km1[0x40] = '\0';
    km1[0x41] = '\0';
    km1[0x42] = '\0';
    km1[0x43] = '\0';
    km1[0x44] = '\0';
    km1[0x45]= '\0';
    km1[0x46] = '\0';
    km1[0x47] = '7';
    km1[0x48] = '8';
    km1[0x49] = '9';
    km1[0x4a] = '-';
    km1[0x4b] = '4';
    km1[0x4c] = '5';
    km1[0x4d] = '6';
    km1[0x4e] = '+';
    km1[0x4f] = '1';
    km1[0x50] = '2';
    km1[0x51] = '3';
    km1[0x52] = '0';
    km1[0x53] = '.';
}

uint8_t is_letter(char character) {
    uint8_t c = (uint8_t)character;
    return (('A' <= c) && (c <= 'Z')) || (('a' <= c) && (c <= 'z'));
}

uint8_t is_number(char character) {
    uint8_t c = (uint8_t)character;
    return (('0' <= c) && (c <= '9'));
}

char apply_shift(char c) {
    if (!keyboard_shift)
        return c;
    if (is_letter(c))
        return c - 0x20;
    if (is_number(c))
        return c - 0x10;
    return 0;
}

char keyboard_get_char(uint8_t scancode) {
    if (scancode == 0x2A) {
        keyboard_shift = 255;
        return 0;
    }
    if (scancode == 0xAA) {
        keyboard_shift = 0;
        return 0;
    }
    if (scancode == 0x1C) {
        vga_newline();
        return 0;
    }
    switch (scancode)
    {
    case 0x2A:
        keyboard_shift = 255;
        return 0;
    
    case 0x36:
        keyboard_shift = 255;
        return 0;

    case 0xAA:
        keyboard_shift = 0;
        return 0;
    
    case 0xB6:
        keyboard_shift = 0;
        return 0;
    
    case 0x1C:
        vga_newline();
        return 0;
    
    case 0x0E:
        if (vga_col > 0) {
            vga_pos(vga_col - 1, vga_row);
        } else {
            vga_pos(VGA_WIDTH - 1, vga_row - 1);
            serial_write_char((char)(tbuffer_get(&vga_buffer, vga_col, vga_row) & 0x00FF), COM1);
            if ((char)(tbuffer_get(&vga_buffer, vga_col, vga_row) & 0x00FF) == '\0') {
                while ((char)(tbuffer_get(&vga_buffer, vga_col, vga_row) & 0x00FF) == '\0') {
                    vga_pos(vga_col - 1, vga_row);
                }
                vga_pos(vga_col + 1, vga_row);
            }
        }
        vga_set('\0', VGA_COLOR(VGA_WHITE, VGA_BLACK), vga_col, vga_row);
        return 0;
    
    default:
        return apply_shift(km1[scancode]);
    }
}

void keyboard_handler() {
    uint8_t scancode = inb(0x60);
    char c = keyboard_get_char(scancode);
    if (c)
        vga_put(c, VGA_COLOR(VGA_WHITE, VGA_BLACK));

    outb(0x20, 0x20);
}