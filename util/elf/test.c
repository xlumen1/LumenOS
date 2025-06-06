#define KERNEL_EXPORTS_ADDR 0x300000
typedef void (*func_t)(const char *, ...);
void _start() {
    func_t* kernel_exports = (func_t*)KERNEL_EXPORTS_ADDR;
    __asm__ volatile ( // Exit Syscall
        "mov $1, %%eax\n\t"
        "int $0x80\n\t"
        :
        :
        : "%eax"
    );
}