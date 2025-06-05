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
#include <multiboot/multiboot.h>
#include <fs/fs.h>
#include <lsc/lsc.h>

__attribute__((section(".exports")))
void (*kernel_exports[])(const char *, ...) = {
    printf
};

void kmain(uint32_t multiboot_magic, multiboot_info_t* mb_info)
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

    lsc_init();
    
    // I sure do love volatiles
    __asm__ volatile ("sti");

    serial_write("Kernel Loaded\n", COM1);

    // Access fs.img loaded as a GRUB module
    void* fsimg_addr = NULL;
    uint32_t fsimg_size = 0;
    if (multiboot_find_fsimg(mb_info, &fsimg_addr, &fsimg_size)) {
        printf("Found fs image at %p, size %x bytes\n", fsimg_addr, fsimg_size);
        fs_use_memdisk(fsimg_addr, fsimg_size);
    } else {
        printf("Fs image not found as a GRUB module, assuming local install\n");
    }

    char* buffer = malloc(1024);
    if (!buffer) {
        printf("Failed to allocate memory for buffer\n");
        return;
    }
    serial_write("Test code update\n", COM1);
    serial_write("Test1\n", COM1);
    struct FsEntry test_file = lufs_frompath("/doc/welcome.txt");
    serial_write("Test2\n", COM1);
    if (lufs_isnull(&test_file)) {
        printf("File not found: /doc/welcome.txt\n");
    } else {
        printf("Found file: %s, size: %u bytes, isfile: %d\n", test_file.name, test_file.size, test_file.isfile);
        printf("struct FsEntry {\n  name = %s;\n  size = %u;\n  isfile = %u;\n  start_block = %u;\n  end_block = %u;\n  start_byte = %x;\n  end_byte = %x;\n} __attribute__ ((packed))\n", test_file.name, test_file.size, test_file.isfile, test_file.start_block, test_file.end_block, test_file.start_byte, test_file.end_byte);
        if (lufs_isfile(&test_file)) {
            if (lufs_read(&test_file, (uint8_t*)buffer) == 0) {
                printf("File content:\n%s\n", buffer);
            } else {
                printf("Failed to read file content\n");
            }
        }
    }
    free(buffer);
    
    while (1) {
        
    }
}