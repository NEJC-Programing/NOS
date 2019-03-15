#include <types.h>
extern void _start();
extern void GoGraphics();
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
  GoGraphics();
  print("test\n");
  for(;;);
}
