#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("hello world\n");
    
    char *args[] = {"ls", "-l", "/home", NULL};
    execv("/bin/ls", args);

    printf("hello world\n");
    
    return 0;
}