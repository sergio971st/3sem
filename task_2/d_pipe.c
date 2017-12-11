#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

//родитель записывает в txd[1], ребенок записывает в rxd[1]
typedef struct {
  int txd[2];
  int rxd[2];
} dpipe_t;

int main()
{
  char parent_read[4096];
  char child_read[4096];
  int size;
  dpipe_t dpipe;
  if(pipe(dpipe.txd) < 0)
  {
    printf("Can't create 1st pipe\n");
    exit(-1);
  }
  if(pipe(dpipe.rxd) < 0)
  {
    printf("Can't create 2nd pipe\n");
    exit(-1);
  }
  int pid = fork();
  if(pid < 0)
  {
    printf("Unable to fork\n");
    exit(-1);
  }
  if (pid > 0)
  {
    close(dpipe.txd[0]);
    close(dpipe.rxd[1]);
    size = write(dpipe.txd[1], "Hello, child!", 14);
    if (size != 14)
    {
      printf("Trouble with writing\n");
      exit(-1);
    }
    size = read(dpipe.rxd[0], parent_read, 14);
    if (size < 0)
    {
      printf("Trouble with reading\n");
      exit(-1);
    }
    printf("parent: %s\n", parent_read);
    close(dpipe.txd[1]);
    close(dpipe.rxd[0]);
  }
  if (pid == 0)
  {
    close(dpipe.rxd[0]);
    close(dpipe.txd[1]);
    size = write(dpipe.rxd[1], "Hello,parent!", 14);
    if (size != 14)
    {
      printf("Trouble with writing\n");
      exit(-1);
    }
    size = read(dpipe.txd[0], child_read, 14);
    if (size < 0)
    {
      printf("Trouble with reading\n");
      exit(-1);
    }
    printf("child: %s\n", child_read);
    close(dpipe.rxd[1]);
    close(dpipe.txd[0]);
  }
}
