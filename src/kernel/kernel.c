#include "libk/vga/vga.h"
#include "libk/util/util.h"
#include "libk/buffer/buffer.h"

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
    vga_newline();
    vga_print("Something Something Startup Info...", DEFCOL);
    vga_newline();
    vga_print(KERNEL_VERSION, DEFCOL);
}