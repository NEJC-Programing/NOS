#ifndef VESA_H
#define VESA_H
#include <types.h>

typedef struct VESA_INFO
{
   unsigned char  VESASignature[4]     __attribute__ ((packed));
   unsigned short VESAVersion          __attribute__ ((packed));
   unsigned long  OEMStringPtr         __attribute__ ((packed));
   unsigned char  Capabilities[4]      __attribute__ ((packed));
   unsigned long  VideoModePtr         __attribute__ ((packed));
   unsigned short TotalMemory          __attribute__ ((packed));
   unsigned short OemSoftwareRev       __attribute__ ((packed));
   unsigned long  OemVendorNamePtr     __attribute__ ((packed));
   unsigned long  OemProductNamePtr    __attribute__ ((packed));
   unsigned long  OemProductRevPtr     __attribute__ ((packed));
   unsigned char  Reserved[222]        __attribute__ ((packed));
   unsigned char  OemData[256]         __attribute__ ((packed));
} VESA_INFO;


typedef struct MODE_INFO
   {
      unsigned short ModeAttributes       __attribute__ ((packed));
      unsigned char  WinAAttributes       __attribute__ ((packed));
      unsigned char  WinBAttributes       __attribute__ ((packed));
      unsigned short WinGranularity       __attribute__ ((packed));
      unsigned short WinSize              __attribute__ ((packed));
      unsigned short WinASegment          __attribute__ ((packed));
      unsigned short WinBSegment          __attribute__ ((packed));
      unsigned long  WinFuncPtr           __attribute__ ((packed));
      unsigned short BytesPerScanLine     __attribute__ ((packed));
      unsigned short XResolution          __attribute__ ((packed));
      unsigned short YResolution          __attribute__ ((packed));
      unsigned char  XCharSize            __attribute__ ((packed));
      unsigned char  YCharSize            __attribute__ ((packed));
      unsigned char  NumberOfPlanes       __attribute__ ((packed));
      unsigned char  BitsPerPixel         __attribute__ ((packed));
      unsigned char  NumberOfBanks        __attribute__ ((packed));
      unsigned char  MemoryModel          __attribute__ ((packed));
      unsigned char  BankSize             __attribute__ ((packed));
      unsigned char  NumberOfImagePages   __attribute__ ((packed));
      unsigned char  Reserved_page        __attribute__ ((packed));
      unsigned char  RedMaskSize          __attribute__ ((packed));
      unsigned char  RedMaskPos           __attribute__ ((packed));
      unsigned char  GreenMaskSize        __attribute__ ((packed));
      unsigned char  GreenMaskPos         __attribute__ ((packed));
      unsigned char  BlueMaskSize         __attribute__ ((packed));
      unsigned char  BlueMaskPos          __attribute__ ((packed));
      unsigned char  ReservedMaskSize     __attribute__ ((packed));
      unsigned char  ReservedMaskPos      __attribute__ ((packed));
      unsigned char  DirectColorModeInfo  __attribute__ ((packed));
      unsigned long  PhysBasePtr          __attribute__ ((packed));
      unsigned long  OffScreenMemOffset   __attribute__ ((packed));
      unsigned short OffScreenMemSize     __attribute__ ((packed));
      unsigned char  Reserved[206]        __attribute__ ((packed));
   } MODE_INFO;
//////////////////////////////////////////////////////////
int GoGraphics();
int get_vesa_info();
int get_mode_info(int mode);
//int find_vesa_mode(int w, int h);
//int set_vesa_mode(int w, int h);
int set_vesa_mode(int mode_number);
void set_vesa_bank(int bank_number);
void putpixel_vesa_640x480(int x, int y, int color);
void copy_to_vesa_screen(char *memory_buffer, int screen_size);
#endif
