#ifndef LIBK_H
#define LIBK_H
#include <types.h>
uint16_t strlen(string ch);
string decToHexa(int n);
string int_to_string(int n);
void int_to_ascii(int n, char str[]);
void * malloc(int nbytes);
int strncmp (const char *s1, const char *s2, size_t n);
void * memset (void *dstpp, int c, size_t len);
void * memcpy (void *dstpp, const void *srcpp, size_t len);
#endif
