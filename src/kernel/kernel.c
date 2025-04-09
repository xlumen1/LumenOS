#include "libk/vga/vga.h"
#include "libk/util/util.h"
#include "libk/buffer/buffer.h"
#include "gdt/gdt.h"
#include "libk/time/time.h"

#define DEFCOL VGA_COLOR(VGA_WHITE, VGA_BLUE)
#define HEADERCOL VGA_COLOR(VGA_BLUE, VGA_LIGHT_GREY)

#define KERNEL_VERSION "v0.0.3-prototype"

void kmain()
{
    vga_init();
    vga_clear(DEFCOL);

    vga_print("=================================[Lumen Kernel]=================================", HEADERCOL);
    vga_pos(0, 1);
    
    unsigned char gdt[24];
    gdt_encode(&gdt[0], (struct GDT){0, 0, 0, 0}); // Null descriptor
    gdt_encode(&gdt[8], (struct GDT){0, 0xFFFFF, 0x9A, 0xC}); // Code segment
    gdt_encode(&gdt[16], (struct GDT){0, 0xFFFFF, 0x92, 0xC}); // Data segment

    struct GDTR gdtr = {sizeof(gdt) - 1, (unsigned int)&gdt};
    __asm__ volatile("lgdt (%0)" : : "r"(&gdtr));

    __asm__ volatile(
        "mov $0x10, %ax\n"
        "mov %ax, %ds\n"
        "mov %ax, %es\n"
        "mov %ax, %fs\n"
        "mov %ax, %gs\n"
        "mov %ax, %ss\n"
        "ljmp $0x08, $.1\n"
        ".1:"
    );

    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
    while (1)
    {
        struct time t = get_time();
        vga_pos(4, 0);
        if (t.month < 10)
            vga_put('0', HEADERCOL);
        vga_print(itoa(t.month, 10), HEADERCOL);
        vga_put('/', HEADERCOL);
        if (t.day < 10)
            vga_put('0', HEADERCOL);
        vga_print(itoa(t.day, 10), HEADERCOL);
        vga_put('/', HEADERCOL);
        if (t.century < 10)
            vga_put('0', HEADERCOL);
        vga_print(itoa(t.century, 10), HEADERCOL);
        if (t.year < 10)
            vga_put('0', HEADERCOL);
        vga_print(itoa(t.year, 10), HEADERCOL);
        vga_pos(21, 0);
        if (t.hour < 10) {
            vga_put('0', HEADERCOL);
            if (t.hour == 0)
                vga_put('0', HEADERCOL);
        }
        vga_print(itoa(t.hour, 10), HEADERCOL);
        vga_put(':', HEADERCOL);
        if (t.minute < 10) {
            vga_put('0', HEADERCOL);
            if (t.minute == 0)
                vga_put('0', HEADERCOL);
        }
        vga_print(itoa(t.minute, 10), HEADERCOL);
        vga_put(':', HEADERCOL);
        if (t.second < 10) {
            vga_put('0', HEADERCOL);
            if (t.second == 0)
                vga_put('0', HEADERCOL);
        }
        vga_print(itoa(t.second, 10), HEADERCOL);
    }
}