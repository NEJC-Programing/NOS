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
int strncmp(char* s1, char* s2, size_t n) {
	for (size_t i = 0; i < n && *s1 == *s2; s1++, s2++, i++)
		if (*s1 == '\0')
			return 0;
	return ( *(unsigned char*)s1 - *(unsigned char*)s2 );
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

string decToHexa(int n)
{   
    // char array to store hexadecimal number
    char hexaDeciNum[100];
     
    // counter for hexadecimal number array
    int i = 0;
    while(n!=0)
    {   
        // temporary variable to store remainder
        int temp  = 0;
         
        // storing remainder in temp variable.
        temp = n % 16;
         
        // check if temp < 10
        if(temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }
         
        n = n/16;
    }
     
    // printing hexadecimal number array in reverse order
    string buff;
    int f = 0;
    for(int j=i-1; j>=0; j--){
        buff[f] = hexaDeciNum[j];
        f++;
    }
    buff[f+1] = 0x00;
    return buff;
}

size_t str_backspace(string str, char c)
{
	size_t i = strlen(str);
	i--;
	while(i)
	{
		i--;
		if(str[i] == c)
		{
			str[i+1] = 0;
			return 1;
		}
	}
	return 0;
}
size_t strsplit(string str, char delim)
{
	size_t n = 0;
	uint32_t i = 0;
	while(str[i])
	{
		if(str[i] == delim)
		{
			str[i] = 0;
			n++;
		}
		i++;
	}
	n++;
	return n;
}