#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "system.h"

#define VGA_320X200X256   1
#define VGA_TEXT80X25X16  2
#define VGA_640X480X16    3

typedef struct Bitmap_Head_{
  char Type[2];           //File type. Set to "BM".
  unsigned long Size;     //Size in BYTES of the file.
  unsigned long Reserved;      //Reserved. Set to zero.
  unsigned long OffSet;   //Offset to the data.
  unsigned long headsize; //Size of rest of header. Set to 40.
  unsigned long Width;     //Width of bitmap in pixels.
  unsigned long Height;     //  Height of bitmap in pixels.
  unsigned int  Planes;    //Number of Planes. Set to 1.
  unsigned int  BitsPerPixel;       //Number of Bits per pixels.
  unsigned long Compression;   //Compression. Usually set to 0.
  unsigned long SizeImage;  //Size in bytes of the bitmap.
  unsigned long XPixelsPreMeter;     //Horizontal pixels per meter.
  unsigned long YPixelsPreMeter;     //Vertical pixels per meter.
  unsigned long ColorsUsed;   //Number of colors used.
  unsigned long ColorsImportant;  //Number of "important" colors.
}Bitmap_Head;
typedef struct Bitmap_{
    struct Bitmap_Head_ header;
    unsigned char* img[];
}Bitmap;


void vga_setgmode(int mode);
void PutPixel(int x, int y, char color);
void set_text_mode(int hi_res);
void PutRect(int X, int Y, int Width, int Height, char color);
void PutBitmap(int x,int y,Bitmap bitmap);
void PutCercle(int X, int Y, int Radius, char color);

#endif