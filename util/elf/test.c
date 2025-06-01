#define KERNEL_EXPORTS_ADDR 0x300000
typedef void (*func_t)(const char *, ...);
void _start() {
    func_t* kernel_exports = (func_t*)KERNEL_EXPORTS_ADDR;
    //kernel_exports[0]("Hello from ELF!\n");
    while (1) {}
}