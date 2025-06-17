extern int main(int argc, char** argv);

void _exit(int code) {
    __asm__ volatile ( // Exit Syscall
        "mov $1, %%eax\n\t"
        "mov %0, %%ebx\n\t"
        "int $0x80\n\t"
        :
        : "r"(code)
        : "%eax", "%ebx"
    );
}

void _start() {
    int argc;
    char** argv;

    __asm__ volatile (
        "mov (%%esp), %0\n\t"        // argc
        "lea 4(%%esp), %1\n\t"       // argv = esp + 4
        : "=r"(argc), "=r"(argv)
        :
        : "memory"
    );

    _exit(main(argc, argv));
}
