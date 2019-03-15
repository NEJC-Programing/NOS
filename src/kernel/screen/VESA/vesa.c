#include "vesa.h"
#include <libk.h>
#include <bios32.h>
#include <ports.h>
////////////////////////////////////////////////////////////////////////////////
///////////////http://www.delorie.com/djgpp/doc/ug/graphics/vesa.html///////////
////////////////////////////////////////////////////////////////////////////////

VESA_INFO vesa_info;

void GoGraphics()
{
  if(get_vesa_info() !=0)
  {
    return -1;
  }
}

int get_vesa_info()
   {
      regs16_t r;
      int c;

      VESA_INFO *buff = malloc(sizeof(VESA_INFO));
      memset(buff, 0, sizeof(VESA_INFO));
      memcpy(buff->VESASignature, "VBE2", 4);

      /* call the VESA function */
      r.ax = 0x4F00;
      r.di = buff;
  //    r.es = (buff>>4) & 0xFFFF;
      biosint32(0x10, &r);

      if (r.ax!=0x004f){return -1;}
      /* copy the resulting data into our structure */
      memcpy(&vesa_info, buff, sizeof(VESA_INFO));
      /* check that we got the right magic marker value */
      if (strncmp(vesa_info.VESASignature, "VESA", 4) != 0) return -1;

      /* it worked! */
      return 0;
   }
