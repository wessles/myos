#pragma once

#include "types.h"
#include "io_ports.h"

/*
This is the maximum frequency
the timer frequency is the result of dividing this number
*/
#define PIT_CLOCK_HZ       1193180;

#define PIT_PORT_CHANNEL_0 0x40
#define PIT_PORT_CHANNEL_1 0x41
#define PIT_PORT_CHANNEL_2 0x42
#define PIT_REG_MODE_CMD   0x43

/*
Initialize a square wave timer at maximum frequency (divisor=1)
*/
void pit_timer_init() {
    u16 divisor = 1;
    port_write_byte(PIT_REG_MODE_CMD, 0x36);
    port_write_byte(PIT_PORT_CHANNEL_0, divisor & 0xff);
    port_write_byte(PIT_PORT_CHANNEL_0, divisor >> 8);
}

