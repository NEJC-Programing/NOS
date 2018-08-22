#ifndef KB_H
#define KB_H
#include "screen.h"
#include "util.h"
#include "system.h"
void keyboard_handler(register_t * r);

void keyboard_init();
string readStr_buff();
string readStr();
#endif
