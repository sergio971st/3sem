#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#define THREAD_QUANTITY 1
#define TO_GENERATE 1000000
//отрезок [0;40]

int total_generated = 0;
int under_y = 0;
int active_threads = 0;
const double RAND_MAX_F = RAND_MAX;

void *mythread(void *dummy)
{
  pthread_t mythid;
  mythid = pthread_self();
  int beginning = active_threads*10*4/THREAD_QUANTITY;
  active_threads++;
  for (int j = 0; j < (TO_GENERATE*4/THREAD_QUANTITY); j++)
  {
    srand(active_threads+total_generated+under_y+j);
    double rand_val_x = beginning + ((double)rand()/RAND_MAX_F)*10*4/THREAD_QUANTITY;
    srand(time(NULL));
    double rand_val_y = beginning + ((double)rand()/RAND_MAX_F)*10*4/THREAD_QUANTITY;
    total_generated++;
    if (rand_val_x < rand_val_y) {
        under_y++;
    }
  }
  return NULL;
}

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
  if ((shmid = shmget(key, 2*sizeof(int), 0666|IPC_CREAT)) < 0){
    printf ("Can\'t create shared memory\n");
  }
  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can\'t attach shared memory\n");
  }
  pthread_t mythid;
  pthread_t thid[THREAD_QUANTITY-1];
  for (int i = 0; i < (THREAD_QUANTITY-1); i++)
  {
    int result = pthread_create( &thid[i], (pthread_attr_t *)NULL, mythread, NULL);
    if (result != 0) {
      printf ("Error on thread create, return value = %d\n", result);
      exit(-1);
    }
  }
  mythid = pthread_self();
  mythid = pthread_self();
  int beginning = active_threads*10*4/THREAD_QUANTITY;
  active_threads++;
  srand(time(NULL));
  for (int j = 0; j < (TO_GENERATE*4/THREAD_QUANTITY); j++)
  {
    srand(active_threads+total_generated+under_y);
    double rand_val_x = beginning + ((double)rand()/RAND_MAX_F)*10*4/THREAD_QUANTITY;
    srand(active_threads+total_generated+under_y+(int)rand_val_x);
    double rand_val_y = beginning + ((double)rand()/RAND_MAX_F)*10*4/THREAD_QUANTITY;
    total_generated++;
    if (rand_val_x < rand_val_y) {
        under_y++;
    }
  }
  for (int i = 0; i < (THREAD_QUANTITY-1); i++)
  {
    pthread_join(thid[i], (void *)NULL);
  }
  printf("Thread %ld(main), calculation result = %d, totally generated %d\n", mythid, under_y, total_generated);
  array[0] = under_y;
  array[1] = total_generated;
  shmdt(array);
  return 0;
}
