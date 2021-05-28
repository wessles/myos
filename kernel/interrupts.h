#pragma once

#include "types.h"

#define IDT_SIZE 256

/*
Entry in the interrupt descriptor table, used to direct
the CPU to interrupt service routines.
*/
typedef struct idt_entry {
    u16     offset_lo;
    u16     selector;
    u8      zero;
    u8      type_attr;
    u16     offset_hi;
} idt_entry_t;

struct idt_entry idt[IDT_SIZE];