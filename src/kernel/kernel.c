#include <vga/vga.h>
#include <util/util.h>
#include <buffer/buffer.h>
#include "gdt/gdt.h"
#include <time/time.h>
#include <log/log.h>
#include "idt/idt.h"

#define KERNEL_VERSION "v0.0.3-prototype"

void kmain()
{
    vga_init();
    vga_clear(VGA_COLOR(VGA_WHITE, VGA_BLACK));

    vga_pos(0, 0);

    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);

    gdt_install();
    log_info("GDT loaded");

    idt_install();
    isr_install();
    log_info("IDT loaded");

    __asm__ volatile ("sti");


    while (1)
    {
        
    }
}