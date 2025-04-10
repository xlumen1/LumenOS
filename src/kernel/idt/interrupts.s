.global isr_stub_table

.section .text
isr_stub_table:
    .rept 32
    .long isr_stub
    .endr

isr_stub:
    pusha
    call isr_handler
    popa
    iret