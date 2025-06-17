#include "../syscall.h"
#include <fs/fs.h>
#include <stdio.h>

int sys_write(uint32_t fd, const char* buf, size_t count) {
    printf("@sys.write:call\n");
    if (fd == 1 || fd == 2) { // Stdout / Stderr
        printf("%s", buf);
    }
    printf("@sys.write:return\n");
    return count;
}