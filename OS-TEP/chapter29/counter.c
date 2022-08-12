#include <stdio.h>
#include "mythreads.h"

typedef struct counter_t
{
    int value;
    pthread_mutex_t lock;
}counter_t;

void init(counter_t* c) {
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

void increment(counter_t* c) {
    Pthread_mutex_lock(&c->lock);
    c->value++;
    Pthread_mutex_unlock(&c->lock);
}

void decrement(counter_t* c) {
    Pthread_mutex_lock(&c->lock);
    c->value--;
    Pthread_mutex_unlock(&c->lock);
}

int get(counter_t* c) {
    Pthread_mutex_lock(&c->lock);
    int rc = c->value;
    Pthread_mutex_unlock(&c->lock);
    return rc;
}

void* mythread(void* arg) {
    counter_t* c = (counter_t* ) arg;
    for (int i = 0; i < 1e7; i++) {
        increment(c);
    }
    return NULL;
}

int main(void) {
    pthread_t p1, p2, p3, p4;
    counter_t c;
    init(&c);

    Pthread_create(&p1, NULL, mythread, &c);
    Pthread_create(&p2, NULL, mythread, &c);
    Pthread_create(&p3, NULL, mythread, &c);
    Pthread_create(&p4, NULL, mythread, &c);
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    Pthread_join(p3, NULL);
    Pthread_join(p4, NULL);

    printf("%d\n", c.value);
    return 0;
}
