#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
int main()
{
    key_t key = ftok("shmfile",65); //ftok generates unique key
    int shmid = shmget(key,1024,0666|IPC_CREAT); //shmget returns identifier in shmid
    printf("shmid = %d \n",shmid); //print shmid
    char *str = (char*) shmat(shmid,(void*)0,0); //shmat attach to shared memory
    shmdt(str); //detach shared memory
    shmctl(shmid,IPC_RMID,NULL); //destroy shared memory
return 0;
}
