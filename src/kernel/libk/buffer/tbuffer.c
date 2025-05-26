#include "buffer.h"

void tbuffer_init(tbuffer_t* buffer, uint16_t* address, size_t width, size_t height) {
    buffer->address = address;
    buffer->width = width;
    buffer->height = height;
}

void tbuffer_fill(tbuffer_t* buffer, uint16_t c) {
    for (size_t i = 0; i < buffer->width * buffer->height; i++)
        buffer->address[i] = c;
}

void tbuffer_set(tbuffer_t* buffer, uint16_t c, size_t x, size_t y) {
    buffer->address[y * buffer->width + x] = c;
}