#include "libk/vga/vga.h"
#include "libk/util/util.h"

#define DEFCOL VGA_COLOR(VGA_WHITE, VGA_BLACK)

void kmain() {
    vga_clear();
    vga_print("Lumen OS", DEFCOL);
    vga_col = 0;
    vga_row = 1;
    vga_print(itoa(42, 10), DEFCOL);
}
