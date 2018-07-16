#include "../include/string.h"
uint16 strlength(string ch){
        uint16 i = 0;           //Changed counter to 0
        while(ch[i++]);  
        return i-1;               //Changed counter to i instead of i--
}

uint16 strlen(string ch)
{
        return strlength(ch);
}


uint8 strEql(string ch1,string ch2)                     
{
        uint8 result = 1;
        uint8 size = strlength(ch1);
        if(size != strlength(ch2)) result =0;
        else 
        {
        uint8 i = 0;
        for(i;i<=size;i++)
        {
                if(ch1[i] != ch2[i]) result = 0;
        }
        }
        return result;
}
uint8 strcmp(string ch1, string ch2){
       uint8 result = 0;
        uint8 size = strlength(ch1);
        if(size != strlength(ch2)) result =0;
        else 
        {
        uint8 i = 0;
        for(i;i<=size;i++)
        {
                if(ch1[i] != ch2[i]) result = 1;
        }
        }
        return result; 
}
uint8 strEqle(string ch1,string ch2, int chars)                     
{
        uint8 result = 1;
        int i = 0;
        while(i != chars){
                if(ch1[i] != ch2[i]) result = 0;
                i++;
        }
        return result;
}

string remove_form_start(string str, int chars_to_remove)
{
        int size = strlength(str);
        //print(int_to_string(size));
        string buff;
        int i = chars_to_remove;
        int j = 0;
       // print(int_to_string(size - i));
        while(i != size){
                buff[j] = str[i];
                i++;
                j++;
        }
        return buff;
}

string first(string str, int ch){
        string buff;
        if (strlength(str)<=ch)
                return str;
        for(int i =0; i!=ch-1; i++)
                buff[i]=str[i];
        return buff;
}
char toupper(char c)
 {
  if (c>='a'&&c<='z') return (c-'a'+'A');
  return c;
 };
 char tolower(char c)
 {
  if (c>='A'&&c<='Z') return (c-'A'+'a');
  return c;
 };

 static char hex [] = { '0', '1', '2', '3', '4', '5', '6', '7',
                        '8', '9' ,'A', 'B', 'C', 'D', 'E', 'F' };
 
//The function that performs the conversion. Accepts a buffer with "enough space" TM 
//and populates it with a string of hexadecimal digits.Returns the length in digits
string uintToHexStr(unsigned int num)
{
    string buff;
    int len=0,k=0;
    do//for every 4 bits
    {
        //get the equivalent hex digit
        buff[len] = hex[num];
        len++;
        num>>=4;
    }while(num!=0);
    //since we get the digits in the wrong order reverse the digits in the buffer
    for(;k<len/2;k++)
    {//xor swapping
        buff[k]^=buff[len-k-1];
        buff[len-k-1]^=buff[k];
        buff[k]^=buff[len-k-1];
    }
    //null terminate the buffer and return the length in digits
    buff[len]='\0';
    return buff;
}