.global isr0
isr0:
    cli     

    pusha

    pushl $0
    pushl $0

    mov %esp, %eax
    pushl %eax

    call isr_handler

    add $12, %esp

    popa

    sti
    iret
