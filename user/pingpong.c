#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pipeOne[2];
  int pipeTwo[2];
  if (pipe(pipeOne) < 0){
    fprintf(2, "pipe: failed to create pipe\n");
    exit(1);
  }
  if (pipe(pipeTwo) < 0){
    fprintf(2, "pipe: failed to create pipe\n");
    exit(1);
  }

  int pid; 
  if ((pid = fork()) < 0){
    fprintf(2, "fork: failed to fork process\n");
    exit(1);
  }

  if (pid == 0){
    close(pipeOne[1]);
    close(pipeTwo[0]);
    char c;
    read(pipeOne[0], &c, 1);
    if (c == 'a'){
        c = 'b';
        printf("%d: received ping\n", getpid());
        write(pipeTwo[1], &c, 1);
    }
    close(pipeOne[0]);
    close(pipeTwo[1]);
  }
  else{
    close(pipeOne[0]);
    close(pipeTwo[1]);

    char c = 'a';
    write(pipeOne[1], &c, 1);
    read(pipeTwo[0], &c, 1);
    if (c == 'b'){
        printf("%d: received pong\n", getpid());
    }
    close(pipeOne[0]);
    close(pipeTwo[1]);
  }

  exit(0);
}