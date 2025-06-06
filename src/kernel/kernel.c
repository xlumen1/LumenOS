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

    serial_write("[KERNEL] Kernel Loaded\n", COM1);

    // Access fs.img loaded as a GRUB module
    void* fsimg_addr = NULL;
    uint32_t fsimg_size = 0;
    if (multiboot_find_fsimg(mb_info, &fsimg_addr, &fsimg_size)) {
        printf("[KERNEL] Found fs image at %p, size %x bytes\n", fsimg_addr, fsimg_size);
        fs_use_memdisk(fsimg_addr, fsimg_size);
    } else {
        printf("[KERNEL] Fs image not found as a GRUB module, assuming local install\n");
    }

    // lufs_create("/create_test.txt", 1, "Hello, World!", 13);
    // printf("Created file /create_test.txt\n");

    printf("%s", read_file("/doc/welcome.txt"));

    printf("[KERNEL] Kernel_exports address: %p\n", kernel_exports);

    // Load and execute an ELF file
    struct FsEntry elf_entry = lufs_frompath("/bin/test.elf");
    uint8_t* elf_data = malloc(elf_entry.size);
    if (lufs_read(&elf_entry, elf_data) == 0) {
        printf("[KERNEL] Loaded ELF file %s, size %d bytes\n", elf_entry.name, elf_entry.size);
        if (elf_load(elf_data, elf_entry.size) == 0) {
            printf("[KERNEL] ELF loaded successfully!\n");
        } else {
            printf("[KERNEL] Failed to load ELF file.\n");
        }
    } else {
        printf("[KERNEL] Failed to read ELF file.\n");
    }
    free(elf_data);

    while (1) {
        
    }
}