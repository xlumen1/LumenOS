#ifndef UTIL_H
#define UTIL_H
typedef unsigned int size_t;

char* itoa(int val, int base);
size_t strlen(const char* str);

void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);

#endif