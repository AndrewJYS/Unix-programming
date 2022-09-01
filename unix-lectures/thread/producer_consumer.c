#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

//define node (products)
typedef struct _node_t {
    int val;
    struct _node_t* next;
} node_t;

node_t* head = NULL;

//condtion_variable
pthread_cond_t cond;

//mutex
pthread_mutex_t mutex;

void* producer(void* arg) {
    // produce in a loop
    while (1) {
        //lock
        pthread_mutex_lock(&mutex);

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

        //unlock
        pthread_mutex_unlock(&mutex);

        //wake thread blocked by condition variable
        pthread_cond_signal(&cond);
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    node_t* tmp = NULL;

    //consume in a loop
    while (1)  {
        //lock
        pthread_mutex_lock(&mutex);

        if (NULL == head) {
            // wait
            printf("list is empty, sleep\n");
            pthread_cond_wait(&cond, &mutex);
        }

        //delete the first node
        tmp = head;
        head = head->next;
        printf("consumer consume product %d\n", tmp->val);

        //release space
        free(tmp);

        //unlock
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() 
{
    int ret = -1;
    pthread_t tid1, tid2, tid3;

    // init condition variable and mutex
    ret = pthread_cond_init(&cond, NULL);
    if (0 != ret) {
        printf("pthread_connd_init failed\n");
        return 1;
    }
    ret = pthread_mutex_init(&mutex, NULL);
    if (0 != ret) {
        printf("pthread_mutex_init failed\n");
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
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return 0;
}