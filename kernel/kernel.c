#include "types.h"
#include "io_ports.h"

#include "math.h"

const char* test = "Hello world, welcome to myOS!\0";

u8 *const VIDEO_BUFFER = (u8*) 0xb8000;

void clear() {
    u8* video = VIDEO_BUFFER;
    for(int i = 0; i < 80*25; i++) {
        *video = '\0';
        video ++;
        *video = 0xf0;
        video ++;
    }
}

void draw(u32 x, u32 y, u8 cell, u8 col) {
    u8* video = VIDEO_BUFFER;
    video[(x+y*80)*2]=cell;
    video[(x+y*80)*2+1]=col;
}

void write(const char* str) {
    u8* video = VIDEO_BUFFER;
    for(int i = 0; i < 30; i++) {
        video[i*2] = str[i];
    }
}

void setCursor(u32 x, u32 y) {
    u16 i = (u16)(x % 80 + y * 80);
    port_write_byte(0x3D4, 14);
    port_write_byte(0x3D5, (u8)(i >> 8));
    port_write_byte(0x3D4, 15);
    port_write_byte(0x3D5, (u8)(i & 0xff));
}

int fill_len = 9;
const u8* fill = "@#%*O*+X?";

void main() {
    clear();

    write(test);

    double x = 0.0f, y = 3.0f;
    double xo = 0.0f, yo = 10.0f;
    double r = 5.0;
    double t = 0.0f;
    u8 col = 0x1f;
    u8 charidx = 0;
    while(1) {
        t += 0.05f;
        if(t > 80) {
            t = 0;
            u8 c = col>>4;
            c = (c+1)%(0x10);
            col &= 0x0f;
            col |= c<<4;
            charidx = (charidx+1)%fill_len;
        }
        for(int j = 0; j < 25; j++) {
            for(int i = 0; i < 80; i++) {
                float dx = i-40, dy = j-(25/2);
                int length = (int)sqrt(dx*dx + dy*dy);
                if(length == (int)t) {
                    draw((u16)i,(u16)j,fill[charidx],col);
                }
            }
        }
    }
}