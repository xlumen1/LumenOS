#ifndef SYSCALL_H
#define SYSCALL_H

#include <idt/isr.h>
#include <stdarg.h>
#include <stddef.h>

extern uint8_t** fd_list;
extern uint8_t fd_count;

void syscall_init();

void syscall_dispatch(isr_regs_t regs);

void sys_exit(int32_t code);
void sys_read(uint32_t fd, char* buf, size_t count);
void sys_write(uint32_t fd, const char* buf, size_t count);
void sys_open(const char* filename, int32_t flags, uint32_t mode);
void sys_close(uint32_t fd);

#endif