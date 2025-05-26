#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <util/util.h>

#define SERIAL_PORT 0x3F8

void serial_init(void);
void serial_write_char(char c);
void serial_write(const char* str);

#endif