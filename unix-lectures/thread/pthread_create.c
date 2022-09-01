#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* fun1(void* arg) {
    printf("new thread 1 tid: %lu\n", pthread_self());
    return NULL;
}

void* fun2(void* arg) {
    int var = (int)(long)arg;

    printf("new thread 2 var = %d\n", var);

    return NULL;
} 

int main()
{
    pthread_t tid = -1;
    int ret = -1;

    ret = pthread_create(&tid, NULL, fun1, NULL);
    if (0 != ret) {
        printf("pthread create failed\n");
        return 1;
    }

    ret = pthread_create(&tid, NULL, fun2, (void*)0x3);
    if (0 != ret) {
        printf("pthread create failed\n");
        return 1;
    }

    printf("main thread tid: %lu\n", pthread_self());

    printf("press any key to quit\n");
    getchar();

    return 0;
}