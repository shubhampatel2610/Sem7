#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
    char append_msg[50]="This line is appended by the parent process.\n"; //append message from parent
    pid_t pid; //pid variable to save process id
    pid=fork(); //creates two processes and save process id in pid

    if(pid==0){ //child process
        open("newfile.txt",O_RDWR | O_CREAT, 0644); //create empty newfile.txt  
        exit(0);
    }else{ //parent process
        wait(NULL); //wait till child process is finish
        int fd=open("newfile.txt",O_APPEND | O_WRONLY); //file descriptor of newfile.txt
        if(fd!=-1){ //if file found
            write(fd,append_msg,50); //writing appended message in newfile.txt
            printf("\n");
            close(fd); //closing file
        }else{ //if file not found
            printf("No file found.\n");
        }
    }    
return 0;
}