#include "../syscall.h"

#include <fs/fs.h>

void sys_read(uint32_t fd, char* buf, size_t count) {
    if (fd == 0) {
        for (size_t i = 0; i < count; i++) {
            buf[i] = keyboard_getc();
        }
    }

}