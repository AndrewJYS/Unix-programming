#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int main()
{
    pid_t pid = -1;

    // create a child
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    // child process
    if (0 == pid) {
        while (1)
        {
            printf("child process works\n");
            sleep(1);
        }

        exit(0);
    }
    else { // parent process
        sleep(3);
        printf("\'kill\' the child\n");
        kill(pid, 15);
        printf("parent process ends\n");
    }

    return 0;
}