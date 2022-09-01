# thread  

## Thread Identification  

A thread ID is represented by the pthread_t data type. Implementations are allowed to use a structure to represent the pthread_t data type, so portable implementations **can’t treat them as integers**. Therefore, **a function must be used to compare two thread IDs**.  

```c
#include <pthread.h>

int pthread_equal(pthread_t tid1, pthread_t tid2);

// Returns: nonzero if equal, 0 otherwise
```

A thread can obtain its own thread ID by calling the pthread_self function.

```c
#include <pthread.h>

pthread_t pthread_self(void);

// Returns: the thread ID of the calling thread
```

example using pthread_equal and pthread_self  

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int main()
{
    pthread_t tid = 0;

    // get current thread id
    tid = pthread_self();

    printf("tid: %ld\n", tid);

    //compare two thread
    if (pthread_equal(tid, pthread_self())) {
        printf("two thread ids are the same\n");
    } else {
        printf("two thread ids are not the same\n");
    }

    return 0;
}
```

use following command to compile  

```shell
gcc mythread.c -pthread
```

## Thread Creation  

```c
#include <pthread.h>

int pthread_create(pthread_t *restrict tidp, 
                   const pthread_attr_t *restrict attr, 
                   void *(*start_rtn)(void *), 
                   void *restrict arg);

// Returns: 0 if OK, error number on failure
```

tidp：线程标识符地址  
attr：线程属性结构体地址。通常设置为NULL  
start_rtn：线程函数的入口地址  
arg：线程函数的参数  

下述例子中，测试了线程函数的调用，注意(int)(long)arg的强制转换  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* fun1(void* arg) {
    printf("new thread 1 tid: %lu\n", pthread_self());
    return NULL;
}

void* fun2(void* arg) {
    int var = (int)(long)arg;

    printf("new thread 2 var = %d\n", var);

    return NULL;
} 

int main()
{
    pthread_t tid = -1;
    int ret = -1;

    ret = pthread_create(&tid, NULL, fun1, NULL);
    if (0 != ret) {
        printf("pthread create failed\n");
        return 1;
    }

    ret = pthread_create(&tid, NULL, fun2, (void*)0x3);
    if (0 != ret) {
        printf("pthread create failed\n");
        return 1;
    }

    printf("main thread tid: %lu\n", pthread_self());

    printf("press any key to quit\n");
    getchar();

    return 0;
}
```

## Thread Termination  

If any thread within a process calls **exit, \_Exit, or \_exit**, then the entire process terminates. Similarly, when the default action is to terminate the process, a signal sent to a thread will **terminate the entire process**  

A single thread can exit in three ways, thereby stopping its flow of control, without terminating the entire process.  
1.The thread can simply return from the start routine. The return value is the thread’s exit code.  
2.The thread can be canceled by another thread in the same process.  
3.The thread can call pthread_exit.  

### pthread_exit  

```c
#include <pthread.h>

void pthread_exit(void *rval_ptr);
```

rval_ptr：存储线程退出状态的指针  

### pthread_cancel  

One thread can request that another **in the same process** be canceled by calling the pthread_cancel function.  

```c
#include <pthread.h>

int pthread_cancel(pthread_t tid);

// Returns: 0 if OK, error number on failure
```

线程的取消并不是实时的，可能有一定的延时，需要等待线程到达某个取消点（检查点）  

### pthread_join  

```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **rval_ptr);

// Returns: 0 if OK, error number on failure
```

等待线程结束（此函数会阻塞），并回收线程资源，如果该线程已经结束，那么该函数立即返回  

thread:被等待的线程号  
retval:用来存储线程退出状态的指针的地址  

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void* fun(void* arg) {
    printf("subthread is working\n");
    sleep(5); //可以观察到pthread_join确实阻塞了
    return NULL;
}

int main()
{
    int ret = -1;
    void* retp = NULL;
    pthread_t tid = -1;

    // create a thread
    ret = pthread_create(&tid, NULL, fun, NULL);
    if (0 != ret) {
        printf("pthread create failed\n");
        return 1;
    }

    printf("main thread is running\n");

    // wait for subthread terminating
    ret = pthread_join(tid, &retp);
    if (0 != ret) {
        printf("pthread join failed\n");
        return 1;
    }
    printf("retp: %p\n", &retp);
    printf("main thread exit...\n");

    return 0;
}
```

### pthread_detach  

```c
#include <pthread.h>

