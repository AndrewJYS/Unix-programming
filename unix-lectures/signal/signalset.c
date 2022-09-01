#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

void show_signal_set(sigset_t* s) {
    for (int i = 1; i < 32; ++i) {
        if (sigismember(s, i)) {
            printf("1");
        }
        else {
            printf("0");
        }
    }
    putchar('\n');
}

int main()
{
    sigset_t set;

    //empty the signal set
    sigemptyset(&set);
    show_signal_set(&set);

    //include all signals
    sigfillset(&set);
    show_signal_set(&set);
    
    //delete signals
    sigdelset(&set, 9);
    sigdelset(&set, 14);
    show_signal_set(&set);

    //add signal
    sigaddset(&set, SIGKILL);
    sigaddset(&set, SIGALRM);
    show_signal_set(&set);

    return 0;
}