#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = -1;

    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    // child process
    if (0 == pid) {
        for (int i = 0; i < 5; ++i) {
            printf("child do things %d\n", i);
            sleep(1);
        }
        printf("child exit\n");
        exit(0);
    }
    // int status = -1;
    // wait(&status);

    //parent process
    sleep(100);
    // when parent is sleeping, the exited child becomes a zombie
    // after parent sleeping, resources of chid will be collected by parent

    return 0;
}