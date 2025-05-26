#include <vga/vga.h>
#include <util/util.h>
#include <buffer/buffer.h>
#include <gdt/gdt.h>
#include <time/time.h>
#include <log/log.h>
#include <idt/idt.h>
#include <serial/serial.h>
#include <io/io.h>

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

    keyboard_init();
    
    __asm__ volatile ("sti");

    serial_init();
    serial_write("Serial Debug Connection Established\n");
    while (1) {

    }
}