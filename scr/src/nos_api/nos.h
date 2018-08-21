void* NOS_create();
void NOS_release(void* nos);
void NOS_put_ch(void* nos, char c,int x, int y, int text_c, int bg_c);
unsigned char NOS_Get_sw(void* nos);
unsigned char NOS_Get_sh(void* nos);
unsigned char NOS_strlen(void* nos, char* string);