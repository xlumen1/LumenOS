#include "serial.h"

void serial_init() {
    outb(SERIAL_PORT + 1, 0x00);
    outb(SERIAL_PORT + 3, 0x80);
    outb(SERIAL_PORT + 0, 0x03);
    outb(SERIAL_PORT + 1, 0x00);
    outb(SERIAL_PORT + 3, 0x03);
    outb(SERIAL_PORT + 2, 0xC7);
    outb(SERIAL_PORT + 4, 0x0B);
}

int serial_is_transmit_ready() {
    return inb(SERIAL_PORT + 5) & 0x20;
}

void serial_write_char(char c) {
    while (!serial_is_transmit_ready());
    outb(SERIAL_PORT, c);
}

void serial_write(const char* str) {
    while (*str) {
        if (*str == '\n') {
            serial_write_char('\r');
        }
        serial_write_char(*(str++));
    }
}
