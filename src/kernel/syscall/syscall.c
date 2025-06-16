#include "syscall.h"

#include <stdlib.h>

uint8_t** fd_list;
uint8_t fd_count;

static void (*exit_return_handler)(int32_t code) = 0;

void syscall_init() {
    fd_list = malloc(255);
    fd_count = 0;
}

void syscall_set_exit_handler(void (*handler)(int32_t)) {
    exit_return_handler = handler;
}

void syscall_dispatch(isr_regs_t regs) {
    switch (regs.eax)
    {
    case 1: // SYS_EXIT
        sys_exit((int32_t)regs.ebx);
        break;
    
    case 3:
        sys_read((uint32_t)regs.ebx, (char*)regs.ecx, (size_t)regs.edx);
        break;
    
    case 255:
        sys_dbg();
        break;
    
    default:
        break;
    }
}
