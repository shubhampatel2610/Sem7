#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    printf("_____PARENT PROCESS_____\n"); //parent process
    system("ifconfig"); //prints ip addresses

    return 0;
}