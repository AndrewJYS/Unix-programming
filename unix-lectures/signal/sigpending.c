#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

//signal handler 1
void fun1(int signum) {
    printf("signal: %d\n", signum);
}

//signal handler 2
void fun2(int signum) {
    printf("signal: %d\n", signum);
}

int main()
{   
    int ret = -1;

    //signal set
    sigset_t set;
    sigset_t oset;

    //register signal
    // CTRL + C
    signal(SIGINT, fun1);
    // CTRL + '\'
    signal(SIGQUIT, fun2);

    printf("press any key to block siganl 2, 3\n");
    getchar();

    // block signal
    sigemptyset(&oset);
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    ret = sigprocmask(SIG_BLOCK, &set, &oset);
    if (-1 == ret) {
        perror("sigprocmask");
        return 1;
    }
    printf("successfully block signal 2, 3\n");

    /////////////////////////////////////
    // get pending signals
    printf("press enter key to get pending signal\n");
    getchar();

    sigemptyset(&set);
    ret = sigpending(&set);
    if (-1 == ret) {
        perror("sigpending");
        return 1;
    }
    for (int i = 1; i < 32; ++i) {
        if (sigismember(&set, i)) {
            printf("%d ", i);
        }
    }
    putchar('\n');
    /////////////////////////////////////

    printf("press any key to unblock signal 2, 3\n");
    getchar();

    // unblock signal
    ret = sigprocmask(SIG_SETMASK, &oset, NULL);
    if (-1 == ret) {
        perror("sigprocmask");
        return 1;
    }

    printf("press any key to quit\n");
    getchar();

    return 0;
}