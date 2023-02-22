#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
int main(void) {
int number1;
int save_stdin = dup(0); /* save original fd */
int input_fds = open("text2.txt", O_RDONLY);
if(dup2(input_fds, 0) < 0) {
    printf("Unable to duplicate file descriptor.");
    exit(EXIT_FAILURE);
}
scanf("%d", &number1);
printf("%d",number1);
printf("\n");
return EXIT_SUCCESS;
}