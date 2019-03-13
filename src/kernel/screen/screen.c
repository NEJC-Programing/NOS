#include <screen.h>

bool textmode = true;

void print (const char * ch)
{
  if (textmode){
    TM_print(ch);
  }
}

void printch(char ch)
{
  if(textmode){
    TM_printch(ch);
  }
}
