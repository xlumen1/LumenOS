#pragma once
#include <stdint.h>

struct fat32_bpb {
    uint8_t  jmp[3];
    char     oem[8];

    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t  fat_count;
    uint16_t root_entries;
    uint16_t total_sectors16;
    uint8_t  media;
    uint16_t fat_size16;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors32;

    uint32_t fat_size32;
    uint16_t flags;
    uint16_t version;
    uint32_t root_cluster;
    uint16_t fsinfo_sector;
    uint16_t backup_boot;
    uint8_t  reserved[12];

    uint8_t  drive;
    uint8_t  nt_flags;
    uint8_t  sig;
    uint32_t vol_id;
    char     label[11];
    char     type[8];
} __attribute__((packed));

struct fat_dirent {
    char     name[11];
    uint8_t  attr;
    uint8_t  nt;
    uint8_t  crt_tenth;
    uint16_t crt_time;
    uint16_t crt_date;
    uint16_t acc_date;
    uint16_t cluster_hi;
    uint16_t mod_time;
    uint16_t mod_date;
    uint16_t cluster_lo;
    uint32_t size;
} __attribute__((packed));
