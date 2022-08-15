#include "apue.h"

int main(void) {
    pid_t pid;
    printf("hello\n");

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid > 0)
        sleep(2);
    
    exit(0);
}