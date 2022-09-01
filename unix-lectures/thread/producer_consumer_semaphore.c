#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

//define node (products)
typedef struct _node_t {
    int val;
    struct _node_t* next;
} node_t;

node_t* head = NULL;

// semaphore
sem_t sem_producer; 
sem_t sem_consumer;

void* producer(void* arg) {
    node_t* new = NULL;

    // produce in a loop
    while (1) {
        sem_wait(&sem_producer);

        // malloc node
        node_t* new = malloc(sizeof(node_t));
        if (NULL == new) {
            printf("malloc failed\n");
            break;
        }
        memset(new, 0, sizeof(node_t));

        //produce and insert into list
        new->val = random() % 100 + 1;
        new->next = head;
        head = new;
        printf("produce product %d\n", new->val);

        sem_post(&sem_consumer);
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    node_t* tmp = NULL;

    //consume in a loop
    while (1)  {
        sem_wait(&sem_consumer);

        if (NULL == head) {
            // wait
            printf("list is empty, sleep\n");
        }

        //delete the first node
        tmp = head;
        head = head->next;
        printf("consumer consume product %d\n", tmp->val);

        //release space
        free(tmp);

        sem_post(&sem_producer);
    }

    return NULL;
}

int main() 
{
    int ret = -1;
    pthread_t tid1, tid2, tid3;

    //init semaphore
    ret = sem_init(&sem_consumer, 0, 0); // can buy 0 product at first
    if (0 != ret) {
        printf("sesm_init failed\n");
        return 1;
    }
    ret = sem_init(&sem_producer, 0, 4); // can produce 4 products at maximum
    if (0 != ret) {
        printf("sesm_init failed\n");
        return 1;
    }

    //create two threads
    pthread_create(&tid1, NULL, producer, NULL);
    pthread_create(&tid2, NULL, producer, NULL);
    pthread_create(&tid3, NULL, consumer, NULL);

    // join
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    //destroy
    sem_destroy(&sem_consumer);
    sem_destroy(&sem_producer);

    return 0;
}