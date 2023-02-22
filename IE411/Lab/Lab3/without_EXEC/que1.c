#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir("."); // returns a pointer of DIR type.

    if(fork()==0){
        printf("_____CHILD PROCESS_____\n"); //child process 
        if(dr){
            while ((de = readdir(dr)) != NULL){
                printf("%s\n", de->d_name); //prints name of files / directories
            }
            closedir(dr);  
        }else{
            printf("Could not open current directory" );
            exit(0);
        }
    }else{
        wait(NULL);
        printf("_____PARENT PROCESS_____\n"); //parent process
        system("ifconfig"); //prints ip addresses
    }    
    return 0;
}