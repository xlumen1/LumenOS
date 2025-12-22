#include "serial.h"

void serial_init(uint16_t port) {
    outb(port + 1, 0x00);
    outb(port + 3, 0x80);
    outb(port + 0, 0x03);
    outb(port + 1, 0x00);
    outb(port + 3, 0x03);
    outb(port + 2, 0xC7);
    outb(port + 4, 0x0B);
}

int serial_is_transmit_ready(uint16_t port) {
    return inb(port + 5) & 0x20;
}

void serial_write_char(char c, uint16_t port) {
    while (!serial_is_transmit_ready(port));
    outb(port, c);
}

void serial_write(const char* str, uint16_t port) {
    while (*str) {
        if (*str == '\n') {
            serial_write_char('\r', port);
        }
        serial_write_char(*(str++), port);
    }
}
