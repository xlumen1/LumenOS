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

    uint8_t mbr[512];

    disk_read(0, mbr, 1);

    for (int i = 0; i < 512; i++) {
        printf("%x ", mbr[i]);
    }

    // elf_load((uint8_t*)test_elf, (size_t)test_elf_len);

    while (1) {
        
    }
}