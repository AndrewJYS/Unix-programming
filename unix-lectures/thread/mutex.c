#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;

void * fun1(void * arg) {
    pthread_mutex_lock(&mutex);

    for (int i = 'A'; i < 'Z'; ++i) {
        putchar(i);
        fflush(stdout);
        usleep(100000); // 100ms
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void * fun2(void * arg) {
    pthread_mutex_lock(&mutex);

    for (int i = 'a'; i < 'z'; ++i) {
        putchar(i);
        fflush(stdout);
        usleep(50000); // 50ms
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    int ret = -1;
    
    //init mutex
    ret = pthread_mutex_init(&mutex, NULL);
    if (0 != ret) {
        printf("pthread_mutex_init failed\n");
        return 1;
    }
    printf("successfully init a mutex\n");

    //create two threads
    pthread_create(&tid1, NULL, fun1, NULL);
    pthread_create(&tid2, NULL, fun2, NULL);

    //wait for threads terminating
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // destroy mutex
    pthread_mutex_destroy(&mutex);
    putchar('\n');

    return 0;
}