#pragma once

#include <uefi.h>

typedef struct mem_reservation {
    unsigned int                *addr;
    unsigned int                size;
} mem_reservation_t;

typedef struct boot_params {
    unsigned int                *framebuffer;
    unsigned int                width;
    unsigned int                height;
    unsigned int                pitch;
    mem_reservation_t           *mem_reservations;
    unsigned int                num_reservations;
    efi_runtime_services_t      *runtime_services;
    int                         argc;
    char                        **argv;
} boot_params_t;