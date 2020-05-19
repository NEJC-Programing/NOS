#include <libk.h>

uint16_t strlen(string ch)
{
  uint16_t i = 0;
  while (ch[i++]);
  return i-1;
}


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

void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    /* TODO: implement "reverse" */
    return str;
}
string int_to_string(int n)
{
	string ch = malloc(50);
	int_to_ascii(n,ch);
	int len = strlen(ch);
	int i = 0;
	int j = len - 1;
	while(i<(len/2 + len%2))
	{
		char tmp = ch[i];
		ch[i] = ch[j];
		ch[j] = tmp;
		i++;
		j--;
	}
	return ch;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void * malloc(int nbytes)
{
	char variable[nbytes];
	return &variable;
}


int strncmp (const char *s1, const char *s2, size_t n)
{
  
  unsigned char c1 = '\0';
  unsigned char c2 = '\0';
  if (n >= 4)
    {
      size_t n4 = n >> 2;
      do
        {
          c1 = (unsigned char) *s1++;
          c2 = (unsigned char) *s2++;
          if (c1 == '\0' || c1 != c2)
            return c1 - c2;
          c1 = (unsigned char) *s1++;
          c2 = (unsigned char) *s2++;
          if (c1 == '\0' || c1 != c2)
            return c1 - c2;
          c1 = (unsigned char) *s1++;
          c2 = (unsigned char) *s2++;
          if (c1 == '\0' || c1 != c2)
            return c1 - c2;
          c1 = (unsigned char) *s1++;
          c2 = (unsigned char) *s2++;
          if (c1 == '\0' || c1 != c2)
            return c1 - c2;
        } while (--n4 > 0);
      n &= 3;
    }
  while (n > 0)
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0' || c1 != c2)
        return c1 - c2;
      n--;
    }
  return c1 - c2;
}

#define        op_t        unsigned long int
#define OPSIZ        (sizeof(op_t))
typedef unsigned char byte;
void * memset (void *dstpp, int c, size_t len)
{
  long int dstp = (long int) dstpp;
  if (len >= 8)
    {
      size_t xlen;
      op_t cccc;
      cccc = (unsigned char) c;
      cccc |= cccc << 8;
      cccc |= cccc << 16;
      if (OPSIZ > 4)
        /* Do the shift in two steps to avoid warning if long has 32 bits.  */
        cccc |= (cccc << 16) << 16;
      /* There are at least some bytes to set.
         No need to test for LEN == 0 in this alignment loop.  */
      while (dstp % OPSIZ != 0)
        {
          ((byte *) dstp)[0] = c;
          dstp += 1;
          len -= 1;
        }
      /* Write 8 `op_t' per iteration until less than 8 `op_t' remain.  */
      xlen = len / (OPSIZ * 8);
      while (xlen > 0)
        {
          ((op_t *) dstp)[0] = cccc;
          ((op_t *) dstp)[1] = cccc;
          ((op_t *) dstp)[2] = cccc;
          ((op_t *) dstp)[3] = cccc;
          ((op_t *) dstp)[4] = cccc;
          ((op_t *) dstp)[5] = cccc;
          ((op_t *) dstp)[6] = cccc;
          ((op_t *) dstp)[7] = cccc;
          dstp += 8 * OPSIZ;
          xlen -= 1;
        }
      len %= OPSIZ * 8;
      /* Write 1 `op_t' per iteration until less than OPSIZ bytes remain.  */
      xlen = len / OPSIZ;
      while (xlen > 0)
        {
          ((op_t *) dstp)[0] = cccc;
          dstp += OPSIZ;
          xlen -= 1;
        }
      len %= OPSIZ;
    }
  /* Write the last few bytes.  */
  while (len > 0)
    {
      ((byte *) dstp)[0] = c;
      dstp += 1;
      len -= 1;
    }
  return dstpp;
}

void * memcpy (void *dstpp, const void *srcpp, size_t len)
{
  char * dest = (char*)dstpp;
  char * src = (char*)srcpp;
  int i;
    for (i = 0; i < len; i++) {
        dest[i] = src[i];
    }
    return dstpp;
}

