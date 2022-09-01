#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

//signal handler
void fun(int signum) {
    printf("signum: %d\n", signum);
}

int main()
{
    int ret = -1;
    struct itimerval tmo;

    //first time to set
    //there are 3 secs until the timer is to expire
    tmo.it_value.tv_sec = 3;
    tmo.it_value.tv_usec = 0;

    // period: 2 secs, 
    // after each expiration of the timer, 
    // the timer will be reset to expire again at 2 secs
    tmo.it_interval.tv_sec = 2;
    tmo.it_interval.tv_usec = 0;

    //catch signal SIGALRM
    signal(SIGALRM, fun);
    
    //set timer
    ret = setitimer(ITIMER_REAL, &tmo, NULL);
    if (-1 == ret) {
        perror("setitimer");
        return 1;
    }

    printf("please enter any key to continue\n");
    getchar();

    return 0;
}