int pthread_detach(pthread_t thread);

// Returns 0 on success, or a positive error number on error
```

一旦线程被置为detach状态，当线程终止，就立刻回收它占用的资源，而不保留终止状态（如果**主进程**退出，那么线程也随之被销毁，因为所有的线程都共用进程的资源）。不能对一个已经处理detach状态的线程调用pthread_join，这样的调用会返回EINVAL错误。  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void* fun(void* arg) {
    for (int i = 0; i < 5; ++i) {
        printf("subthread is working: %d\n", i);
        sleep(1);
    }
}

int main()
{
    int ret = -1;
    pthread_t tid = -1;

    //create a subthread
    ret = pthread_create(&tid, NULL, fun, NULL);
    if (0 != ret) {
        printf("pthread create failed\n");
        return 1;
    }

    printf("main thread is working\n");

    //detach subthread
    ret = pthread_detach(tid);
    if (0 != ret) {
        printf("pthread detach failed\n");
        return 1;
    }

    printf("perss any key to quit\n");
    getchar();

    return 0;
}
```

## Thread Synchronization  

### mutexes  

```c
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);

// Both return: 0 if OK, error number on failure
```

To initialize a mutex with the default attributes, we set attr to NULL.  

```c
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

// All return: 0 if OK, error number on failure
```

To **lock a mutex**, we call **pthread_mutex_lock**. If the mutex is **already locked**, the calling **thread will block** until the mutex is unlocked. To **unlock a mutex**, we call **pthread_mutex_unlock**. If a thread can’t afford to block, it can use **pthread_mutex_trylock** to lock the mutex conditionally. If the **mutex is unlocked** at the time pthread_mutex_trylock is called, **then pthread_mutex_trylock will lock the mutex without blocking and return 0**. **Otherwise**, pthread_mutex_trylock will fail, **returning EBUSY without locking the mutex**.  

example printing upper and lower case respectively:  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;

