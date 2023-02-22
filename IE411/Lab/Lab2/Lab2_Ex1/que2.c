#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h> 
int main()
{
char c[10]; //10 characters(string)
int fd=open("text1.txt",O_RDONLY); //file descriptor

if(fd==-1) //if file not found
{
    printf("Unable to open file");
}else{ //if file found
    read(fd, &c, sizeof(c)); //reading characters from given file
    printf("%s",c); //print all characters(string)
    printf("\n");
    close(fd); //closing file
}
return 0;
}