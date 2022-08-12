//producer-consumer
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#include "mythreads.h"

#define MAX 50

int loops;
int buffer[MAX];

int fill_ptr = 0;
int use_ptr = 0;
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty  = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill  = PTHREAD_COND_INITIALIZER;

int consumers = 1;

void put(int value) {
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % MAX;
    count++;
}

int get() {
    int tmp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % MAX;
    count--;
    return tmp;
}

void* producer(void* arg) {
    int i;
    for (i = 0; i < loops; i++) { //produce #loops items
        Pthread_mutex_lock(&mutex);
        while (count == MAX)
            Pthread_cond_wait(&empty, &mutex);
        put(i);
        printf("produce: %d, count = %d, fill_ptr = %d\n", 
            i, count, fill_ptr);
        Pthread_cond_signal(&fill);
        Pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* consumer(void* arg) {
    int i;
    int consid = (int)(long long)arg;
    for (i = 0; i < (int)(loops/consumers); i++) {
        Pthread_mutex_lock(&mutex);
        while (count == 0)
            Pthread_cond_wait(&fill, &mutex);
        int tmp = get();
        printf("consume %d: %d, count = %d, use_ptr: %d\n", 
            consid, tmp, count, use_ptr);
        Pthread_cond_signal(&empty);
        Pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <loops> <consumers>\n", argv[0]);
        exit(1);
    }
    loops = atoi(argv[1]);
    consumers = atoi(argv[2]);

    int i;
    for (i = 0; i < MAX; i++) {
	    buffer[i] = -1;
    }

    pthread_t pid, cid[consumers];
    Pthread_create(&pid, NULL, producer, NULL); 
    for (i = 0; i < consumers; i++) {
	    Pthread_create(&cid[i], NULL, consumer, (void *) (long long) i); 
    }
    Pthread_join(pid, NULL); //produce #loops items
    for (i = 0; i < consumers; i++) {
	    Pthread_join(cid[i], NULL); 
    }
    return 0;
}
