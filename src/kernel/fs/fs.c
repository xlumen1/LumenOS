#include "fs.h"
#include <string.h>

// Basicly just pseudo-code, I'll write the real code later

#ifdef ENABLE_FS
int disk_read(uint32_t sector, void* buf, uint32_t count);
int disk_write(uint32_t sector, const void* buf, uint32_t count);

static int read_header(char* magic, uint32_t* entry_count) {
    uint8_t buf[8];
    if (disk_read(1, buf, 1) != 0) return -1;
    memcpy(magic, buf, 4);
    *entry_count = *(uint32_t*)(buf + 4);
    return 0;
}

static int write_header(const char* magic, uint32_t entry_count) {
    uint8_t buf[8];
    memcpy(buf, magic, 4);
    *(uint32_t*)(buf + 4) = entry_count;
    return disk_write(1, buf, 1);
}

// MANAGEMENT OPS

int lufs_format() {
    const char magic[4] = {'L','u','F','S'};
    if (write_header(magic, 0) != 0) return -1;
    uint8_t zero[ENTRY_SIZE * ENTRIES_PER_SECTOR] = {0};
    for (int i = 2; i < 10; ++i) {
        if (disk_write(i, zero, 1) != 0) return -1;
    }
    return 0;
}

int lufs_mount() {
    char magic[4];
    uint32_t entry_count;
    if (read_header(magic, &entry_count) != 0) return -1;
    if (memcmp(magic, "LuFS", 4) != 0) return -2;
    return 0;
}

// CRUD OPS

int lufs_create(const char* path, uint8_t isfile, char* data, uint32_t size) {
    return 0;
}

int lufs_read(struct FsEntry* file, uint8_t* data) {
    return 0;
}

int lufs_update(struct FsEntry* file, uint8_t* data) {
    return 0;
}

int lufs_delete(struct FsEntry* entry) {
    return 0;
}

// UTIL OPS

struct FsEntry lufs_frompath(const char* path) {
    struct FsEntry entry = NULL_ENTRY;
    return entry;
}

uint8_t lufs_isnull(struct FsEntry* entry) {
    return entry->name[0] == '?';
}

uint8_t lufs_isfile(struct FsEntry* entry) {
    return entry->isfile == 1;
}

struct FsEntry* lufs_children(struct FsEntry* directory) {
    return NULL;
}

#endif
