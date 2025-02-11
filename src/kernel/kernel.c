#include "libk/vga/vga.h"
#include "libk/util/util.h"
#include "libk/buffer/buffer.h"
#include "gdt/gdt.h"
#include "libk/time/time.h"

#define DEFCOL VGA_COLOR(VGA_WHITE, VGA_BLUE)
#define HEADERCOL VGA_COLOR(VGA_BLUE, VGA_WHITE)

#define KERNEL_VERSION "v0.0.3-prototype"

void kmain()
{
    vga_init();
    vga_clear(DEFCOL);
    char *txt = "Lumen Kernel";
    for (unsigned char i = 0; i < (VGA_WIDTH / 2) + 5 - strlen(txt); i++)
        vga_put('=', HEADERCOL);
    vga_put('[', HEADERCOL);
    vga_print(txt, HEADERCOL);
    vga_put(']', HEADERCOL);
    for (unsigned char i = 0; i < (VGA_WIDTH / 2) + 5 - strlen(txt); i++)
        vga_put('=', HEADERCOL);
    vga_print(KERNEL_VERSION, DEFCOL);
    vga_pos(0, 4);
    gdt_encode((unsigned char *)0x20, (struct GDT){0, 0, 0, 0});
    gdt_encode((unsigned char *)0x28, (struct GDT){0, 0xFFFFF, 0x9A, 0xC});
    gdt_encode((unsigned char *)0x30, (struct GDT){0, 0xFFFFF, 0x92, 0xC});

    struct GDTR gdtr = {0x17, 0x20};

    __asm__ volatile("lgdt (%0)" : : "r"(&gdtr));
    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
    while (1)
    {
        struct time t = get_time();
        vga_pos(0, 2);
        if (t.month < 10)
            vga_put('0', DEFCOL);
        vga_print(itoa(t.month, 10), DEFCOL);
        vga_put('/', DEFCOL);
        if (t.day < 10)
            vga_put('0', DEFCOL);
        vga_print(itoa(t.day, 10), DEFCOL);
        vga_put('/', DEFCOL);
        if (t.century < 10)
            vga_put('0', DEFCOL);
        vga_print(itoa(t.century, 10), DEFCOL);
        if (t.year < 10)
            vga_put('0', DEFCOL);
        vga_print(itoa(t.year, 10), DEFCOL);
        vga_newline();
        if (t.hour - 5 < 10) {
            vga_put('0', DEFCOL);
            if (t.hour == 0)
                vga_put('0', DEFCOL);
        }
        vga_print(itoa(t.hour - 5, 10), DEFCOL);
        vga_put(':', DEFCOL);
        if (t.minute < 10) {
            vga_put('0', DEFCOL);
            if (t.minute == 0)
                vga_put('0', DEFCOL);
        }
        vga_print(itoa(t.minute, 10), DEFCOL);
        vga_put(':', DEFCOL);
        if (t.second < 10) {
            vga_put('0', DEFCOL);
            if (t.second == 0)
                vga_put('0', DEFCOL);
        }
        vga_print(itoa(t.second, 10), DEFCOL);
    }
}