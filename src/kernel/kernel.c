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

char* read_file(const char* path)
{
    struct FsEntry entry = lufs_frompath(path);
    if (lufs_isnull(&entry)) {
        return NULL;
    }

    if (!lufs_isfile(&entry)) {
        return NULL;
    }

    char* buffer = malloc(entry.size + 1);
    if (!buffer) {
        return NULL;
    }

    if (lufs_read(&entry, (uint8_t*)buffer) != 0) {
        free(buffer);
        return NULL;
    }

    buffer[entry.size] = '\0';
    return buffer;
}

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

    // lufs_create("/create_test.txt", 1, "Hello, World!", 13);
    // printf("Created file /create_test.txt\n");

    printf("%s", read_file("/doc/welcome.txt"));
    
    free(buffer);
    
    while (1) {
        
    }
}