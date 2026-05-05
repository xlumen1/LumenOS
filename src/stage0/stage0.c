#include <vga/vga.h>
#include <buffer/buffer.h>
#include <gdt/gdt.h>
#include <idt/idt.h>
#include <serial/serial.h>
#include <io/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <disk/disk.h>
#include <multiboot/multiboot.h>
#include <fat/fat32.h>

#define panic(e) printf("[STAGE0] %s\n[STAGE0] KERNEL PANIC!", e); return;

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
    
    // I sure do love volatiles
    // Haha GCC goes BRRRRRRRRR
    __asm__ volatile ("sti");

    serial_write("[STAGE0] Stage 0 Initialized\n", COM1);
    if (multiboot_magic != 0x2BADB002) {
        panic("Multiboot Magic Invalid!");
    }

    // Access fs.img loaded as a GRUB module
    void* fsimg_addr = NULL;
    uint32_t fsimg_size = 0;
    if (multiboot_find_module(mb_info, &fsimg_addr, &fsimg_size, "")) {
        printf("[STAGE0] Found fs image at %p, size %x bytes\n", fsimg_addr, fsimg_size);
        fs_use_memdisk(fsimg_addr, fsimg_size);
    } else {
        panic("Fs image not found as a GRUB module");
    }

    fat32_fs_t* fs;
    fat32_file_t root;
    fat32_file_t file;  

    // Mount FAT32
    if (fat32_mount(0, &fs) != 0) {
        panic("FAT mount failed");
    }
    
    if (fat32_open_root(fs, &root) != 0) {
        panic("Open root failed");
    }
    
    // 8.3 uppercase & space padded
    if (fat32_find(fs, &root, "STAGE1  BIN", &file) != 0) {
        panic("File not found");
    }
        

    while (1) {
        keyboard_handler();
        char c = keyboard_getc();
        if (c != '\0') {
            printf("%c", c);
        }
    }
}
