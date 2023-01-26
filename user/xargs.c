#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

char whitespace[] = " \t\r\n\v";

int
getcmd(char *buf, int nbuf)
{
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

int
parsecmd(int idx, char **new_argv, char **term_argv, char *buf)
{
  char *l = buf, *r = buf;
  int fl = 0;
  for(; *r != 0; ){
    // printf("char %d\n", *r);
    fl = 0;
    if (!strchr(whitespace, *r)){
      ++r;
    }
    else{
      fl = 1;
      new_argv[idx] = l;
      term_argv[idx] = r;
      // printf("idx %d\n", idx);
      ++idx;
      while(strchr(whitespace, *r)){
        ++r;
      }
      l = r;
    }
  }
  if (fl == 0){
    new_argv[idx] = l;
    term_argv[idx] = r;
    ++idx;
  }
  new_argv[idx] = 0;
  term_argv[idx] = 0;
  return idx;
}

int
xargs(char *buf, char **argv)
{
  int pid;
  if ((pid = fork()) < 0){
    fprintf(2, "fork: failed to fork process\n");
    return -1;
  }

  if (pid == 0){
    char **new_argv = &argv[1];
    exec(argv[1], new_argv);
    fprintf(2, "exec: couldn't execute %s\n", argv[1]);
    return -1;
  }
  wait(0);
  return 0;
}

int
main(int argc, char *argv[])
{
  if (argc < 2){
    fprintf(2, "Usage: xargs args\n");
    exit(1);
  }
  if (argc > MAXARG){
    fprintf(2, "xargs: too many arguments\n");
    exit(1);
  }

  char buf[512];
  char *new_argv[MAXARG], *term_argv[MAXARG];
  for(int i = 0 ; i < argc ; i++){
    new_argv[i] = argv[i];
  }
  new_argv[argc] = 0;

  while(getcmd(buf, sizeof(buf)) >= 0){
    int lim = parsecmd(argc, new_argv, term_argv, buf);
    // printf("%s\n", buf);
    // printf("%d %d\n", argc, lim);
    for(int i=argc; i<lim; i++){
      *(term_argv[i]) = 0;
    }
    // for(int i=0; i<lim; i++){
    //   printf("%s\n", new_argv[i]);
    // }
    if (xargs(buf, new_argv) < 0){
      fprintf(2, "xargs: error running cmd\n");
      exit(1);
    }
  }

  exit(0);
}