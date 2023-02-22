#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
/*
This program provides a possible solution for first reader writer
problems using mutex and semaphore.
I have used 10 readers and 5 producers to demonstrate the
solution. You can always play with these values.
*/
sem_t db;
pthread_mutex_t mutex;
int cnt = 1; // content of db
int rc = 0;
void *writer(void *wno)
{
sem_wait(&db);
cnt = cnt*2;
printf("Writer %d modified cnt to %d\n",(*((int *)wno)),cnt);
sem_post(&db);
}
void *reader(void *rno)
{
// Reader acquire the lock before modifying rc

pthread_mutex_lock(&mutex); // ………….(1)
rc++; //………………(2)

if(rc == 1) {
sem_wait(&db); // If this id the first reader, then it will block the writer
}
pthread_mutex_unlock(&mutex);
// Reading Section
printf("Reader %d: read cnt as %d\n",*((int *)rno),cnt);
// Reader acquire the lock before modifying rc

rc--; // ………(4)
pthread_mutex_lock(&mutex); // …….(3)

if(rc == 0) {
sem_post(&db); // If this is the last reader, it will wake up the writer.
}
pthread_mutex_unlock(&mutex);
}
int main()
{
pthread_t read[10],write[5];
pthread_mutex_init(&mutex, NULL);
sem_init(&db,0,1);
int a[10] = {1,2,3,4,5,6,7,8,9,10}; //Just used for numbering the producer and consumer
for(int i = 0; i < 10; i++) {
pthread_create(&read[i], NULL, (void *)reader, (void*)&a[i]);
} for(int i=0; i<5; i++) {
pthread_create(&write[i], NULL, (void *)writer, (void*)&a[i]);
}
for(int i = 0; i < 10; i++) {
pthread_join(read[i], NULL);
} for(int i=0; i<5; i++) {
pthread_join(write[i], NULL);
}
pthread_mutex_destroy(&mutex);
sem_destroy(&db);
return 0;
}