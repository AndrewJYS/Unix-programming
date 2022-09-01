#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("hello world\n");
    execlp("/bin/ls", "ls", "-l", "/home", NULL);

    printf("hello world\n");
    
    return 0;
}