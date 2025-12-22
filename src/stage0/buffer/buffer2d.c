#include "buffer.h"

void buffer2d_init(buffer2d_t* buffer, char* address, size_t width, size_t height) {
    buffer->address = address;
    buffer->width = width;
    buffer->height = height;
}

void buffer2d_fill(buffer2d_t* buffer, char c) {
    for (size_t i = 0; i < buffer->width * buffer->height; i++)
        buffer->address[i] = c;
}

void buffer2d_set(buffer2d_t* buffer, char c, size_t x, size_t y) {
    buffer->address[y * buffer->width + x] = c;
}

char buffer2d_get(buffer2d_t* buffer, size_t x, size_t y) {
    return buffer->address[y * buffer->width + x];
}