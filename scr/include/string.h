#ifndef STRING_H
#define STRING_H

#include "types.h"
uint16 strlength(string ch);

uint8 strEql(string ch1,string ch2);

uint8 strEqle(string ch1,string ch2, int chars);

string remove_form_start(string str, int chars_to_remove);

//string remove_from_end(string str, int chars_to_remove);

string first(string str, int ch);

char toupper(char c);
 char tolower(char c);
#endif
