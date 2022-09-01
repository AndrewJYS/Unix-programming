#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    pid_t pid = -1;
    int ret = -1;

    //1.create child, parent exit
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }
    if (pid > 0) {
        exit(0);
    }

    //2.create new session
    pid = setsid();
    if (-1 == pid) {
        perror("setsid");
        return 1;
    }

    //3.change current working directory
    ret = chdir("/");
    if (-1 == ret) {
        perror("chdir");
        return 1;
    }

    //4.set file umask
    umask(0);

    //5.lose file descriptor
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //6.execute daemon
    //output current time into txt.log
    while (1) {
        system("date >> /tmp/txt.log");
        sleep(10);
    }

    return 0;
}