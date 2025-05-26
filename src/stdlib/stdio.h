#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <vga/vga.h>

void itoa(int value, char* str, int base);
void printf(const char* format, ...);

#endif