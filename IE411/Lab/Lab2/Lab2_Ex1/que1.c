#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{

char append_msg[25]="This is appended line.\n"; //message that we want to append
int fd=open("text1.txt",O_APPEND | O_WRONLY); //file descriptor

if(fd==-1) //if file not found
{
    printf("Unable to open file\n");
}
else{ //if file found
    //read(fd,append_msg,25); //reading message
    write(fd,append_msg,25); //writing message
    printf("\n");
    close(fd); //closing file
}
return 0;

}