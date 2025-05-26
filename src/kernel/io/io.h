#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stdio.h>
#include <idt/isr.h>

void keyboard_handler(isr_regs_t* regs);
void keyboard_init();

#endif