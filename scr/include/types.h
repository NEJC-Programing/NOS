#ifndef TYPES_H
#define TYPES_H

typedef signed char int8;
typedef unsigned char uint8;

typedef signed short int16;
typedef unsigned short uint16;

typedef signed int int32;
typedef unsigned int uint32;

typedef signed long long int64;
typedef unsigned long long uint64;

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef signed short int16_t;
typedef unsigned short uint16_t;

typedef signed int int32_t;
typedef unsigned int uint32_t;

typedef signed long long int64_t;
typedef unsigned long long uint64_t;

typedef unsigned short int WORD;
typedef unsigned char BYTE;
typedef unsigned int DWORD;

typedef char* string; 

typedef struct _devmgr_generic {
int size;           //size of this structure
int id;             //An ID assigned by the Device manager
int type;           //Identifies what kind of device this is
int source;         //holds the module ID of the library or 0 if it is from the kernel
char name[20];      //The name of the device
char description[256]; //a short description of the device
int lock;           //determines if this device is already being used
int (*sendmessage)(int type, int message); //used by the device to receive messages
} devmgr_generic;


typedef struct _devmgr_graphics {
    devmgr_generic hdr;
    int  (*set_graphics_mode)(int mode);
    int  (*set_text_mode)(int mode);
    int  (*write_pixel)(DWORD x,DWORD y,DWORD color);
    int  (*read_palette)(char *r,char *g,char *b,char index);
    void (*write_palette)(char r,char g,char b,char index);
} devmgr_graphics;


#define low_16(address) (uint16)((address) & 0xFFFF)            
#define high_16(address) (uint16)(((address) >> 16) & 0xFFFF)

typedef struct __Date_and_Time{
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned int  year;
} Date_and_Time;

#endif
