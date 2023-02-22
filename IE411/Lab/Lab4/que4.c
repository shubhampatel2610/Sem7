#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
int main()
{
    key_t k= ftok("shmfile",65); //ftok generates unique key
    int shmid = shmget((key_t)101,69,0666|IPC_CREAT); //shmget returns identifier in shmid
    char str=(char) shmat(shmid,NULL,0); //shmat attach to shared memory
    struct shmid_ds buf;
    shmdt(str); //detach shared memory
    shmctl(shmid, IPC_STAT, &buf);
    int length = (int) buf.shm_segsz ;
    shmctl(shmid,IPC_RMID,NULL); //destroy shared memory
    printf("size of shared memory is : %d\n",length);
return 0;
}
