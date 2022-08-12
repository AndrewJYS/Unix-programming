#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct myarg_t {
    int a;
    int b;
}myarg_t;

typedef struct myret_t {
    int x;
    int y;
}myret_t;

void* mythread(void* arg) {
    myarg_t* m = (myarg_t* ) arg;
    printf("%d %d\n", m->a, m->b);
    myret_t* r = malloc(sizeof(myret_t));
    r->x = 1;
    r->y = 2;
    return (void* ) r;
}

int main(int argc, char* argv[]) {
    pthread_t p;
    int rc;
    myret_t* m;

    myarg_t args;
    args.a = 10;
    args.b = 20;
    rc = pthread_create(&p, NULL, mythread, &args);
    assert(rc == 0);
    rc = pthread_join(p, (void** ) &m);
    assert(rc == 0);
    printf("returned %d %d\n", m->x, m->y);
    
    return 0;
}
