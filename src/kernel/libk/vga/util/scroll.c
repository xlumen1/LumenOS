#include "../vga.h"

#include <string.h>

void vga_scroll(uint8_t lines) {
    serial_write("Scroll\n", COM1);

    if (lines >= VGA_HEIGHT) {
        for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
            VGA_ADDRESS[i] = ' ' | (0x07 << 8);
        return;
    }

    size_t chars_to_copy = (VGA_HEIGHT - lines) * VGA_WIDTH;
    memmove(
        VGA_ADDRESS,
        VGA_ADDRESS + (lines * VGA_WIDTH),
        chars_to_copy * sizeof(uint16_t)
    );

    // for (int row = VGA_HEIGHT - lines; row < VGA_HEIGHT; row++) {
    //     for (int col = 0; col < VGA_WIDTH; col++) {
    //         VGA_ADDRESS[row * VGA_WIDTH + col] = ' ' | (0x07 << 8);
    //     }
    // }
}
