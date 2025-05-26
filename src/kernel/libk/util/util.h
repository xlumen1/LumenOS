#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

size_t strlen(const char* str);

void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);

#endif