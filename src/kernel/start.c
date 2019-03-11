#include <types.h>
extern void _start();
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
}
