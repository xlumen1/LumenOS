#ifndef TIME_H
#define TIME_H

#include "../util/util.h"

struct time {
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned char year;
    unsigned char century;
};

struct time get_time();

unsigned char bcd_to_bin(unsigned char bcd);

#endif