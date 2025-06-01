#ifndef TIME_H
#define TIME_H

#include <stdint.h>
#include <io/io.h>

struct time {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t century;
};

struct time get_time();

unsigned char bcd_to_bin(unsigned char bcd);

#endif