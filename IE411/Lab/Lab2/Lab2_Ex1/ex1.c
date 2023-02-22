#include <stdio.h>
#include <unistd.h> //dup, dup2
#include <fcntl.h> //read, write
int main()
{

int fd;
char msg[50]="This is the first line of the file.";
fd=open("text1.txt",O_RDWR);

if(fd==-1)
{
    printf("Unable to open file.");
}
else{
    read(fd,msg,sizeof(msg));
    write(fd,msg,sizeof(msg));
    printf("\n");
    close(fd);
}
return 0;

}