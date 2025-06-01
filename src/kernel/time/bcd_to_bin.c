#include "time.h"

uint8_t bcd_to_bin(uint8_t bcd) {
    return ((bcd / 16) * 10) + (bcd % 16);
}