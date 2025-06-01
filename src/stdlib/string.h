#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

void* memmove(void* dest, const void* src, size_t size);
void* memcpy(void* dest, const void* src, size_t size);
void* memset(void* dest, int value, size_t size);

size_t strlen(const char* str);

#endif