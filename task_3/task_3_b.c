#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#define THREAD_QUANTITY 4
#define TO_GENERATE 1000000
//отрезок [0;40]

int main()
{
  int *array;
  int shmid;
  char pathname[] = "pipe.c";
  key_t key;
  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  if ((shmid = shmget(key, 2*sizeof(int), 0666)) < 0){
    printf ("Can\'t connect to shared memory\n");
  }
  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can\'t attach shared memory\n");
  }
  double s = ((double)array[0])/((double)array[1])*1600;
  printf("Integral = %.5lf, theoretical quality = 800\n", s);
  shmdt(array);
  return 0;
}
