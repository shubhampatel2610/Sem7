#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define MaxItems 3 // Maximum items a producer can produce or a consumer can consume
#define BufferSize 3 // Size of the buffer

sem_t empty; // empty variable
sem_t full; // full variable
int in = 0; // in for producer
int out = 0; // out for consumer
int buffer[BufferSize]; // buffer
pthread_mutex_t mutex; // mutex

void *producer(void *pno){   
    int item;
    for(int i = 0; i < MaxItems; i++){
        item = rand(); // Produce an random item

        sem_wait(&empty); // lock semaphore (empty--)
        pthread_mutex_lock(&mutex); // lock mutex

        buffer[in] = item; // put produced item in buffer
        printf("Producer %d: Insert Item %d at %d\n", *((int *)pno),buffer[in],in); // item detail
        in = (in+1)%BufferSize; // increase in

        pthread_mutex_unlock(&mutex); // unlock mutex
        sem_post(&full); // release semaphore (full++)
    }
}

void *consumer(void *cno){   
    for(int i = 0; i < MaxItems; i++){

        sem_wait(&full); // lock semaphore (full--)
        pthread_mutex_lock(&mutex); // lock mutex

        int item = buffer[out]; // get item from buffer
        printf("Consumer %d: Remove Item %d from %d\n",*((int *)cno),item, out); // item detail
        out = (out+1)%BufferSize; // increase out

        pthread_mutex_unlock(&mutex); // unlock mutex
        sem_post(&empty); // release semaphore (empty++)
    }
}

int main(){   

    pthread_t pro[3],con[3]; // threads
    pthread_mutex_init(&mutex, NULL); // create mutex
    sem_init(&empty,0,BufferSize); // initialize semaphore
    sem_init(&full,0,0); // initialize semaphore

    int a[3] = {1,2,3}; //Just used for numbering the producer and consumer

    // creating thread for producer
    for(int i = 0; i < 3; i++){
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    // creating thread for consumer
    for(int i = 0; i < 3; i++){
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    // waiting for producer threads to complete
    for(int i = 0; i < 3; i++){
        pthread_join(pro[i], NULL);
    }
    // waiting for consumer threads to complete
    for(int i = 0; i < 3; i++){
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex); // destroy mutex
    sem_destroy(&empty); // destroy semaphore
    sem_destroy(&full); // destroy semaphore

    return 0;
    
}