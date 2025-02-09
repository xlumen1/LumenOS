#ifndef BUFFER_H
#define BUFFER_H

#include "../util/util.h"

typedef struct
{
    char* address;
    size_t size;
} buffer_t;

typedef struct
{
    char* address;
    size_t width;
    size_t height;
} buffer2d_t;

typedef struct
{
    short* address;
    size_t width;
    size_t height;
} tbuffer_t;

void buffer_init(buffer_t* buffer, char* address, size_t size);
void buffer_fill(buffer_t* buffer, char c);
void buffer_set(buffer_t* buffer, char c, size_t index);
char buffer_get(buffer_t* buffer, size_t index);

void buffer2d_init(buffer2d_t* buffer, char* address, size_t width, size_t height);
void buffer2d_fill(buffer2d_t* buffer, char c);
void buffer2d_set(buffer2d_t* buffer, char c, size_t x, size_t y);
char buffer2d_get(buffer2d_t* buffer, size_t x, size_t y);

void tbuffer_init(tbuffer_t* buffer, short* address, size_t width, size_t height);
void tbuffer_fill(tbuffer_t* buffer, short c);
void tbuffer_set(tbuffer_t* buffer, short c, size_t x, size_t y);
short tbuffer_get(tbuffer_t* buffer, size_t x, size_t y);

#endif