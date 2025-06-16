#include "../syscall.h"

extern void sys_exit(int32_t code) {
    // if (exit_return_handler) {
    //     exit_return_handler(code);
    // }
    while (1);
}