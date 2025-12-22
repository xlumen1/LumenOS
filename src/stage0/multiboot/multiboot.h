#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

typedef struct {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved;
} multiboot_module_t;

typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    // ...other fields not needed for modules...
} multiboot_info_t;

// Finds the fs.img module, returns pointer and size via out params, returns 1 if found, 0 if not
int multiboot_find_fsimg(multiboot_info_t* mb_info, void** out_addr, uint32_t* out_size);

#endif