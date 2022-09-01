#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

sem_t sem;

pthread_mutex_t mutex;

void * fun1(void * arg) {
    sem_wait(&sem);

    for (int i = 'A'; i < 'Z'; ++i) {
        putchar(i);
        fflush(stdout);
        usleep(100000); // 100ms
    }

    sem_post(&sem);

    return NULL;
}

void * fun2(void * arg) {
    sem_wait(&sem);

    for (int i = 'a'; i < 'z'; ++i) {
        putchar(i);
        fflush(stdout);
        usleep(50000); // 50ms
    }

    sem_post(&sem);

    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    int ret = -1;
    
    //init semaphore
    ret = sem_init(&sem, 0, 1);
    if (0 != ret) {
        printf("sem_init failed\n");
        return 1;
    }
    printf("successfully init semaphore\n");

    //create two threads
    pthread_create(&tid1, NULL, fun1, NULL);
    pthread_create(&tid2, NULL, fun2, NULL);

    //wait for threads terminating
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    //destroy semaphore
    sem_destroy(&sem);

    return 0;
}