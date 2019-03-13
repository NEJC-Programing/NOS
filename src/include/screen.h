#ifndef SCREEN_H
#define SCREEN_H
#include <types.h>
#include <ports.h>
int cursorX , cursorY;
const uint8 sw ,sh ,sd ;

void clearLine(uint8 from,uint8 to);
void clearScreen();//
void scrollUp(uint8 lineNumber);//
void printch(char c);
void print (string ch);
int printf (const char* str, ...);//
void setcolor(int color_code);//
void print_colored(string ch, int color_code);//
void SetCursor(int x, int y);//
////////////////////////////////////////////////////////////////////////////////
///////////////////////TEXT//MODE///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void TM_clearLine(uint8 from,uint8 to);
void TM_updateCursor();
void TM_clearScreen();
void TM_scrollUp(uint8 lineNumber);
void TM_newLineCheck();
void TM_printch(char c);
void TM_print (string ch);
void TM_setcolor(int color_code);
void TM_print_colored(string ch, int color_code);
void TM_SetCursor(int x, int y);
////////////////////////////////////////////////////////////////////////////////
//////////////////////           ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
