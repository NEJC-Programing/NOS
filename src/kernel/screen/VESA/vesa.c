#include "vesa.h"
#include <libk.h>
#include <bios32.h>
////////////////////////////////////////////////////////////////////////////////
///////////////http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html///////////
////////////////////////////////////////////////////////////////////////////////

/*
void GoGraphics()
{
  struct VbeInfoBlock *vib = malloc(512);
  regs16_t *regs = malloc(sizeof(regs16_t));
  print("malloc\n");
  //regs->ax = 0x4f00; //get Info
  regs->ax = 0x4F02; // set mode
  regs->bx = 0x0112; //vido mode
  //regs.es;
  //regs.di;
  printf("%s\n", "regs");
  biosint32(0x10, regs);
  printf("int\n" );
  if (regs->ax!=0x004f) {printf("%s\n", "error\n\n\n");}
}

void set_vesa_bank(int bank_number)
   {
      regs16_t *r;

      r->ax = 0x4F05;
      r->bx = 0;
      r->dx = bank_number;
      biosint32(0x10, r);
   }
   void putpixel_vesa_640x480(int x, int y, int color)
   {
      int address = y*640+x;
      int bank_size = mode_info.WinGranularity*1024;
      int bank_number = address/bank_size;
      int bank_offset = address%bank_size;

      set_vesa_bank(bank_number);

      _farpokeb(_dos_ds, 0xA0000+bank_offset, color);
   }
/*
uint16_t findMode(int x, int y, int d)
{
  struct VbeInfoBlock *ctrl = (VbeInfoBlock *)0x2000;
  struct ModeInfoBlock *inf = (ModeInfoBlock *)0x3000;
  uint16_t *modes;
  int i;
  uint16_t best = 0x13;
  int pixdiff, bestpixdiff = DIFF(320 * 200, x * y);
  int depthdiff, bestdepthdiff = 8 >= d ? 8 - d : (d - 8) * 2;

  strncpy(ctrl->VbeSignature, "VBE2", 4);
  intV86(0x10, "ax,es:di", 0x4F00, 0, ctrl); // Get Controller Info
  if ( (uint16_t)v86.tss.eax != 0x004F ) return best;

  modes = (uint16_t*)REALPTR(ctrl->VideoModePtr);
  for ( i = 0 ; modes[i] != 0xFFFF ; ++i ) {
      intV86(0x10, "ax,cx,es:di", 0x4F01, modes[i], 0, inf); // Get Mode Info

      if ( (uint16_t)v86.tss.eax != 0x004F ) continue;

      // Check if this is a graphics mode with linear frame buffer support
      if ( (inf->attributes & 0x90) != 0x90 ) continue;

      // Check if this is a packed pixel or direct color mode
      if ( inf->memory_model != 4 && inf->memory_model != 6 ) continue;

      // Check if this is exactly the mode we're looking for
      if ( x == inf->XResolution && y == inf->YResolution &&
          d == inf->BitsPerPixel ) return modes[i];

      // Otherwise, compare to the closest match so far, remember if best
      pixdiff = DIFF(inf->Xres * inf->Yres, x * y);
      depthdiff = (inf->bpp >= d)? inf->bpp - d : (d - inf->bpp) * 2;
      if ( bestpixdiff > pixdiff ||
          (bestpixdiff == pixdiff && bestdepthdiff > depthdiff) ) {
        best = modes[i];
        bestpixdiff = pixdiff;
        bestdepthdiff = depthdiff;
      }
  }
  if ( x == 640 && y == 480 && d == 1 ) return 0x11;
  return best;
}
//*/
