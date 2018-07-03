#ifndef SHELL_H
#define SHELL_H
#include "system.h"
#include "string.h"
#include "kb.h"
#include "screen.h"
#include "types.h"
#include "util.h"

void launch_shell();

void switch_old();

void switch_new();
void set_ch(int i, char ch);
#endif
