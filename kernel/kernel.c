#include <uefi.h>

#include <stdarg.h>

#include "boot_params.h"
#include "types.h"

#include "raster_font.h"

boot_params_t bparam;

int _start(boot_params_t *bparam_ptr) {
    bparam = *bparam_ptr;

    u32* video = bparam.framebuffer;
    
    while(true) {
        for(int i = 0; i < bparam.width*bparam.height; i++) {
            video[i] = 0xffffff;
        }

        char test[] = "Welcome to MyOS";
        print(1,1,test);

        for(int i = 0; i < 10000000; i++);
    }
    return 0;
}