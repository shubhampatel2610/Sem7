#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

int balance=500; // bank balance
pthread_mutex_t mutex; // mutex

void* credit(int amount){
    pthread_mutex_lock(&mutex); // lock mutex
    balance=balance+amount; // add amount in balance
    pthread_mutex_unlock(&mutex); // unlock mutex
}

void* debit(int amount){
    pthread_mutex_lock(&mutex); // lock mutex
    balance=balance-amount; // subtract amount from balance
    pthread_mutex_unlock(&mutex); // unlock mutex
}

int main(){
    pthread_mutex_init(&mutex, NULL); // create mutex
       
    pthread_t bank[2]; // create 2 threads

    pthread_create(&bank[0], NULL, (void *)credit,(int *)50); // thread for credit amount
    pthread_create(&bank[1], NULL, (void *)debit, (int *)50); // thread for debit amount

    // waiting for both threads to complete
    pthread_join(bank[0], NULL);
    pthread_join(bank[1], NULL);

    printf("Final amount in bank is %d Rs. \n",balance); // print final balance
    
    pthread_mutex_destroy(&mutex); // delete mutex

    return 0;
}