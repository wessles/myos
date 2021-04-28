#pragma once

#include "types.h"

static inline u8    port_read_byte  (u16 portno) {
    u8 returned;
    asm(
        "in %%dx, %%al"
        : // out
            "=a" (returned)
        : // in
            "d" (portno)
    );
    return returned;
}

static inline void  port_write_byte (u16 portno, u8 byte) {
    asm(
        "out %%al, %%dx"
        : // out
        : // in
            "a" (byte),
            "d" (portno)
    );
}

static inline u16   port_read_word  (u16 portno) {
    u16 returned;
    asm(
        "in %%dx, %%ax"
        : // out
            "=a" (returned)
        : // in
            "d" (portno)
    );
    return returned;
}

static inline void  port_write_word (u16 portno, u16 word) {
    asm(
        "out %%ax, %%dx"
        : // out
        : // in
            "a" (word),
            "d" (portno)
    );
}