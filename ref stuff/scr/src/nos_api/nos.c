#include "nos.h"



void printf(const char* text,...)
{

}



unsigned char CX = 0;
unsigned char CY = 0;

char text_c = 0xF;
char bg_c = 0;
void putc(char c)
{
    void* NOS = NOS_create();
    unsigned char sw = NOS_Get_sw(NOS);
    unsigned char sh = NOS_Get_sh(NOS);
    switch(c)
    {
        case (0x08):
            if(CX > 0) 
            {
	            CX--;
                NOS_put_ch(NOS,0,CX,CY,text_c,bg_c);                      
	        }
	        break;
       /* case (0x09):
                CX = (CX + 8) & ~(8 - 1); 
                break;*/
        case ('\r'):
                CX = 0;
                break;
        case ('\n'):
                CX = 0;
                CY++;
                break;
        default:
                NOS_put_ch(NOS,c,CX,CY,text_c,bg_c);
                CX++; 
                break;
	
    }
    if(CX >= sw)                                                                   
    {
        CX = 0;                                                                
        CY++;                                                                    
    }
    NOS_release(NOS);
}

void puts(char* text)
{
    void* NOS = NOS_create();
    unsigned short i = 0;
    unsigned char l = NOS_strlen(NOS,text);
    for (i;i<l;i++)
    {
        putc(text[i]);
    }
    NOS_release(NOS);
}