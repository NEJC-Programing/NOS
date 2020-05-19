#include <printf.h>
void __itoa_s(int i,unsigned base,char* buf) {
   if (base > 16) return;
   if (i < 0) {
      *buf++ = '-';
      i *= -1;
   }
   __itoa(i,base,buf);
}
char tbuf[32];
char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void __itoa(unsigned i,unsigned base,char* buf) {
	int pos = 0;
	int opos = 0;
	int top = 0;

	if (i == 0 || base > 16) {
		buf[0] = '0';
		buf[1] = '\0';
		return;
	}

	while (i != 0) {
		tbuf[pos] = bchars[i % base];
		pos++;
		i /= base;
	}
	top=pos--;
	for (opos=0; opos<top; pos--,opos++)
		buf[opos] = tbuf[pos];
	buf[opos] = 0;
}
int printf (const char* str, ...) {
	if(!str)
		return 0;
	char* s = 0;
	va_list ap;
	va_start(ap, str);
	for(uint32 i = 0; i < strlen((string)str); i++)
	{
		if(str[i] == '%')
		{
			switch(str[i+1])
			{
				/** string **/
				case 's':
					s = va_arg(ap, string);
					print(s);
					i++;
					continue;
				/** decimal **/
				case 'd': {
					int c = va_arg(ap, int);
					print(int_to_string(c));
					i++;
					continue;
				}
                                /** hex **/
				case 'x': {
					int c = va_arg(ap, int);
					print(decToHexa((int)c));
					i++;
					continue;
				}
				/** character **/
				case 'c': {
					// char gets promoted to int for va_arg!
					// clean it.
					char c = (char)(va_arg(ap, int) & ~0xFFFFFF00);
                                        int_to_ascii(va_arg(ap, int),c);
					print(c);
					i++;
					continue;
				}
				default:
					break;
			}
		} else {
			printch(str[i]);
		}
	}
	va_end(ap);
	return 1;
}
