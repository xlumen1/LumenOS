.global isr0
isr0:
    cli                         
    pushl $0
    pushl $0

    pusha

    mov %ds, %ax
    pushl %eax
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    call isr_handler

    popl %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    popa
    add $8, %esp

    sti
    iret
