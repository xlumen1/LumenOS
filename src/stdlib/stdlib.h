#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <stddef.h>
#include <serial/serial.h>

#define HEAP_SIZE (64 * 1024)

void* malloc(size_t size);
void free(void* ptr);

#endif