#include <stdio.h>
#include "mythreads.h"

#define NUMCPUS 4

typedef struct counter_t
{
    int global;
    pthread_mutex_t glock;
    int local[NUMCPUS];
    pthread_mutex_t llock[NUMCPUS];
    int threshold;
}counter_t;

void init(counter_t* c, int threshold) {
    c->threshold = threshold;

    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);

    int i;
    for (i = 0; i < NUMCPUS; i++) {
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}

void update(counter_t* c, int threadID, int amt) {
    Pthread_mutex_lock(&c->llock[threadID]);
    c->local[threadID] += amt;
    if (c->local[threadID] >= c->threshold) {
        Pthread_mutex_lock(&c->glock);
        c->global += c->local[threadID];
        Pthread_mutex_unlock(&c->glock);
        c->local[threadID] = 0;
    }
    Pthread_mutex_unlock(&c->llock[threadID]);
}

int get(counter_t* c) {
    Pthread_mutex_lock(&c->glock);
    int val = c->global;
    Pthread_mutex_unlock(&c->glock);
    return val;
}