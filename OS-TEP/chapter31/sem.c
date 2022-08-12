#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include "mythreads.h"

sem_t s;

void* child(void* arg) {
    printf("child\n");
    sem_post(&s);
    return NULL;
}

int main(int argc, char* argv[]) {
    sem_init(&s, 0, 0);
    printf("parent: begin\n");
    pthread_t p;
    Pthread_create(&p, NULL, child, NULL);
    sem_wait(&s);
    printf("parent: end\n");
    return 0;
}