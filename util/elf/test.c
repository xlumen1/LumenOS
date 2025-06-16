void _start() {
    __asm__ volatile ( // Debug Syscall
        "mov $255, %%eax\n\t"
        "int $0x80\n\t"
        :
        :
        : "%eax"
    );
}