//producer-consumer wrong code
#include "mythreads.h"
#include <stdio.h>
#include <assert.h>

int buffer;
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;

void put(int value) {
    assert(count == 0);
    count = 1;
    buffer = value;
}

int get() {
    assert(count == 1);
    count = 0;
    return buffer;
}

void* producer(void* arg) {
    int i;
    int loops = (int)(long long) arg;
    for (i = 0; i < loops; i++) {
        Pthread_mutex_lock(&mutex);
        if (count == 1)
            Pthread_cond_wait(&cond, &mutex);
        put(i);
        Pthread_cond_signal(&cond);
        Pthread_mutex_unlock(&mutex);
    }
}

void* consumer(void* arg) {
    int i;
    int loops = (int)(long long) arg;
    for (i = 0; i < loops; i++) {
        Pthread_mutex_lock(&mutex);
        if (count == 0)
            Pthread_cond_wait(&cond, &mutex);
        int tmp = get();
        Pthread_cond_signal(&cond);
        Pthread_mutex_unlock(&mutex);
        printf("%d\n", tmp);
    }
}

int main(int argc, char* argv[]) {
    pthread_t p1, c1, c2;
    Pthread_create(&p1, NULL, producer, (void* )2);
    Pthread_create(&c1, NULL, consumer, (void* )1);
    Pthread_create(&c2, NULL, consumer, (void* )1);
    Pthread_join(p1, NULL);
    Pthread_join(c1, NULL);
    Pthread_join(c2, NULL);
    return 0;
}