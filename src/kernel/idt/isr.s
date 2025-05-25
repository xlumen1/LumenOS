.macro ISR_NOERR num
.global isr\num
isr\num:
    cli
    pusha
    pushl $0 # Push dummy error code
    pushl $\num
    call isr_handler
    add $8, %esp # Clean up the stack
    popa
    sti
    iret
.endm

.macro ISR_ERR num
.global isr\num
isr\num:
    cli
    pusha
    pushl $\num # Interrupt number
    call isr_handler
    add $4, %esp
    popa
    sti
    iret
.endm

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8  # Double fault
ISR_NOERR 9
ISR_ERR   10 # Invalid TSS
ISR_ERR   11 # Segment not present
ISR_ERR   12 # Stack-segment fault
ISR_ERR   13 # General protection fault
ISR_ERR   14 # Page fault
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17 # Alignment check
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31
