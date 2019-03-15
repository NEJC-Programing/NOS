#ifndef VESA_H
#define VESA_H
#include <types.h>
struct VbeInfoBlock {
   char VbeSignature[4];             // == "VESA"
   uint16_t VbeVersion;                 // == 0x0300 for VBE 3.0
   uint16_t OemStringPtr[2];            // isa vbeFarPtr
   uint8_t Capabilities[4];
   uint16_t VideoModePtr[2];         // isa vbeFarPtr
   uint16_t TotalMemory;             // as # of 64KB blocks
} __attribute__((packed));

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
uint16_t findMode(int x, int y, int d);
#endif
