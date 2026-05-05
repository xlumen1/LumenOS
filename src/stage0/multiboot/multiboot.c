#include "multiboot.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int multiboot_find_module(multiboot_info_t* mb_info, void** out_addr, uint32_t* out_size, const char* name) {
    if (!mb_info || !(mb_info->flags & (1 << 3)) || mb_info->mods_count == 0)
        return 0;
    multiboot_module_t* mods = (multiboot_module_t*)(uintptr_t)mb_info->mods_addr;
    for (uint32_t i = 0; i < mb_info->mods_count; ++i) {
        const char* mod_name = (const char*)(uintptr_t)mods[i].string;
        if (mod_name && strstr(mod_name, name)) {
            *out_addr = (void*)(uintptr_t)mods[i].mod_start;
            *out_size = mods[i].mod_end - mods[i].mod_start;
            return 1;
        }
    }
    return 0;
}
