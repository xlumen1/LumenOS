#ifndef FS_H
#define FS_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <serial/serial.h>

#define ENTRY_SIZE 49
#define ENTRIES_PER_SECTOR 9
#define NULL_ENTRY {"?", 0, 255, 0, 0, 0, 0}
#define MAX_ENTRIES 4294967295U

struct FsEntry {
    char name[32];
    uint32_t size;
    uint8_t isfile;
    uint32_t start_block; // Inclusive
    uint32_t end_block;   // Inclusive
    uint16_t start_byte;  // Inclusive
    uint16_t end_byte;    // Inclusive
} __attribute__((packed));

// MANAGMENT OPS

int lufs_format(); // Format the disc with lufs
int lufs_mount(); // Mount by loading data, verifying

// CRUD OPS

int lufs_create(const char* path, uint8_t isfile, char* data, uint32_t size); // Create a new entry
int lufs_read(struct FsEntry* file, uint8_t* data);  // Read entire file into memory
int lufs_update(struct FsEntry* file, uint8_t* data); // Write entire file
int lufs_delete(struct FsEntry* entry); // Delete entry and data (or child entries recursively)

// UTIL OPS

struct FsEntry lufs_frompath(const char* path); // Get entry by path
uint8_t lufs_isnull(struct FsEntry* entry); // Is an entry a null (?) entry
uint8_t lufs_isfile(struct FsEntry* entry); // Is an entry a file
struct FsEntry* lufs_children(struct FsEntry* directory); // Get all children of entry

void fs_use_memdisk(void* fsimg_addr, uint32_t fsimg_size);

#endif