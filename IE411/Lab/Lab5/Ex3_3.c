#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

sem_t db;
sem_t oktowrite;
sem_t oktoread;
int aw=0; // active writer
int ar=0; // active reader
int wr=0; // waiting reader
int ww=0; // waiting writer
pthread_mutex_t mutex; // mutex
int cnt = 1; // content of file

void *writer(void *wno)
{
  sleep(1);
  pthread_mutex_lock(&mutex); // lock mutex
  if(aw+ar>0) // if ac or aw is present
	  ww++; // increase ww
  else{
    sem_post(&oktowrite); // increase oktowrite 
    aw++; // increase aw
  }
  pthread_mutex_unlock(&mutex); // unlock mutex
  sem_wait(&oktowrite);
  cnt=cnt*2;
  printf("writing %d \n",cnt);
  pthread_mutex_lock(&mutex); // lock mutex
  aw--; // decrease aw
  if(ww>0) // if ww is present
  {
    sem_post(&oktowrite); // increase oktowrite
    aw++; // increase aw
    ww--; // decrease ww
  }else if(wr>0) // if wr is present
  {
	  sem_post(&oktoread); // increase oktoread
	  ar++; // increase ar
	  wr--; // decrease wr
  }
  pthread_mutex_unlock(&mutex); // unlock mutex
}

void *reader(void *rno)
{
  sleep(1);
  pthread_mutex_lock(&mutex); // lock mutex
  if(aw+ww>0) // if aw or ww is present
	  wr++; // increase wr
  else{
    sem_post(&oktoread); // increase oktoread
    ar++; // increase ar
  }
  pthread_mutex_unlock(&mutex); // unlock mutex
  sem_wait(&oktoread);
  printf("reading %d \n",cnt);
  pthread_mutex_lock(&mutex); // lock mutex
  ar--; // decrease ar
  if(ar==0 && ww>0) // if ar is absent and ww is present
  {
    sem_post(&oktowrite); // increase oktowrite
    aw++; // increase aw
    ww--; // decrease ww
  }
  pthread_mutex_unlock(&mutex); // unlock mutex
}

int main(){

pthread_t read[10],write[10]; // two threades

pthread_mutex_init(&mutex, NULL); // initialize mutex
sem_init(&db,0,1); // initialize semaphore
sem_init(&oktoread,0,1);
sem_init(&oktowrite,0,1);

int a[10] = {1,2,3,4,5,6,7,8,9,10}; 

// creating threads for reader and writer
for(int i = 0; i < 10; i++) {
  pthread_create(&read[i], NULL, (void *)reader, (void*)&a[i]);
  pthread_create(&write[i], NULL, (void *)writer, (void*)&a[i]);
}

// waiting for all threades to complete
for(int i = 0; i < 10; i++){
  pthread_join(read[i], NULL);
}
for(int i = 0; i < 5; i++){
  pthread_join(write[i], NULL);
}

pthread_mutex_destroy(&mutex); // destroy mutex
sem_destroy(&db); // destroy semaphore
sem_destroy(&oktoread);
sem_destroy(&oktowrite);

return 0;
}