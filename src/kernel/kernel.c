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
    
    // I sure do love volatiles
    __asm__ volatile ("sti");

    serial_write("Kernel Loaded", COM1);

    // Access fs.img loaded as a GRUB module
    void* fsimg_addr = NULL;
    uint32_t fsimg_size = 0;
    if (multiboot_find_fsimg(mb_info, &fsimg_addr, &fsimg_size)) {
        printf("Found fs image at %p, size %u bytes\n", fsimg_addr, fsimg_size);
        fs_use_memdisk(fsimg_addr, fsimg_size);
    } else {
        printf("Fs image not found as a GRUB module, assuming local install\n");
    }

    char* buffer = malloc(1024);
    if (!buffer) {
        printf("Failed to allocate memory for buffer\n");
        return;
    }

    struct FsEntry root = lufs_frompath("");

    printf("Root directory: %s, size: %u bytes, isfile: %d\n", root.name, root.size, root.isfile);
    struct FsEntry* children = lufs_children(&root);
    if (!children) {
        printf("Failed to get children of root directory\n");
    } else {
        printf("%s:\nSize: %u\nIsfile: %s\n", children[0].name, children[0].size, children[0].isfile ? "Yes" : "No");
        printf("Children of root directory:\n");
        for (int i = 0; children[i].name[0] != '\0'; ++i) {
            printf(" - %s, size: %u bytes, isfile: %d\n", children[i].name, children[i].size, children[i].isfile);
        }
    }
    serial_write("Test1", COM1);
    free(children);
    serial_write("Test2", COM1);
    struct FsEntry test_file = lufs_frompath("/test.txt");
    serial_write("Test3", COM1);
    if (lufs_isnull(&test_file)) {
        printf("File not found: doc/welcome.txt\n");
    } else {
        printf("Found file: %s, size: %u bytes, isfile: %d\n", test_file.name, test_file.size, test_file.isfile);
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