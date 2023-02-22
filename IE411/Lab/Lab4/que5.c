#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
int main()
{
    key_t key1 = ftok("sh1file",61); //ftok generates unique key
    key_t key2 = ftok("sh2file",62); //ftok generates unique key
    int shmid1 = shmget(key1,1024,0666|IPC_CREAT); //shmget returns identifier in shmid
    int shmid2 = shmget(key2,1024,0666|IPC_CREAT); //shmget returns identifier in shmid
    printf("shmid1 = %d\n",shmid1); //print shmid
    printf("shmid2 = %d\n",shmid2); //print shmid
    shmctl(shmid1,IPC_RMID,NULL); //destroy shared memory
    shmctl(shmid2,IPC_RMID,NULL); //destroy shared memory
return 0;
}
