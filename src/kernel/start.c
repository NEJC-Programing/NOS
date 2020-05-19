#include <types.h>
#include <bios32.h>
extern void _start();
extern void main();
void start()
{
  /*
  * get kernel start
  * get kernel end
  * init
  * call main
  */
  print("In start\n");
  printf("_start = %x\n", _start);
  main();
  hlt();
}

