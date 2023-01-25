#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int find(char *start, char *file)
{
  struct dirent de;
  struct stat st;
  char buf[512], *p;

  int fd;
  if((fd = open(start, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", start);
    return -1;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", start);
    close(fd);
    return -1;
  }

  if(st.type != T_DIR){
    fprintf(2, "find: %s not a directory\n", start);
    return -1;
  }
  
  strcpy(buf, start);
  p = buf + strlen(start);
  *p = '/';
  ++p;

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0){
        continue;
    }

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", start);
      close(fd);
      return -1;
    }

    // printf("test: %s\n", de.name);
    if(st.type == T_DIR && strcmp(de.name, ".") && strcmp(de.name, "..") && strcmp(de.name, file)){
      find(buf, file);
    }
    else if(!strcmp(de.name, file)){
      printf("%s\n", buf);
    }
  }
  close(fd);
  return 0;
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "Usage: find starting_point file_name\n");
    exit(1);
  }

  if(find(argv[1], argv[2]) < 0){
    exit(1);
  }

  exit(0);
}