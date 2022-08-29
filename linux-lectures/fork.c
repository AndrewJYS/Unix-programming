#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int g_cnt = 100;

int main()
{
    int var = 88;
    pid_t pid = -1;
    int* p = malloc(sizeof(int));
    if (NULL == p) {
        printf("malloc failed...\n");
        return 1;
    }
    //memset(p, 0, sizeof(p));
    *p = 200;

    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    if (0 == pid) {
        // child process
        sleep(1);
        printf("child process after parent process operation: g_cnt = %d, var = %d, *p = %d\n", g_cnt, var, *p);
        free(p);
    }
    else {
        // father process
        printf("parent process before operation: g_cnt = %d, var = %d, *p = %d\n", g_cnt, var, *p);
        g_cnt++;
        var++;
        (*p)++;
        printf("parent process after operation: g_cnt = %d, var = %d, *p = %d\n", g_cnt, var, *p);
        free(p);
    }

    return 0;
}