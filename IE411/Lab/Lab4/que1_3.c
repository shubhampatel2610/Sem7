#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
int main()
{
    key_t key = ftok("shfile",78); //ftok generates unique key
    int shmid = shmget(key,2048,0666|IPC_CREAT); //shmget returns identifier in shmid
    char *str = (char*) shmat(shmid,(void*)0,0); //shmat attach to shared memory
    printf("Data read from memory: %s\n",str);
    shmdt(str); //detach shared memory
    shmctl(shmid,IPC_RMID,NULL); //destroy shared memory
return 0;
}