void * fun1(void * arg) {
    pthread_mutex_lock(&mutex);

    for (int i = 'A'; i < 'Z'; ++i) {
        putchar(i);
        fflush(stdout);
        usleep(100000); // 100ms
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void * fun2(void * arg) {
    pthread_mutex_lock(&mutex);

    for (int i = 'a'; i < 'z'; ++i) {
        putchar(i);
        fflush(stdout);
        usleep(50000); // 50ms
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    int ret = -1;
    
    //init mutex
    ret = pthread_mutex_init(&mutex, NULL);
    if (0 != ret) {
        printf("pthread_mutex_init failes\n");
        return 1;
    }
    printf("successfully init a mutex\n");

    //create two threads
    pthread_create(&tid1, NULL, fun1, NULL);
    pthread_create(&tid2, NULL, fun2, NULL);

    //wait for threads terminating
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // destroy mutex
    pthread_mutex_destroy(&mutex);
    putchar('\n');

    return 0;
}
```

### reader-writer-lock  

读写锁有如下特点：  
1.如果有其他线程读数据，则允许其他线程执行读操作，但不允许写操作  
2.如果有其他线程写数据，则其他线程不能进行读，写操作  

读写锁分为读锁和写锁，规则如下：  
1.如果某线程申请了读锁，其他线程可以再申请读锁，但不能申请写锁  
2.如果某线程申请了写锁，其他线程不能申请读锁，也不能申请写锁  

```c
#include <pthread.h>

int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);

// Both return: 0 if OK, error number on failure
```

We can pass a **null pointer** for **attr** if we want the reader–writer lock to have the **default** attributes.  

```c
#include <pthread.h>

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

// All return: 0 if OK, error number on failure
```

```c
#include <pthread.h>

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);

Both return: 0 if OK, error number on failure
```

pthread_rwlock_rdlock()以阻塞方式在读写锁上获取读锁。如果没有写者持有该锁，并且没有写者阻塞在该锁上，则调用线程会获取读锁。如果调用线程未获取锁，则它将阻塞直到它获取锁。一个线程可以在一个读写锁上多次执行读锁定。线程可以成功调用pthread_rw_rdlock()函数n次，但是之后该线程必须调用n次pthread_rwlock_unlock()函数才能解除锁定  

pthread_rwlock_tryrdlock()用于尝试以非阻塞的方式在读写锁上获取读锁，如果有任何的写者持有该锁或有写者阻塞在该读写锁上，则立即失败返回  

pthread_rwlock_wrlock()在读写锁上获取写锁。如果没有写者，读者持有该锁，则调用线程会获取写锁。如果调用线程未获取写锁，则它将阻塞直到它获取了该锁  

pthread_rwlock_trywrlock()用于尝试以非阻塞的方式在读写锁上获取锁，如果有任何的读者或写者持有该锁，则立即失败返回  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// reader-wrtier-lock
pthread_rwlock_t rwlock;

// global var
int num = 0;

//read thread
void* fun_read(void* arg) {
    int index = (int)(long)arg;
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        printf("thread %d read num: %d\n", index, num);
        pthread_rwlock_unlock(&rwlock); //在睡眠前释放锁，否则写者几乎不可能拿到锁
        sleep(random() % 3 + 1);
    }
    return NULL;
}

// wrtie thread
void* fun_write(void* arg) {
    int index = (int)(long)arg;
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("thread %d write num: %d\n", index, num);
        pthread_rwlock_unlock(&rwlock);
        sleep(random() % 3 + 1);
    }
    return NULL;
}

int main()
{
    pthread_t tid[8];
    int ret = -1;

    // random seed
    srandom(getpid());

    //init reader-writer-lock
    ret = pthread_rwlock_init(&rwlock, NULL);
    if (0 != ret) {
        printf("pthread_rwlock_init failed\n");
        return 1;
    }

    // create 8 threads
    for (int i = 0; i < 8; ++i) {
        if (i < 5) {
            pthread_create(&tid[i], NULL, fun_read, (void* )(long)i);
        }
        else {
            pthread_create(&tid[i], NULL, fun_write, (void* )(long)i);
        }
    }

    // join
    for (int i = 0; i < 8; ++i) {
        pthread_join(tid[i], NULL);
    }

    // destroy lock
    pthread_rwlock_destroy(&rwlock);

    return 0;
}
```

## Condition Variables  

### condition variable basics

```c
#include <pthread.h>

int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
int pthread_cond_destroy(pthread_cond_t *cond);

// Both return: 0 if OK, error number on failure
```

Unless you need to create a conditional variable with nondefault attributes, the **attr** argument to pthread_cond_init can be set to **NULL**.  

```c
#include <pthread.h>
int pthread_cond_wait(pthread_cond_t *restrict cond, 
                      pthread_mutex_t *restrict mutex);

int pthread_cond_timedwait(pthread_cond_t *restrict cond, 
                           pthread_mutex_t *restrict mutex,
                           const struct timespec *restrict tsptr);

// Both return: 0 if OK, error number on failure
```

pthread_cond_wait()用来阻塞等待一个条件变量。(1)阻塞等条件变量cond满足(2)释放已掌握的互斥锁，相当于pthread_mutex_unlock(&mutex);(1)(2)两步是一个原子操作。(3)当被唤醒，pthread_cond_wait()函数返回时，解除阻塞并重新申请获取互斥锁pthread_mutex_lock(&mutex)  

pthread_cond_timedwait()限时等待一个条件变量，如果条件不满足，最多等待n分钟，时间到还没满足条件就返回出错  

```c
#include <pthread.h>

int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);

// Both return: 0 if OK, error number on failure
```

pthread_cond_signal()唤醒至少一个阻塞在条件变量上的线程  
pthread_cond_broadcast()唤醒所有阻塞在条件变量上的线程  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

int flags = 0;

void* fun1(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        flags = 1;
        pthread_mutex_unlock(&mutex);

        // wake
        pthread_cond_signal(&cond);
        printf("thread 1 has changes the cond\n");

        sleep(2);
    }
    return NULL;
}

