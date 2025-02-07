#include "libk/vga/vga.h"
#include "libk/util/util.h"

#define DEFCOL VGA_COLOR(VGA_WHITE, VGA_BLUE)
#define HEADERCOL VGA_COLOR(VGA_BLUE, VGA_WHITE)

void kmain() {
    vga_clear(DEFCOL);
    char* txt = "Lumen Kernel";
    for (unsigned char i = 0; i < (VGA_WIDTH / 2) + 5 - strlen(txt); i++)
        vga_put('=', HEADERCOL);
    vga_put('[', HEADERCOL);
    vga_print(txt, HEADERCOL);
    vga_put(']', HEADERCOL);
    for (unsigned char i = 0; i < (VGA_WIDTH / 2) + 5 - strlen(txt); i++)
        vga_put('=', HEADERCOL);
    vga_row = 1;
    vga_col = 0;
    vga_print("Something Something Startup Info...", DEFCOL);
}