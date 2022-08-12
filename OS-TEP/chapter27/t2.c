#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void* mythread(void* arg) {
    int m = (int)(long long) arg;
    printf("%d\n", m);
    return (void* ) (arg + 1);
}

int main(int argc, char* argv[]) {
    pthread_t p;
    int rc, m;
    rc = pthread_create(&p, NULL, mythread, (void* ) 100);
    assert(rc == 0);
    rc = pthread_join(p, (void** ) &m);
    assert(rc == 0);
    return 0;
}