#include "../syscall.h"
#include <stdio.h>

extern void sys_dbg() {
    printf("[SYS] Syscall Test\n");
}