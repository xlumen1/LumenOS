#pragma once
#include <stdint.h>

typedef struct fat32_fs {
    uint32_t part_lba;

    uint32_t fat_lba;
    uint32_t data_lba;
    uint32_t fat_sectors;

    uint32_t sectors_per_cluster;
    uint32_t bytes_per_cluster;

    uint32_t root_cluster;
} fat32_fs_t;

typedef struct fat32_file {
    uint32_t first_cluster;
    uint32_t size;
    uint32_t pos;
    uint8_t  is_dir;
} fat32_file_t;

int fat32_mount(uint32_t partition_lba, fat32_fs_t** out);
int fat32_open_root(fat32_fs_t* fs, fat32_file_t* out);

int fat32_find(fat32_fs_t* fs, fat32_file_t* dir, const char name83[11], fat32_file_t* out);

uint32_t fat32_read(fat32_fs_t* fs, fat32_file_t* file, uint32_t offset, uint32_t size, void* out);

void fs_use_memdisk(void* fsimg_addr, uint32_t fsimg_size);
