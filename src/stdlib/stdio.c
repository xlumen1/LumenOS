#include <stdio.h>
#include <stdarg.h>
#include <vga/vga.h>

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
        if (format[i] == '\n' || format[i] == '\r') {
            vga_newline();
            continue;
        }
        if (format[i] == '%') {
            i++;
            // Field width parsing (minimum, no padding)
            int width = 0;
            while (format[i] >= '0' && format[i] <= '9') {
                width = width * 10 + (format[i] - '0');
                i++;
            }
            switch (format[i]) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    if (c == '\n' || c == '\r')
                        vga_newline();
                    else
                        vga_put(c, VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    break;
                }
                case 's': {
                    char* s = va_arg(args, char*);
                    int len = 0;
                    while (s && s[len]) len++;
                    for (int j = len; j < width; j++) {
                        if (format[i] == '\n' || format[i] == '\r') {
                            vga_newline();
                            continue;
                        }
                        vga_put(' ', VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    }
                    for (int j = 0; s && s[j]; j++) {
                        if (s[j] == '\n' || s[j] == '\r')
                            vga_newline();
                        else
                            vga_put(s[j], VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    }
                    break;
                }
                case 'd':
                case 'i': {
                    int val = va_arg(args, int);
                    itoa(val, buffer, 10);
                    int len = 0;
                    while (buffer[len]) len++;
                    for (int j = len; j < width; j++)
                        vga_put(' ', VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    vga_print(buffer, VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    break;
                }
                case 'u': {
                    unsigned int val = va_arg(args, unsigned int);
                    itoa(val, buffer, 10);
                    int len = 0;
                    while (buffer[len]) len++;
                    for (int j = len; j < width; j++)
                        vga_put(' ', VGA_COLOR(VGA_WHITE, VGA_BLACK));
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
                case 'X': {
                    int val = va_arg(args, int);
                    itoa(val, buffer, 16);
                    // Uppercase hex
                    for (int j = 0; buffer[j]; j++)
                        if (buffer[j] >= 'a' && buffer[j] <= 'f')
                            buffer[j] -= 32;
                    vga_print("0x", VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    vga_print(buffer, VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    break;
                }
                case 'o': {
                    unsigned int val = va_arg(args, unsigned int);
                    itoa(val, buffer, 8);
                    vga_print(buffer, VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    break;
                }
                case 'p': {
                    void* ptr = va_arg(args, void*);
                    vga_print("0x", VGA_COLOR(VGA_WHITE, VGA_BLACK));
                    itoa((unsigned int)(uintptr_t)ptr, buffer, 16);
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

int sprintf(char* str, const char* format, ...) {
    va_list args;
    va_start(args, format);

    int pos = 0;
    char buffer[32];

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            int width = 0;
            while (format[i] >= '0' && format[i] <= '9') {
                width = width * 10 + (format[i] - '0');
                i++;
            }
            switch (format[i]) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    str[pos++] = c;
                    break;
                }
                case 's': {
                    char* s = va_arg(args, char*);
                    int len = 0;
                    while (s && s[len]) len++;
                    for (int j = len; j < width; j++)
                        str[pos++] = ' ';
                    for (int j = 0; s && s[j]; j++)
                        str[pos++] = s[j];
                    break;
                }
                case 'd':
                case 'i': {
                    int val = va_arg(args, int);
                    itoa(val, buffer, 10);
                    int len = 0;
                    while (buffer[len]) len++;
                    for (int j = len; j < width; j++)
                        str[pos++] = ' ';
                    for (int j = 0; buffer[j]; j++)
                        str[pos++] = buffer[j];
                    break;
                }
                case 'u': {
                    unsigned int val = va_arg(args, unsigned int);
                    itoa(val, buffer, 10);
                    int len = 0;
                    while (buffer[len]) len++;
                    for (int j = len; j < width; j++)
                        str[pos++] = ' ';
                    for (int j = 0; buffer[j]; j++)
                        str[pos++] = buffer[j];
                    break;
                }
                case 'x': {
                    int val = va_arg(args, int);
                    itoa(val, buffer, 16);
                    str[pos++] = '0';
                    str[pos++] = 'x';
                    for (int j = 0; buffer[j]; j++)
                        str[pos++] = buffer[j];
                    break;
                }
                case 'X': {
                    int val = va_arg(args, int);
                    itoa(val, buffer, 16);
                    str[pos++] = '0';
                    str[pos++] = 'x';
                    for (int j = 0; buffer[j]; j++) {
                        char c = buffer[j];
                        if (c >= 'a' && c <= 'f')
                            c -= 32;
                        str[pos++] = c;
                    }
                    break;
                }
                case 'o': {
                    unsigned int val = va_arg(args, unsigned int);
                    itoa(val, buffer, 8);
                    for (int j = 0; buffer[j]; j++)
                        str[pos++] = buffer[j];
                    break;
                }
                case 'p': {
                    void* ptr = va_arg(args, void*);
                    str[pos++] = '0';
                    str[pos++] = 'x';
                    itoa((unsigned int)(uintptr_t)ptr, buffer, 16);
                    for (int j = 0; buffer[j]; j++)
                        str[pos++] = buffer[j];
                    break;
                }
                case '%': {
                    str[pos++] = '%';
                    break;
                }
                default: {
                    str[pos++] = '%';
                    str[pos++] = format[i];
                    break;
                }
            }
        } else {
            str[pos++] = format[i];
        }
    }
    str[pos] = '\0';
    va_end(args);
    return pos;
}
