#include "fat32.h"
#include "fat32_internal.h"
#include <string.h>

#define FAT_EOC 0x0FFFFFF8

// -- BEGIN YOINK --

// Disk backend abstraction
typedef struct {
    int (*read)(uint32_t sector, void* buf, uint32_t count);
    int (*write)(uint32_t sector, const void* buf, uint32_t count);
    void* ctx;
} disk_backend_t;

static disk_backend_t* current_disk = 0;

// Memory disk backend
typedef struct {
    uint8_t* base;
    uint32_t sector_size;
    uint32_t num_sectors;
} memdisk_ctx_t;

static int memdisk_read(uint32_t sector, void* buf, uint32_t count) {
    memdisk_ctx_t* ctx = (memdisk_ctx_t*)current_disk->ctx;
    if (sector + count > ctx->num_sectors) return -1;
    memcpy(buf, ctx->base + sector * ctx->sector_size, count * ctx->sector_size);
    return 0;
}

static int memdisk_write(uint32_t sector, const void* buf, uint32_t count) {
    memdisk_ctx_t* ctx = (memdisk_ctx_t*)current_disk->ctx;
    if (sector + count > ctx->num_sectors) return -1;
    memcpy(ctx->base + sector * ctx->sector_size, buf, count * ctx->sector_size);
    return 0;
}

// Call this at boot with fsimg_addr and fsimg_size
void fs_use_memdisk(void* fsimg_addr, uint32_t fsimg_size) {
    static memdisk_ctx_t memdisk;
    static disk_backend_t backend;
    memdisk.base = (uint8_t*)fsimg_addr;
    memdisk.sector_size = 512;
    memdisk.num_sectors = fsimg_size / 512;
    backend.read = memdisk_read;
    backend.write = memdisk_write;
    backend.ctx = &memdisk;
    current_disk = &backend;
}

#define disk_read(sector, buf, count)  (current_disk->read((sector), (buf), (count)))
#define disk_write(sector, buf, count) (current_disk->write((sector), (buf), (count)))

// -- END YOINK --

static uint32_t cluster_to_lba(fat32_fs_t* fs, uint32_t cluster) {
    return fs->data_lba + (cluster - 2) * fs->sectors_per_cluster;
}

static uint8_t sector[512];

int fat32_mount(uint32_t part_lba, fat32_fs_t** out) {
    static fat32_fs_t fs;
    struct fat32_bpb bpb;

    disk_read(part_lba, 1, &bpb);

    if (bpb.fat_size32 == 0) return -1;
    if (bpb.bytes_per_sector != 512) return -1;

    fs.part_lba = part_lba;
    fs.fat_lba  = part_lba + bpb.reserved_sectors;
    fs.fat_sectors = bpb.fat_size32;
    fs.data_lba = fs.fat_lba + bpb.fat_count * bpb.fat_size32;

    fs.sectors_per_cluster = bpb.sectors_per_cluster;
    fs.bytes_per_cluster = fs.sectors_per_cluster * 512;

    fs.root_cluster = bpb.root_cluster;

    *out = &fs;
    return 0;
}

static uint32_t fat_next(fat32_fs_t* fs, uint32_t cluster) {
    uint32_t off = cluster * 4;
    uint32_t lba = fs->fat_lba + (off / 512);
    uint32_t ent = off % 512;

    disk_read(lba, 1, sector);
    return (*(uint32_t*)(sector + ent)) & 0x0FFFFFFF;
}

int fat32_open_root(fat32_fs_t* fs, fat32_file_t* out) {
    out->first_cluster = fs->root_cluster;
    out->size = 0;
    out->pos = 0;
    out->is_dir = 1;
    return 0;
}

int fat32_find(fat32_fs_t* fs, fat32_file_t* dir, const char name83[11], fat32_file_t* out) {
    uint32_t cl = dir->first_cluster;
    uint8_t buf[4096]; // enough for 8 sector clusters

    while (cl < FAT_EOC) {
        uint32_t lba = cluster_to_lba(fs, cl);
        disk_read(lba, fs->sectors_per_cluster, buf);

        struct fat_dirent* e = (void*)buf;
        for (uint32_t i = 0;
             i < fs->bytes_per_cluster / 32;
             i++) {

            if (e[i].name[0] == 0x00) return -1;
            if (e[i].name[0] == 0xE5) continue;
            if (e[i].attr == 0x0F) continue;

            if (!memcmp(e[i].name, name83, 11)) {
                out->first_cluster = (e[i].cluster_hi << 16) | e[i].cluster_lo;
                out->size = e[i].size;
                out->pos = 0;
                out->is_dir = (e[i].attr & 0x10) != 0;
                return 0;
            }
        }

        cl = fat_next(fs, cl);
    }
    return -1;
}

uint32_t fat32_read(fat32_fs_t* fs, fat32_file_t* file, uint32_t offset, uint32_t size, void* out) {
    if (offset >= file->size) return 0;
    if (offset + size > file->size)
        size = file->size - offset;

    uint32_t cl = file->first_cluster;
    uint32_t skip = offset / fs->bytes_per_cluster;
    uint32_t off  = offset % fs->bytes_per_cluster;

    for (uint32_t i = 0; i < skip; i++)
        cl = fat_next(fs, cl);

    uint8_t* dst = out;
    uint8_t buf[4096];

    uint32_t remaining = size;

    while (remaining && cl < FAT_EOC) {
        uint32_t lba = cluster_to_lba(fs, cl);
        disk_read(lba, fs->sectors_per_cluster, buf);

        uint32_t copy = fs->bytes_per_cluster - off;
        if (copy > remaining) copy = remaining;

        memcpy(dst, buf + off, copy);

        dst += copy;
        remaining -= copy;
        off = 0;
        cl = fat_next(fs, cl);
    }

    return size - remaining;
}

