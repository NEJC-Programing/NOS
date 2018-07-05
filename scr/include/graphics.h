#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "system.h"

#define VGA_320X200X256   1
#define VGA_TEXT80X25X16  2
#define VGA_640X480X16    3

typedef struct Bitmap{
    int ColorDepth;
    int Height;
    int Width;
    char * Pixels;
}Bitmap;

void vga_setgmode(int mode);
void set_plane(unsigned p);
void PutPixel(int x, int y, char color);
void set_text_mode(int hi_res);
void clearScreen_vga();
void PutRect(int X, int Y, int Width, int Height, char color);
void PutBitmap(int x,int y,Bitmap bitmap);


#endif