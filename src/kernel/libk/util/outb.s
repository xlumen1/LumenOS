.global outb
.type outb, %function

outb:
    mov %di, %dx
    out %al, %dx
    ret