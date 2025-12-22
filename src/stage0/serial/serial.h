#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <io/io.h>

#define COM1 0x3F8

void serial_init(uint16_t port);
void serial_write_char(char c, uint16_t port);
void serial_write(const char* str, uint16_t port);

#endif