#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main()
{
    int i, n;
    for ( i=1; i<=n; i++ )
        fork();
}