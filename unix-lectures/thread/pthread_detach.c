#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void* fun(void* arg) {
    for (int i = 0; i < 5; ++i) {
        printf("subthread is working: %d\n", i);
        sleep(1);
    }
}

int main()
{
    int ret = -1;
    pthread_t tid = -1;

    //create a subthread
    ret = pthread_create(&tid, NULL, fun, NULL);
    if (0 != ret) {
        printf("pthread create failed\n");
        return 1;
    }

    printf("main thread is working\n");

    //detach subthread
    ret = pthread_detach(tid);
    if (0 != ret) {
        printf("pthread detach failed\n");
        return 1;
    }

    printf("perss any key to quit\n");
    getchar();

    return 0;
}