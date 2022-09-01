#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void* fun(void* arg) {
    printf("subthread is working\n");
    sleep(5);
    return NULL;
}

int main()
{
    int ret = -1;
    void* retp = NULL;
    pthread_t tid = -1;

    // create a thread
    ret = pthread_create(&tid, NULL, fun, NULL);
    if (0 != ret) {
        printf("pthread create failed\n");
        return 1;
    }

    printf("main thread is running\n");

    // wait for subthread terminating
    ret = pthread_join(tid, &retp);
    if (0 != ret) {
        printf("pthread join failed\n");
        return 1;
    }
    printf("retp: %p\n", &retp);
    printf("main thread exit...\n");

    return 0;
}