#include <vga/vga.h>
#include <buffer/buffer.h>
#include <gdt/gdt.h>
#include <time/time.h>
#include <idt/idt.h>
#include <serial/serial.h>
#include <io/io.h>
#include <stdio.h>

void kmain()
{
    vga_init();
    vga_clear(VGA_COLOR(VGA_WHITE, VGA_BLACK));

    vga_cursor_setmode(1);
    vga_cursor_pos(0, 0);

    vga_pos(0, 0);

    gdt_install();
    idt_install();
    isr_install();

    keyboard_init();
    
    __asm__ volatile ("sti");

    serial_init(COM1);
    serial_write("Serial Debug Connection Established\n", COM1);
    
    while (1) {
        
    }
}