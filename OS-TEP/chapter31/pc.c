//producer-consumer
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include "mythreads.h"

#define MAX 50

sem_t empty;
sem_t full;
sem_t mutex;

int buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;

int loops;
int consumers = 1;
int producers = 1;

void put(int value) {
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % MAX;
}

int get() {
    int tmp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % MAX;
    return tmp;
}

void* producer(void* arg) {
    int i;
    int pid = (int)(long long)arg;
    for (i = 0; i < (int)(loops / producers); i++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);
        printf("producer %d: %d, fill_ptr: %d\n", pid, i, fill_ptr);
        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void* consumer(void* arg) {
    int i;
    int cid = (int)(long long)arg;
    for (i = 0; i < (int)(loops / consumers); i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        int tmp = get();
        printf("consumer %d: %d, use_ptr: %d\n", cid, i, use_ptr);
        sem_post(&mutex);
        sem_post(&empty);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <loops> < producers> <consumers>\n", argv[0]);
        exit(1);
    }
    loops = atoi(argv[1]);
    producers = atoi(argv[2]);
    consumers = atoi(argv[3]);

    int i;
    for (i = 0; i < MAX; i++) {
	    buffer[i] = -1;
    }

    sem_init(&empty, 0, MAX);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t pid[producers], cid[consumers];
    for (int i = 0; i < producers; i++) 
        Pthread_create(&pid[i], NULL, producer, (void* )(long long)i);
    for (int i = 0; i < consumers; i++) 
        Pthread_create(&cid[i], NULL, consumer, (void* )(long long)i);
    for (int i = 0; i < producers; i++) 
        Pthread_join(pid[i], NULL);
    for (int i = 0; i < consumers; i++) 
        Pthread_join(cid[i], NULL);

    return 0;
}