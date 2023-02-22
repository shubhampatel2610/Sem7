#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main(){
    int a=1, b=2, c=3;
    c+=a+=b+=c*2;

    printf("%d\n",a);
    printf("%d\n",b);
    printf("%d\n",c);

    if(fork() == 0){
        a+=b+=c+=3;
        printf("%d\n",a);
        printf("%d\n",b);
        printf("%d\n",c);
        c++;
    }else{
        b*=a*=c+=3;
        printf("%d\n",a);
        printf("%d\n",b);
        printf("%d\n",c);
        c--;
    }
    c+=a+=b+1;
    printf("%d\n",c);
}