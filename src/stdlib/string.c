#include "string.h"

void* memmove(void* dest, const void* src, size_t size) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;

    if (d == s || size == 0)
        return dest;

    if (d < s || d >= s + size) {
        for (size_t i = 0; i < size; i++) {
            d[i] = s[i];
        }
    } else {
        for (size_t i = size; i > 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }

    return dest;
}

void* memcpy(void* dest, const void* src, size_t size) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;

    for (size_t i = 0; i < size; i++) {
        d[i] = s[i];
    }

    return dest;
}

void* memset(void* dest, int value, size_t size) {
    unsigned char *d = (unsigned char*)dest;
    unsigned char v = (unsigned char)value;
    for (size_t i = 0; i < size; i++) {
        d[i] = v;
    }
    return dest;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

char* strstr(const char* haystack, const char* needle) {
    if (!*needle) return (char*)haystack;
    for (; *haystack; ++haystack) {
        const char *h = haystack, *n = needle;
        while (*h && *n && *h == *n) { ++h; ++n; }
        if (!*n) return (char*)haystack;
    }
    return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    const unsigned char* a = (const unsigned char*)s1;
    const unsigned char* b = (const unsigned char*)s2;
    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i])
            return a[i] - b[i];
    }
    return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0')
            return (unsigned char)s1[i] - (unsigned char)s2[i];
    }
    return 0;
}

char* strncpy(char* dest, const char* src, size_t n) {
    size_t i = 0;
    for (; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
    return dest;
}

// Simple strtok implementation (not thread-safe)
char* strtok(char* str, const char* delim) {
    static char* next = 0;
    if (str)
        next = str;
    if (!next)
        return 0;
    // Skip leading delimiters
    while (*next && strchr(delim, *next))
        next++;
    if (!*next)
        return 0;
    char* start = next;
    while (*next && !strchr(delim, *next))
        next++;
    if (*next) {
        *next = '\0';
        next++;
    } else {
        next = 0;
    }
    return start;
}

// Helper for strtok
char* strchr(const char* s, int c) {
    while (*s) {
        if (*s == (char)c)
            return (char*)s;
        s++;
    }
    return 0;
}