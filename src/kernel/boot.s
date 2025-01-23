/* MB HEADER */
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + 3)

.section .multiboot
.align 4
.long MAGIC
.long 3
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384 # Stack is 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
mov $stack_top, %esp

call kmain

cli
hlt