#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
int number;
int sum = 0;
int save_stdin = dup(0);
int input_fds = open("numbers.txt", O_RDONLY);
//printf("%d %d \n", save_stdin, input_fds);
if(dup2(input_fds, 0) < 0) {
    printf("Unable to duplicate file descriptor.");
    exit(EXIT_FAILURE);
}
while(scanf("%d", &number)==1){
    sum=sum+number;
}
printf("Sum = %d\n",sum);
close(input_fds);
return EXIT_SUCCESS;
}