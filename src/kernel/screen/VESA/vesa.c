#include "vesa.h"
#include <libk.h>
#include <bios32.h>
#include <ports.h>
////////////////////////////////////////////////////////////////////////////////
///////////////http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html///////////
////////////////////////////////////////////////////////////////////////////////

VESA_INFO vesa_info;
MODE_INFO mode_info;

int GoGraphics()
{
  if(get_vesa_info() !=0)
  {
    printf("%s\n", "Error ");
    return -1;
  }
  print("Got Info\n");

  if(set_vesa_mode(0x13)!=0){
    print("Error\n");
    return -1;
  }
  //hlt();
  for (size_t i = 0; i < 50; i++) {
    for (size_t j = 0; j < 100; j++) {
      putpixel_vesa_640x480(i,j,10);
    }
  }//*/
   
}

int get_vesa_info()
   {
      regs16_t r;
      int c;

      VESA_INFO *buff = 0x1FFF;//malloc(sizeof(VESA_INFO));
      memset(buff, 0, sizeof(VESA_INFO));
      memcpy(buff, "VBE2", 4);
      //print(buff->VESASignature);

      /* call the VESA function */
      r.ax = 0x4F00;
      r.di = (int)buff & 0xf;
      r.es = (int)((int)buff)>>4 & 0xFFFF;
      //r.es = (buff>>4) &
      //print("\nregs\n");
      //hlt();
      biosint32(0x10, &r);
      //print("int \n");
      if (r.ax!=0x004f){return -1;}
      /* copy the resulting data into our structure */
      memcpy(&vesa_info, buff, sizeof(VESA_INFO));
      /* check that we got the right magic marker value */
      //printf("%s\n",vesa_info.VESASignature);
      if (strncmp(vesa_info.VESASignature, "VESA"/*VESA ??*/, 4) != 0) return -1;

      //printf("VideoModePtr = %d\n", vesa_info.VideoModePtr);
      /* it worked! */
      return 0;
   }

   int get_mode_info(int mode)
   {
      regs16_t r;
      long * buff = 0x1FFF;
      int c;


      /* initialize the buffer to zero */
      memset(buff, 0, sizeof(VESA_INFO));

      /* call the VESA function */
      r.ax = 0x4F01;
      r.di = (long)buff & 0xF;
      r.es = ((long)buff>>4) & 0xFFFF;
      r.cx = mode;
      biosint32(0x10, &r);

      /* quit if there was an error */
    if (r.ax!=0x004f){return -1;}

      /* copy the resulting data into our structure */
      //dosmemget(dosbuf, sizeof(MODE_INFO), &mode_info);
      memcpy(&mode_info, buff, sizeof(MODE_INFO));

      /* it worked! */
      return 0;
   }
   int set_vesa_mode(int mode_number)
      {
         regs16_t r;


         /* call the VESA mode set function */
         r.ax = 0x4F02;
         r.bx = mode_number;
         biosint32(0x10, &r);

         /* it worked! */
         return 0;
      }
      void set_vesa_bank(int bank_number)
      {
         regs16_t r;

         r.ax = 0x4F05;
         r.bx = 0;
         r.dx = bank_number;
         biosint32(0x10, &r);
      }
      void putpixel_vesa_640x480(int x, int y, int color)
   {
      int address = y*640+x;
      int bank_size = mode_info.WinGranularity*1024;
      int bank_number = address/bank_size;
      int bank_offset = address%bank_size;

      set_vesa_bank(bank_number);

      char screen = (char)0xA0000+bank_offset;
      screen = color;
   }
