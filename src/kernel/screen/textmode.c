#include <screen.h>

int cursorX = 0, cursorY = 0;
const uint8 sw = 80,sh = 26,sd = 2;
int color = 0x0F;

void TM_clearLine(uint8 from,uint8 to)
{
        uint16 i = sw * from * sd;
        char* vidmem=(char*)0xb8000;
        for(i;i<(sw*to*sd);i++)
        {
                vidmem[(i / 2)*2 + 1 ] = color ;
                vidmem[(i / 2)*2 ] = 0;
        }
}
void TM_updateCursor()
{
    unsigned temp;

    temp = cursorY * sw + cursorX-1;                                                      // Position = (y * width) +  x

    outb(0x3D4, 14);                                                                // CRT Control Register: Select Cursor Location
    outb(0x3D5, temp >> 8);                                                         // Send the high byte across the bus
    outb(0x3D4, 15);                                                                // CRT Control Register: Select Send Low byte
    outb(0x3D5, temp);                                                              // Send the Low byte of the cursor location
}
void TM_clearScreen()
{
        TM_clearLine(0,sh-1);
        cursorX = 0;
        cursorY = 0;
        TM_updateCursor();
}

void TM_scrollUp(uint8 lineNumber)
{
        char* vidmem = (char*)0xb8000;
        uint16 i = 0;
        TM_clearLine(0,lineNumber-1);                                            //updated
        for (i;i<sw*(sh-1)*2;i++)
        {
                vidmem[i] = vidmem[i+sw*2*lineNumber];
        }
        TM_clearLine(sh-1-lineNumber,sh-1);
        if((cursorY - lineNumber) < 0 )
        {
                cursorY = 0;
                cursorX = 0;
        }
        else
        {
                cursorY -= lineNumber;
        }
        TM_updateCursor();
}


void TM_newLineCheck()
{
        if(cursorY >=sh-1)
        {
                TM_scrollUp(1);
        }
}



void TM_printch(char c)
{
    char* vidmem = (char*) 0xb8000;
    switch(c)
    {
        case (0x08):
                if(cursorX > 0)
                {
	                cursorX--;
                        vidmem[(cursorY * sw + cursorX)*sd]=0;	     //(0xF0 & color)
	        }
	        break;
       /* case (0x09):
                cursorX = (cursorX + 8) & ~(8 - 1);
                break;*/
        case ('\r'):
                cursorX = 0;
                break;
        case ('\n'):
                cursorX = 0;
                cursorY++;
                break;
        default:
                vidmem [((cursorY * sw + cursorX))*sd] = c;
                vidmem [((cursorY * sw + cursorX))*sd+1] = color;
                cursorX++;
                break;

    }
    if(cursorX >= sw)
    {
        cursorX = 0;
        cursorY++;
    }
    TM_updateCursor();
    TM_newLineCheck();
}

void TM_print (const char * ch)
{
        uint16 i = 0;
        uint8 length = strlen(ch);              //Updated (Now we store const char * length on a variable to call the function only once)
        for(i;i<length;i++)
        {
                TM_printch(ch[i]);
        }

}
void TM_setcolor(int color_code)
{
  color = color_code;
}

void TM_print_colored(const char * ch,int color_code)
{
	int current_color = color;
	TM_setcolor(color_code);
	TM_print(ch);
	TM_setcolor(current_color);
}

void TM_SetCursor(int x, int y){
        cursorX = x;
        cursorY = y;
        TM_updateCursor();
}
