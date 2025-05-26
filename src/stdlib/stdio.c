#include <stdio.h>

void itoa(int value, char* str, int base) {
    char* digits = "0123456789ABCDEF";
    char buf[32];
    int i = 0;
    unsigned int uval = (base == 10 && value < 0) ? -value : value;

    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    while (uval) {
        buf[i++] = digits[uval % base];
        uval /= base;
    }

    if (base == 10 && value < 0)
        buf[i++] = '-';

    int j = 0;
    while (i--) str[j++] = buf[i];
    str[j] = '\0';

}

void printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[32];

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '\n') {
            vga_newline();
            continue;
        }
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    vga_put(c, VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    break;
                }
                case 's': {
                    char* s = va_arg(args, char*);
                    vga_print(s, VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    break;
                }
                case 'd':
                case 'i': {
                    int val = va_arg(args, int);
                    itoa(val, buffer, 10);
                    vga_print(buffer, VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    break;
                }
                case 'x': {
                    int val = va_arg(args, int);
                    itoa(val, buffer, 16);
                    vga_print("0x", VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    vga_print(buffer, VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    break;
                }
                case '%': {
                    vga_put('%', VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    break;
                }
                default: {
                    vga_put('%', VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    vga_put(format[i], VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    break;
                }
            }
        } else {
            vga_put(format[i], VGA_COLOR(VGA_WHITE, VGA_BLACK));
        }
    }

    va_end(args);
}
