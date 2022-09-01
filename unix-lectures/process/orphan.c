#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid = -1;

    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    //parent process
    if (pid > 0) {
        printf("parent sleep for 1s and exit\n");
        sleep(1);
        printf("parent exits\n");
        exit(0);
    }

    //child process
    //parent process exits, when child process does not exit
    //then child becomes an orphan process
    while (1)
    {
        printf("child process is working, ppid: %d\n", getppid());
        sleep(1);
    }

    return 0;    
}