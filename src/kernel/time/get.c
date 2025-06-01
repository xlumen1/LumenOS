#include "time.h"

struct time get_time() {
    struct time t;
    outb(0x70, 0x00);
    t.second = bcd_to_bin(inb(0x71));
    outb(0x70, 0x02);
    t.minute = bcd_to_bin(inb(0x71));
    outb(0x70, 0x04);
    t.hour = bcd_to_bin(inb(0x71));
    outb(0x70, 0x07);
    t.day = bcd_to_bin(inb(0x71));
    outb(0x70, 0x08);
    t.month = bcd_to_bin(inb(0x71));
    outb(0x70, 0x09);
    t.year = bcd_to_bin(inb(0x71));
    outb(0x70, 0x32);
    t.century = bcd_to_bin(inb(0x71));
    return t;
}