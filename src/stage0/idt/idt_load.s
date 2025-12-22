.global idt_load
idt_load:
    lidt idtp
    ret
