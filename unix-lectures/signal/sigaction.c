#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void fun(int signum) {
    printf("get signal %d\n", signum);
}

int main()
{
    int ret = -1;

    struct sigaction act;
    act.sa_handler = fun;
    act.sa_flags = 0;

    // register signal
    ret = sigaction(SIGINT, &act, NULL);
    if (-1 == ret) {
        perror("sigaction");
        return 1;
    }
    printf("register signal successfully\n");

    printf("enter any key to continue\n");
    while (1) {
        getchar();
    }

    return 0;
}