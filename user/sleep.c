#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(2, "Usage: sleep ticks\n");
    exit(1);
  }

  int ticks = atoi(argv[1]);
  if(sleep(ticks) < 0){
    fprintf(2, "sleep: failed to sleep for %s ticks\n", argv[1]);
    exit(1);
  }

  exit(0);
}