#include "libk/vga/vga.h"
#include "libk/util/util.h"
#include "libk/buffer/buffer.h"
#include "gdt/gdt.h"

#define DEFCOL VGA_COLOR(VGA_WHITE, VGA_BLUE)
#define HEADERCOL VGA_COLOR(VGA_BLUE, VGA_WHITE)

#define KERNEL_VERSION "v0.0.2-prototype"

void kmain() {
    vga_init();
    vga_clear(DEFCOL);
    char* txt = "Lumen Kernel";
    for (unsigned char i = 0; i < (VGA_WIDTH / 2) + 5 - strlen(txt); i++)
        vga_put('=', HEADERCOL);
    vga_put('[', HEADERCOL);
    vga_print(txt, HEADERCOL);
    vga_put(']', HEADERCOL);
    for (unsigned char i = 0; i < (VGA_WIDTH / 2) + 5 - strlen(txt); i++)
        vga_put('=', HEADERCOL);
    vga_print(KERNEL_VERSION, DEFCOL);
    vga_newline();
    gdt_encode((unsigned char*)0x20, (struct GDT){0, 0, 0, 0});
    gdt_encode((unsigned char*)0x28, (struct GDT){0, 0xFFFFF, 0x9A, 0xC});
    gdt_encode((unsigned char*)0x30, (struct GDT){0, 0xFFFFF, 0x92, 0xC});
    __asm__ volatile ("lgdt (0x20)");
    outb(0x70, 0x08);
    vga_print(itoa(inb(0x71), 10), DEFCOL);
    vga_put('/', DEFCOL);
    outb(0x70, 0x07);
    vga_print(itoa(inb(0x71), 10), DEFCOL);
    vga_put('/', DEFCOL);
    outb(0x70, 0x09);
    vga_print(itoa(inb(0x71) - 12, 10), DEFCOL);
    vga_newline();
    outb(0x70, 0x04);
    vga_print(itoa(inb(0x71) - 24, 10), DEFCOL);
    vga_put(':', DEFCOL);
    outb(0x70, 0x02);
    vga_print(itoa(inb(0x71) - 30, 10), DEFCOL);
}