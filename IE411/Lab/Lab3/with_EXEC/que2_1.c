#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
    if(fork()==0){ //child process
        open("newfile.txt",O_RDWR | O_CREAT, 0644); //creates empty newfile.txt
        exit(0);
    }else{
        wait(NULL); //wait till child process is finish
        execvp("./que2_2",NULL,NULL); //exec for next task of parent process
    }    
return 0;
}