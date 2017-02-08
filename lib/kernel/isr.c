#include <kernel/kernel.h>
#include <serial.h>
#include "ISR.h"

void div_by_zero_ISR(void) {
	serial_writestring("div by zero\n");
	abort("div by zero\n");
}