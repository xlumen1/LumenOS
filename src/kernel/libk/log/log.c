#include "log.h"

void log_info(const char* message) {
    vga_print(message, VGA_COLOR(VGA_WHITE, VGA_BLACK));
    vga_newline();
}

void log_error(const char* message) {
    vga_print(message, VGA_COLOR(VGA_RED, VGA_BLACK));
    vga_newline();
}