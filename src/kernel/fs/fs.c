#include "fs.h"
#include <string.h>
#include <stdint.h>

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

static int read_header(char* magic, uint32_t* entry_count) {
    uint8_t buf[8];
    if (disk_read(0, buf, 1) != 0) return -1;
    memcpy(magic, buf, 4);
    *entry_count = *(uint32_t*)(buf + 4);
    return 0;
}

static int write_header(const char* magic, uint32_t entry_count) {
    uint8_t buf[8];
    memcpy(buf, magic, 4);
    *(uint32_t*)(buf + 4) = entry_count;
    return disk_write(0, buf, 1);
}

// MANAGEMENT OPS

int lufs_format() {
    const char magic[4] = {'L','u','F','S'};
    if (write_header(magic, 0) != 0) return -1;
    uint8_t zero[ENTRY_SIZE * ENTRIES_PER_SECTOR] = {0};
    for (uint32_t i = 1; i < 1 + (MAX_ENTRIES / ENTRIES_PER_SECTOR); ++i) {
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

static int read_dir_children(const struct FsEntry* dir, struct FsEntry* out_entries, uint32_t* out_count);

static int read_entries(struct FsEntry* entries, uint32_t* out_count) {
    uint8_t buf[ENTRY_SIZE * ENTRIES_PER_SECTOR];
    uint32_t count = 0;
    for (uint32_t s = 1; s < 1 + (MAX_ENTRIES / ENTRIES_PER_SECTOR); ++s) {
        if (disk_read(s, buf, 1) != 0) return -1;
        for (int i = 0; i < ENTRIES_PER_SECTOR; ++i) {
            struct FsEntry* e = (struct FsEntry*)(buf + i * ENTRY_SIZE);
            if (e->name[0] != 0 && e->name[0] != '?') {
                entries[count++] = *e;
            }
        }
    }
    *out_count = count;
    return 0;
}

static int write_entry(uint32_t idx, struct FsEntry* entry) {
    uint32_t sector = 1 + idx / ENTRIES_PER_SECTOR;
    uint32_t offset = idx % ENTRIES_PER_SECTOR;
    uint8_t buf[ENTRY_SIZE * ENTRIES_PER_SECTOR];
    if (disk_read(sector, buf, 1) != 0) return -1;
    memcpy(buf + offset * ENTRY_SIZE, entry, sizeof(struct FsEntry));
    return disk_write(sector, buf, 1);
}

static int find_free_entry(uint32_t* idx) {
    uint8_t buf[ENTRY_SIZE * ENTRIES_PER_SECTOR];
    for (uint32_t s = 1; s < 1 + (MAX_ENTRIES / ENTRIES_PER_SECTOR); ++s) {
        if (disk_read(s, buf, 1) != 0) return -1;
        for (int i = 0; i < ENTRIES_PER_SECTOR; ++i) {
            struct FsEntry* e = (struct FsEntry*)(buf + i * ENTRY_SIZE);
            if (e->name[0] == 0 || e->name[0] == '?') {
                *idx = (s - 1) * ENTRIES_PER_SECTOR + i;
                return 0;
            }
        }
    }
    return -1;
}

static int find_entry(const char* path, struct FsEntry* out_entry, uint32_t* idx) {
    uint8_t buf[ENTRY_SIZE * ENTRIES_PER_SECTOR];
    for (uint32_t s = 1; s < 1 + (MAX_ENTRIES / ENTRIES_PER_SECTOR); ++s) {
        if (disk_read(s, buf, 1) != 0) return -1;
        for (int i = 0; i < ENTRIES_PER_SECTOR; ++i) {
            struct FsEntry* e = (struct FsEntry*)(buf + i * ENTRY_SIZE);
            if (strncmp(e->name, path, 32) == 0) {
                if (out_entry) *out_entry = *e;
                if (idx) *idx = (s - 1) * ENTRIES_PER_SECTOR + i;
                return 0;
            }
        }
    }
    return -1;
}

// CRUD OPS

int lufs_create(const char* path, uint8_t isfile, char* data, uint32_t size) {
    struct FsEntry entry;
    memset(&entry, 0, sizeof(entry));
    strncpy(entry.name, path, 31);
    entry.name[31] = 0;
    entry.size = size;
    entry.isfile = isfile;

    // Find free data blocks (naive: first available after entries)
    uint32_t needed_blocks = (size + 511) / 512;
    entry.start_block = 1 + (MAX_ENTRIES / ENTRIES_PER_SECTOR); // Data starts after entries
    entry.end_block = entry.start_block + needed_blocks - 1;
    entry.start_byte = 0;
    entry.end_byte = (size == 0) ? 0 : ((size - 1) % 512);

    // Write file data
    if (isfile && data && size > 0) {
        for (uint32_t i = 0; i < needed_blocks; ++i) {
            disk_write(entry.start_block + i, data + i * 512, 1);
        }
    }

    uint32_t idx;
    if (find_free_entry(&idx) != 0) return -1;
    if (write_entry(idx, &entry) != 0) return -1;
    return 0;
}

int lufs_read(struct FsEntry* file, uint8_t* data) {
    if (!file || !data || !file->isfile) return -1;
    uint32_t start_block = file->start_block;
    uint32_t end_block = file->end_block;
    uint32_t start_byte = file->start_byte;
    uint32_t end_byte = file->end_byte;
    uint32_t size = file->size;

    uint32_t data_offset = 0;
    for (uint32_t blk = start_block; blk <= end_block; ++blk) {
        uint8_t buf[512];
        if (disk_read(blk, buf, 1) != 0) return -1;

        uint32_t copy_start = 0;
        uint32_t copy_end = 511;

        if (blk == start_block) copy_start = start_byte;
        if (blk == end_block) copy_end = end_byte;

        uint32_t to_copy = copy_end - copy_start + 1;
        if (to_copy > size) to_copy = size;

        memcpy(data + data_offset, buf + copy_start, to_copy);
        data_offset += to_copy;
        size -= to_copy;
        if (size == 0) break;
    }
    return 0;
}

int lufs_update(struct FsEntry* file, uint8_t* data) {
    if (!file || !data || !file->isfile) return -1;
    uint32_t blocks = file->end_block - file->start_block + 1;
    uint32_t size = file->size;
    for (uint32_t i = 0; i < blocks; ++i) {
        uint32_t to_write = (size > 512) ? 512 : size;
        if (disk_write(file->start_block + i, data + i * 512, 1) != 0) return -1;
        size -= to_write;
        if (size == 0) break;
    }
    return 0;
}

int lufs_delete(struct FsEntry* entry) {
    if (!entry) return -1;
    struct FsEntry null_entry = NULL_ENTRY;
    uint32_t idx;
    if (find_entry(entry->name, NULL, &idx) != 0) return -1;
    if (write_entry(idx, &null_entry) != 0) return -1;
    return 0;
}

// UTIL OPS

struct FsEntry lufs_frompath(const char* path) {
    if (!path || !*path) {
        // Return root directory (first entry in entry table)
        uint8_t buf[ENTRY_SIZE];
        if (disk_read(1, buf, 1) != 0) return (struct FsEntry)NULL_ENTRY;
        return *(struct FsEntry*)buf;
    }
    char temp[256];
    strncpy(temp, path, 255);
    temp[255] = 0;
    char* token = strtok(temp, "/");
    struct FsEntry dir = lufs_frompath(""); // Start at root
    struct FsEntry entry = dir;
    while (token) {
        // Find child with this name
        struct FsEntry children[ENTRIES_PER_SECTOR * 8];
        uint32_t count = 0;
        if (read_dir_children(&dir, children, &count) != 0) return (struct FsEntry)NULL_ENTRY;
        int found = 0;
        for (uint32_t i = 0; i < count; ++i) {
            if (strncmp(children[i].name, token, 32) == 0) {
                entry = children[i];
                dir = entry;
                found = 1;
                break;
            }
        }
        if (!found) return (struct FsEntry)NULL_ENTRY;
        token = strtok(NULL, "/");
    }
    return entry;
}

struct FsEntry* lufs_children(struct FsEntry* directory) {
    static struct FsEntry children[ENTRIES_PER_SECTOR * 8]; // TODO: Make this use realloc for big dir sizes
    uint32_t count = 0;
    if (!directory) return NULL;
    if (read_dir_children(directory, children, &count) != 0) return NULL;
    return children;
}

uint8_t lufs_isnull(struct FsEntry* entry) {
    return entry->name[0] == '?' && entry->isfile == 255;
}

uint8_t lufs_isfile(struct FsEntry* entry) {
    return entry->isfile == 1;
}

static int read_dir_children(const struct FsEntry* dir, struct FsEntry* out_entries, uint32_t* out_count) {
    if (!dir || !out_entries || !out_count) return -1;
    if (!(dir->isfile == 0 && dir->size == 0)) return -1;
    // If empty directory, pointers are all zero
    if (dir->start_block == 0 && dir->end_block == 0 && dir->start_byte == 0 && dir->end_byte == 0) {
        *out_count = 0;
        return 0;
    }

    uint32_t start_offset = dir->start_block * 512 + dir->start_byte;
    uint32_t end_offset = dir->end_block * 512 + dir->end_byte;
    if (end_offset < start_offset) {
        *out_count = 0;
        return 0;
    }
    uint32_t total_bytes = end_offset - start_offset + 1;
    uint32_t entry_count = total_bytes / ENTRY_SIZE;
    if (entry_count == 0) {
        *out_count = 0;
        return 0;
    }
    char* out = malloc(256);
    sprintf(out, "Directory Name: %s\nDirectory Size: %u\nSubentry count: %u\n", dir->name, total_bytes, entry_count);
    serial_write(out, COM1);
    free(out);

    // Read the whole directory table into a buffer
    uint8_t table_buf[ENTRY_SIZE * entry_count];
    uint32_t bytes_read = 0;
    uint32_t offset = start_offset;
    while (bytes_read < total_bytes) {
        uint32_t sector = offset / 512;
        uint32_t sector_offset = offset % 512;
        uint8_t sector_buf[512];
        if (disk_read(sector, sector_buf, 1) != 0) return -1;
        uint32_t to_copy = 512 - sector_offset;
        if (to_copy > total_bytes - bytes_read)
            to_copy = total_bytes - bytes_read;
        memcpy(table_buf + bytes_read, sector_buf + sector_offset, to_copy);
        bytes_read += to_copy;
        offset += to_copy;
    }

    // Parse entries
    uint32_t entries_read = 0;
    for (uint32_t i = 0; i < entry_count; ++i) {
        struct FsEntry* e = (struct FsEntry*)(table_buf + i * ENTRY_SIZE);
        if (e->name[0] != 0 && e->name[0] != '?') {
            out_entries[entries_read++] = *e;
        }
    }
    *out_count = entries_read;
    return 0;
}
