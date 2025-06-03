#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

void* memmove(void* dest, const void* src, size_t size);
void* memcpy(void* dest, const void* src, size_t size);
void* memset(void* dest, int value, size_t size);

size_t strlen(const char* str);
char* strstr(const char* haystack, const char* needle);
char* strchr(const char* s, int c);

int memcmp(const void* s1, const void* s2, size_t n);
int strncmp(const char* s1, const char* s2, size_t n);
char* strncpy(char* dest, const char* src, size_t n);
char* strtok(char* str, const char* delim);

#endif