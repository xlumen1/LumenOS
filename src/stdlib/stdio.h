#ifndef STDIO_H
#define STDIO_H

void itoa(int value, char* str, int base);
void printf(const char* format, ...);
int sprintf(char* str, const char* format, ...);

#endif