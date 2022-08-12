#include <stdio.h>
#include <assert.h>
#include <pthread.h>

static volatile int counter = 0;

void* mythread (void* arg) {
    printf("%s: begin\n", (char* ) arg);
    int i;
    for (i = 0; i < 1e7; i++) {
        counter++;
    }
    printf("%s: done\n", (char* ) arg);
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2;
    int rc;
    printf("main: begin (counter = %d)\n", counter);

    rc = pthread_create(&p1, NULL, mythread, "A"); 
    assert(rc == 0);
    rc = pthread_create(&p2, NULL, mythread, "B"); 
    assert(rc == 0);

    rc = pthread_join(p1, NULL);
    assert(rc == 0);
    rc = pthread_join(p2, NULL);
    assert(rc == 0);

    printf("main: end (counter = %d)\n", counter);
    return 0;
}