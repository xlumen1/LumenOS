#ifndef STRING_H
#define STRING_H

#include <stdint.h>

void* memmove(void* dest, const void* src, size_t size);

size_t strlen(const char* str);

#endif