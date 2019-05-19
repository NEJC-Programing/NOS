#include <types.h>
#include <bios32.h>
extern void _start();
extern void GoGraphics();
extern bool textmode;
void start()
{
  /*
  * get kernel start
  * get kernel end
  * init
  * call main
  */
  print("In start\n");
//  printf("_start = %x\n", _start);
  printf("%s\n", "go");
  regs16_t regs;
  regs.ax = 0x0013;
  biosint32(0x10, &regs);
  memset((char *)0xA0000, 1, (320*200));
  textmode = false;
  circleBres(100,100,50);
  for (int i = -1000; i<0xffffff; i++){
  for (int i = -1000; i<0xffffff; i++);}
  print("test\n");
  hlt();
}

