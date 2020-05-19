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

///////////////////////////////////////////

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef signed short int16_t;
typedef unsigned short uint16_t;

typedef signed int int32_t;
typedef unsigned int uint32_t;

typedef signed long long int64_t;
typedef unsigned long long uint64_t;

typedef unsigned int size_t;

///////////////////////////////////////////

typedef unsigned short int WORD;
typedef unsigned char BYTE;
typedef unsigned int DWORD;

///////////////////////////////////////////

typedef char* string;
#define NULL ((void*) 0)
typedef enum {false, true} bool;

/////////////////////////////////////////////

#define hlt() asm("hlt")

#endif
