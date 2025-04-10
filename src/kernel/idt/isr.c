#include "idt.h"
#include <log/log.h>

void isr_handler() {
    log_error("Interrupt received!");
}