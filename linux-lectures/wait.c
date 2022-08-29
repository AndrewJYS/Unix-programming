#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int ret = -1;
    int status = -1;
    pid_t pid = -1;

    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    //child process
    if (0 == pid) {
        for (int i = 0; i < 15; ++i) {
            printf("child process %d do thing %d\n", getpid(), i + 1);
            sleep(1);
        }
        exit(10);
    }

    //parent process
    printf("parent waits for child exiting\n");
    ret = wait(&status);
    if (-1 == ret) {
        perror("wait");
        return 1;
    }
    printf("parent collect child's resources\n");

    if (WIFEXITED(status)) {
        printf("child exit status: %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
        // kill -9 (childpid)
        printf("child is killed by signal %d\n", WTERMSIG(status));
    }
    else if (WIFSTOPPED(status)) {
        // kill -l      -------kill command options
        // kill -19 childpid       -------stop child
        // kill -18 childpid       -------continue child
        printf("child is stopped bt signal %d\n", WSTOPSIG(status));
    }

    return 0;
}