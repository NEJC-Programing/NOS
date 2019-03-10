#ifndef NOS_H
#define NOS_H
typedef enum {false, true} bool;

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef signed short int16_t;
typedef unsigned short uint16_t;

typedef signed int int32_t;
typedef unsigned int uint32_t;

typedef signed long long int64_t;
typedef unsigned long long uint64_t;

int (*printf)(const char* str, ...);
void (*set_screen_color)(char colorcode);
void (*print)(char* ch);
void (*print_colored)(char* ch,int text_color,int bg_color);
void (*clearScreen)(void);
void (*SetCursor)(int x, int y);
char* (*readStr)(void);
void * (*malloc)(int nbytes);
uint8_t (*inb)(uint16_t port);
void (*outb)(uint16_t port, uint8_t data);
char (*toupper)(char c);
char (*tolower)(char c);
uint16_t (*strlen)(char* ch);
uint8_t (*strcmp)(char* ch1, char* ch2);
void (*memcpy)(char *source, char *dest, int nbytes);
void (*memset)(uint8_t *dest, uint8_t val, uint32_t len);
void (*PutCercle)(int X, int Y, int Radius, char color));
void (*PutRect)(int X, int Y, int Width, int Height, char color);
void (*PutPixel)(int x, int y, char color);
void (*reboot)(void);
void (*shutdown)(void);
void (*panic)(char* reason, int bsod);
void (*delay)(int sec);
#endif