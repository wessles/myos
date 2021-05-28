#pragma once

#include "types.h"

/* VGA I/O port locations */
#define VGA_PORT_ADDRESS    0x3CE
#define VGA_PORT_DATA       0x3CF

/* VGA register addresses */
#define VGA_REG_SET_RESET           0x00
#define VGA_REG_ENABLE_SET_RESET    0x01
#define VGA_REG_COLOR_COMPARE       0x02
#define VGA_REG_DATA_ROTATE         0x03
#define VGA_REG_READ_MAP_SELECT     0x04
#define VGA_REG_GRAPHICS_MODE       0x05
#define VGA_REG_MISC_GRAPHICS       0x06
#define VGA_REG_COLOR_DONT_CARE     0x07
#define VGA_REG_BIT_MASK            0x08

void vga_init();