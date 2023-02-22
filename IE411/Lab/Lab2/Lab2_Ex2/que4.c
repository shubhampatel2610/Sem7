#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int pid;
    float a, b, c;
    pid = fork();
    if(pid == 0){
        a=1+1;
    }
    else{
        wait();
    }
    b=2+2;
    c=a+b;
    printf("c=%f\n", c);
return 0;
}