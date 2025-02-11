#include "time.h"

unsigned char bcd_to_bin(unsigned char bcd) {
    return ((bcd / 16) * 10) + (bcd % 16);
}