#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// reader-wrtier-lock
pthread_rwlock_t rwlock;

// global var
int num = 0;

//read thread
void* fun_read(void* arg) {
    int index = (int)(long)arg;
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        printf("thread %d read num: %d\n", index, num);
        pthread_rwlock_unlock(&rwlock);
        sleep(random() % 3 + 1);
    }
    return NULL;
}

// wrtie thread
void* fun_write(void* arg) {
    int index = (int)(long)arg;
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("thread %d write num: %d\n", index, num);
        pthread_rwlock_unlock(&rwlock);
        sleep(random() % 3 + 1);
    }
    return NULL;
}

int main()
{
    pthread_t tid[8];
    int ret = -1;

    // random seed
    srandom(getpid());

    //init reader-writer-lock
    ret = pthread_rwlock_init(&rwlock, NULL);
    if (0 != ret) {
        printf("pthread_rwlock_init failed\n");
        return 1;
    }

    // create 8 threads
    for (int i = 0; i < 8; ++i) {
        if (i < 5) {
            pthread_create(&tid[i], NULL, fun_read, (void* )(long)i);
        }
        else {
            pthread_create(&tid[i], NULL, fun_write, (void* )(long)i);
        }
    }

    // join
    for (int i = 0; i < 8; ++i) {
        pthread_join(tid[i], NULL);
    }

    // destroy lock
    pthread_rwlock_destroy(&rwlock);

    return 0;
}