void* fun2(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        if (0 == flags) {
            // wait for cond satisfying
            pthread_cond_wait(&cond, &mutex);
        }
        printf("flags = %d, thread 2 begins to work\n", flags);
        flags = 0;

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    int ret = -1;
    pthread_t tid1, tid2;

    //init
    ret = pthread_cond_init(&cond, NULL);
    if (0 != ret) {
        printf("pthread_cond_init failed\n");
        return 1;
    }
    ret = pthread_mutex_init(&mutex, NULL);
    if (0 != ret) {
        printf("pthread_mutex_init failed\n");
        return 1;
    }

    //create two threads
    pthread_create(&tid1, NULL, fun1, NULL);
    pthread_create(&tid2, NULL, fun2, NULL);

    // join
    ret = pthread_join(tid1, NULL);
    if (0 != ret) {
        printf("pthread_join failed\n");
        return 1;
    }
    ret = pthread_join(tid2, NULL);
    if (0 != ret) {
        printf("pthread_join failed\n");
        return 1;
    }

    //destroy
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return 0;
}
```

### 用条件变量解决生产者消费者问题  

```c
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
```

## Semaphores  

### POSIX Semaphores  

To **create** an unnamed **semaphore**, we call the **sem_init** function.  

```c
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value);

// Returns: 0 if OK, −1 on error
```

The **pshared** argument indicates if we plan to use the semaphore with multiple processes. If so, we set it to a nonzero value(**if we use it inter-thread-wise, then set it 0**). The **value** argument specifies the **initial value of the semaphore**.  

When we are done using the unnamed semaphore, we can **discard** it by calling the **sem_destroy** function.  

```c
#include <semaphore.h>

int sem_destroy(sem_t *sem);

// Returns: 0 if OK, −1 on error
```

To **decrement the value of a semaphore**, we can use either the **sem_wait or sem_trywait** function.  

```c
#include <semaphore.h>

int sem_trywait(sem_t *sem);
int sem_wait(sem_t *sem);

// Both return: 0 if OK, −1 on error
```

With the **sem_wait** function, we will **block** if the **semaphore count is 0**. We won’t return until we have successfully decremented the semaphore count or are interrupted by a signal. We can **use the sem_trywait function to avoid blocking**. If the semaphore count is zero when we call sem_trywait, it will return −1 with errno set to EAGAIN instead of blocking. A third alternative is to block for a bounded amount of time. We can use the sem_timedwait function for this purpose.  

```c
#include <semaphore.h>
#include <time.h>

int sem_timedwait(sem_t *restrict sem, const struct timespec *restrict tsptr);

// Returns: 0 if OK, −1 on error
```

To **increment the value of a semaphore**, we call the **sem_post** function.  

```c
#include <semaphore.h>

int sem_post(sem_t *sem);

// Returns: 0 if OK, −1 on error
```

One other function is available to allow us to **retrieve the value of a semaphore**. We call the **sem_getvalue** function for this purpose.  

```c
#include <semaphore.h>

int sem_getvalue(sem_t *restrict sem, int *restrict valp);

// Returns: 0 if OK, −1 on error
```

On success, the integer pointed to by the valp argument will contain the value of the semaphore.  

example: we print upper and lower case respectively  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

sem_t sem;

pthread_mutex_t mutex;

void * fun1(void * arg) {
    sem_wait(&sem);

    for (int i = 'A'; i < 'Z'; ++i) {
        putchar(i);
        fflush(stdout);
        usleep(100000); // 100ms
    }

    sem_post(&sem);

    return NULL;
}

void * fun2(void * arg) {
    sem_wait(&sem);

    for (int i = 'a'; i < 'z'; ++i) {
        putchar(i);
        fflush(stdout);
        usleep(50000); // 50ms
    }

    sem_post(&sem);

    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    int ret = -1;
    
    //init semaphore
    ret = sem_init(&sem, 0, 1);
    if (0 != ret) {
        printf("sem_init failed\n");
        return 1;
    }
    printf("successfully init semaphore\n");

    //create two threads
    pthread_create(&tid1, NULL, fun1, NULL);
    pthread_create(&tid2, NULL, fun2, NULL);

    //wait for threads terminating
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    //destroy semaphore
    sem_destroy(&sem);

    return 0;
}
```

### producer-consumer problem with semaphore  

```c
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
```

## references  

Advanced Programming in the UNIX Environment, 3rd edition, Chapter 11, 15  
The Linux programming interface a Linux and UNIX system programming handbook, 1st edition, Chapter 29  
[相关课程](https://www.bilibili.com/video/BV1Yo4y1D7Ap)  
