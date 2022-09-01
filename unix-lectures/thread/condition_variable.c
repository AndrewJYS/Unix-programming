#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

int flags = 0;

void* fun1(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        flags = 1;
        pthread_mutex_unlock(&mutex);

        // wake
        pthread_cond_signal(&cond);
        printf("thread 1 has changes the cond\n");

        sleep(2);
    }
    return NULL;
}

void* fun2(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        if (0 == flags) {
            // wait for cond satisfying
            pthread_cond_wait(&cond, &mutex);
        }
        printf("flags = %d, thread 2 begins to work\n", flags);
        flags = 0;

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    int ret = -1;
    pthread_t tid1, tid2;

    //init
    ret = pthread_cond_init(&cond, NULL);
    if (0 != ret) {
        printf("pthread_cond_init failed\n");
        return 1;
    }
    ret = pthread_mutex_init(&mutex, NULL);
    if (0 != ret) {
        printf("pthread_mutex_init failed\n");
        return 1;
    }

    //create two threads
    pthread_create(&tid1, NULL, fun1, NULL);
    pthread_create(&tid2, NULL, fun2, NULL);

    // join
    ret = pthread_join(tid1, NULL);
    if (0 != ret) {
        printf("pthread_join failed\n");
        return 1;
    }
    ret = pthread_join(tid2, NULL);
    if (0 != ret) {
        printf("pthread_join failed\n");
        return 1;
    }

    //destroy
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return 0;
}