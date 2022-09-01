#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int main()
{
    pthread_t tid = 0;

    // get current thread id
    tid = pthread_self();

    printf("tid: %lu\n", tid);

    //compare two thread
    if (pthread_equal(tid, pthread_self())) {
        printf("two thread ids are the same\n");
    } else {
        printf("two thread ids are not the same\n");
    }

    return 0;
}