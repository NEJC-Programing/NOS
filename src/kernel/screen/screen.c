#include <screen.h>

bool textmode = true;

void print (string ch)
{
  uint16 i = 0;
  uint8 length = strlen(ch);              //Updated (Now we store string length on a variable to call the function only once)
  for(i;i<length;i++)
  {
    printch(ch[i]);
  }
}

void printch(char ch)
{
  if(textmode){
    TM_printch(ch);
  }
  else{
    memset((char *)0xA0000, ch, (320*200));
  }
}

///////////////////////////garfics/////////////////////

#define sh 200
#define sw 320

void putpixal(int x, int y, int color)
{
  string vidmem = 0xA0000;
  vidmem[y*sw+x] = color;
}


drawCircle(int xc, int yc, int x, int y) 
{ 
    putpixal(xc+x, yc+y, 2); 
    putpixal(xc-x, yc+y, 2); 
    putpixal(xc+x, yc-y, 2); 
    putpixal(xc-x, yc-y, 2); 
    putpixal(xc+y, yc+x, 2); 
    putpixal(xc-y, yc+x, 2); 
    putpixal(xc+y, yc-x, 2); 
    putpixal(xc-y, yc-x, 2); 
} 

void circleBres(int xc, int yc, int r) 
{ 
    int x = 0, y = r; 
    int d = 3 - 2 * r; 
    drawCircle(xc, yc, x, y); 
    while (y >= x) 
    { 
        // for each pixel we will 
        // draw all eight pixels 
          
        x++; 
  
        // check for decision parameter 
        // and correspondingly  
        // update d, x, y 
        if (d > 0) 
        { 
            y--;  
            d = d + 4 * (x - y) + 10; 
        } 
        else
            d = d + 4 * x + 6; 
        drawCircle(xc, yc, x, y); 
        //delay(50); 
    } 
} 