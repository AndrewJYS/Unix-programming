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
    //register signal

    // CTRL + C
    signal(SIGINT, fun1);

    // CTRL + '\'
    signal(SIGQUIT, fun2);

    while(1) {
        sleep(1);
    }

    return 0;
}