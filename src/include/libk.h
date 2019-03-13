#ifndef LIBK_H
#define LIBK_H
#include <types.h>
uint16_t strlen(const char * ch);
string decToHexa(int n);
string int_to_string(int n);
void int_to_ascii(int n, char str[]);
void * malloc(int nbytes);
#endif
