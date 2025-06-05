#include "multiboot.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int multiboot_find_fsimg(multiboot_info_t* mb_info, void** out_addr, uint32_t* out_size) {
    if (!mb_info || !(mb_info->flags & (1 << 3)) || mb_info->mods_count == 0)
        return 0;
    multiboot_module_t* mods = (multiboot_module_t*)(uintptr_t)mb_info->mods_addr;
    for (uint32_t i = 0; i < mb_info->mods_count; ++i) {
        const char* mod_name = (const char*)(uintptr_t)mods[i].string;
        // Debug: print all module names
        printf("Module %d name: %s\n", i, mod_name ? mod_name : "(null)");
        // Accept if string contains fs.img, or if string is empty and only one module
        if ((mod_name && (strstr(mod_name, "fs.img") || strstr(mod_name, "/boot/fs.img"))) ||
            (!mod_name || !mod_name[0])) {
            *out_addr = (void*)(uintptr_t)mods[i].mod_start;
            *out_size = mods[i].mod_end - mods[i].mod_start;
            return 1;
        }
    }
    return 0;
}
