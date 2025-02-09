#include "buffer.h"

void buffer_init(buffer_t* buffer, char* address, size_t size) {
    buffer->address = address;
    buffer->size = size;
}

void buffer_fill(buffer_t* buffer, char c) {
    for (size_t i = 0; i < buffer->size; i++)
        buffer->address[i] = c;
}

void buffer_set(buffer_t* buffer, char c, size_t index) {
    buffer->address[index] = c;
}

char buffer_get(buffer_t* buffer, size_t index) {
    return buffer->address[index];
}