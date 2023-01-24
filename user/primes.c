#include "kernel/types.h"
#include "user/user.h"

void
pipeline(int *pipefds)
{
  int n;
  close(pipefds[1]);
  if (read(pipefds[0], &n, sizeof(int)) == 0){
    close(pipefds[0]);
    exit(0);
  }
  printf("prime %d\n", n);

  int p, pid, pipeTemp[2];
  pipe(pipeTemp);

  if ((pid = fork()) < 0){
    fprintf(2, "fork: failed to fork process\n");
    exit(1);
  }

  if (pid == 0){
    pipeline(pipeTemp);
  }
  close(pipeTemp[0]);
  while(read(pipefds[0], &p, sizeof(int)) != 0){
    if (p % n){
        write(pipeTemp[1], &p, sizeof(int));
    }
  }
  close(pipefds[0]);
  close(pipeTemp[1]);
  wait(0);
  exit(0);
}

int
main(int argc, char *argv[])
{
  int pipefds[2];
  if (pipe(pipefds) < 0){
    fprintf(2, "pipe: failed to create pipe\n");
    exit(1);
  }

  int pid;
  if ((pid = fork()) < 0){
    fprintf(2, "fork: failed to fork process\n");
    exit(1);
  }

  if (pid == 0){
    pipeline(pipefds);
  }

  close(pipefds[0]);
  for(int i = 2; i <= 35; i++){
    write(pipefds[1], &i, sizeof(int));
  }
  close(pipefds[1]);

  wait(0);
  exit(0);
}