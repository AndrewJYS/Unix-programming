#include <pthread.h>
#include <assert.h>
#include <stdio.h>

typedef struct myarg_t
{
    int a;
    int b;
}myarg_t;

void* mythread(void* arg) {
    myarg_t* m = (myarg_t* ) arg;
    printf("%d %d\n", m->a, m->b);
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t p;
    int rc;

    myarg_t args;
    args.a = 10;
    args.b = 20;
    rc = pthread_create(&p, NULL, mythread, &args);
    assert(rc == 0);
    rc = pthread_join(p, NULL);
    assert(rc == 0);
    
    return 0;
}
