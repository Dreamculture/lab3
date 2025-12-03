// user/alarmtest.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

volatile static int count;

void
periodic()
{
  count++;
  printf("alarm!\n");
}

int
main(int argc, char *argv[])
{
  int i;

  printf("test0 start\n");
  sigalarm(2, periodic);
  for(i = 0; i < 1000*500000; i++){
    if (count >= 10)
      break;
  }
  if(count < 10){
    printf("test0 failed: count=%d\n", count);
    exit(1);
  }
  printf("test0 passed\n");
  exit(0);
}
