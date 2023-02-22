#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int i, nchildren = 0;
    pid_t pid;
    for (i = 1; i <= 2; i++) {
        pid = fork();
        if (pid > 0){
            nchildren++;
        }
    }
    printf("I had %d children\n", nchildren);
    return nchildren;
}