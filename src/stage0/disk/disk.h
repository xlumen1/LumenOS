#ifndef DISK_H
#define DISK_H

#include <stdint.h>

typedef struct {
    uint32_t sector_count;
    uint32_t sector_size;
} disk_info_t;

void disk_init();
int disk_read(uint32_t start, uint8_t* buffer, uint32_t count);
int disk_write(uint32_t start, const uint8_t* buffer, uint32_t count);
disk_info_t disk_get_info();

#endif