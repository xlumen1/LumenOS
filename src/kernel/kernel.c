#include <vga/vga.h>
#include <buffer/buffer.h>
#include <gdt/gdt.h>
#include <idt/idt.h>
#include <serial/serial.h>
#include <io/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf/elf.h>
#include <disk/disk.h>
#include "test_elf.h"

__attribute__((section(".exports")))
void (*kernel_exports[])(const char *, ...) = {
    printf
};

void kmain()
{
    vga_init();
    vga_clear(VGA_COLOR(VGA_WHITE, VGA_BLACK));
    vga_cursor_setmode(1);
    vga_pos(0, 0);

    gdt_install();
    idt_install();
    isr_install();

    disk_init();

    keyboard_init();
    serial_init(COM1);
    
    // I sure do love volatiles
    __asm__ volatile ("sti");

    serial_write("Kernel Loaded", COM1);

    uint8_t mbr[512];
    char* z = malloc(sizeof(int) * 4);
    int res = disk_read(0, mbr, 1);
    printf("disk_read returned: %d\n", res);
    itoa(res, z, 10);
    serial_write(z, COM1);
    if (res != 0) {
        while (1);
    }

    // elf_load((uint8_t*)test_elf, (size_t)test_elf_len);

    while (1) {
        
    }
}