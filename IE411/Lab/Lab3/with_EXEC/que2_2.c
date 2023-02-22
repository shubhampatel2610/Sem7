#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
    char append_msg[50]="This line is appended by the parent process.\n"; //append message
    
        int fd=open("newfile.txt",O_APPEND | O_WRONLY); //file descripto of newfile.txt
        if(fd==-1){ //if file not found
             printf("No file found.\n");
        }else{ //if file found
            write(fd,append_msg,50); //writing append message in newfile.txt 
            printf("\n");
            close(fd);            
        }      
return 0;
}