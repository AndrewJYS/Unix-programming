#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

// void fun(int signum) {
//     pid_t pid;

//     while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
//         printf("child %d resources released\n", pid);
//     }
// }

int main()
{
    pid_t pid = -1;
    // struct sigaction act;
    // act.sa_handler = fun;
    // act.sa_flags = 0;
    
    // //register signal handler
    // sigaction(SIGCHLD, &act, NULL);

    signal(SIGCHLD, SIG_IGN);

    //create a child process
    pid =  fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    if (0 == pid) {
        //child process
        printf("child process will sleep for 2 secs\n");
        sleep(2);
        printf("after 2 secs, child exits\n");
        exit(0);
    }
    else {
        // parent
        while (1) {
            printf("parent is working\n");
            sleep(1);
        }
    }

    return 0;
}