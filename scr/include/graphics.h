#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "system.h"

#define VGA_320X200X256   1
#define VGA_TEXT80X25X16  2
#define VGA_640X480X16    3


void vga_setgmode(int mode);
void set_plane(unsigned p);
void putpixel(int x, int y, char color);
void set_text_mode(int hi_res);
#endif