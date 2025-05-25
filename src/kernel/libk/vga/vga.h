#include "../util/util.h"
#include "../buffer/buffer.h"

#ifndef VGA_H
#define VGA_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS (short*)0xB8000

#define VGA_COLOR(fg, bg) ((unsigned char)(fg | bg << 4))

#define VGA_CHAR(a, color) ((short) a | (short) color << 8)

// COLORS

#define VGA_BLACK 0
#define VGA_BLUE 1
#define VGA_GREEN 2
#define VGA_CYAN 3
#define VGA_RED 4
#define VGA_MAGENTA 5
#define VGA_BROWN 6
#define VGA_LIGHT_GREY 7
#define VGA_DARK_GREY 8
#define VGA_LIGHT_BLUE 9
#define VGA_LIGHT_GREEN 10
#define VGA_LIGHT_CYAN 11
#define VGA_LIGHT_RED 12
#define VGA_LIGHT_MAGENTA 13
#define VGA_LIGHT_BROWN 14
#define VGA_WHITE 15

// VARIABLES

extern tbuffer_t vga_buffer;
extern short vga_row;
extern short vga_col;

// UTIL FUNCTIONS

void vga_init();
void vga_pos(unsigned char col, unsigned char row);
void vga_newline();
void vga_clear(unsigned char color);

// TEXT FUNCTIONS

void vga_set(char a, unsigned char color, unsigned char col, unsigned char row);
void vga_put(char a, unsigned char color);
void vga_write(const char* a, unsigned char color, unsigned short length);
void vga_print(const char* a, unsigned char color);

#endif
