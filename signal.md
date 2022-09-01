# 信号  

## 信号相关属性查看  

信号的四要素包括：名称，编号，事件，默认动作

使用下述命令可以查看信号编号和名称  

```shell
kill -l
```

使用下述命令可以查看信号更详细的内容  

```shell
man 7 signal
```

## 信号产生函数  

### kill  

```c
#include <signal.h>

int kill(pid_t pid, int signo);

//return: 0 if OK, −1 on error
```

The kill function **sends a signal** to a process or a group of processes.  

There are **four** different conditions for the **pid argument** to kill.  
**pid >0**：The signal is sent to the process whose process ID is pid.  
**pid ==0**：The signal is sent to all processes whose process group ID equals the process group ID of the sender and for which the sender has permission to send the signal. Note that the term all processes excludes an implementation-defined set of system processes. For most UNIX systems, this set of system processes includes the kernel processes and init (pid 1).  
**pid <0**：The signal is sent to all processes whose process group ID equals the absolute value of pid and for which the sender has permission to send the signal. Again, the set of all processes excludes certain system processes, as described earlier.  
**pid == −1**：The signal is sent to all processes on the system for which the sender has permission to send the signal. As before, the set of processes excludes certain system processes.  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int main()
{
    pid_t pid = -1;

    // create a child
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    // child process
    if (0 == pid) {
        while (1)
        {
            printf("child process works\n");
            sleep(1);
        }

        exit(0);
    }
    else { // parent process
        sleep(3);
        printf("\'kill\' the child\n");
        kill(pid, 15);
        printf("parent process ends\n");
    }

    return 0;
}
```

### raise  

```c
#include <signal.h>

int raise(int signo);

//return: 0 if OK, −1 on error
```

The call  

```c
raise(signo);
```

is equivalent to the call  

```c
kill(getpid(), signo);
```

### abort  

```c
#include <stdlib.h>

void abort(void);

//This function never returns
```

This function **sends the SIGABRT signal to the caller**.  

### alarm  

```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);

//Returns: 0 or number of seconds until previously set alarm
```

The alarm function allows us to set a timer that will expire at a specified time in the future. When the timer expires, the **SIGALRM** signal is generated. If we ignore or don’t catch this signal, its **default action is to terminate the process**.  

The seconds value is the number of clock seconds in the future when the signal should be generated.  

There is **only one of these alarm clocks per process**. If, when we call alarm, **a previously registered alarm clock for the process has not yet expired**, the number of seconds left for that alarm clock is returned as the value of this function. **That previously registered alarm clock is replaced by the new value**.  

**定时，与进程状态无关**（自然定时法）。就绪，运行，挂起（阻塞，暂停），终止，僵尸态，无论进程处于何种状态，alarm都计时  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    unsigned int ret = 0;

    // set alarm first time
    ret = alarm(5);
    printf("ret: %d\n", ret);
    
    sleep(3);

    // set alarm second time
    ret = alarm(4);
    printf("ret: %d\n", ret);
    
    printf("please enter something\n");
    getchar();

    return 0;
}
```

if we don't enter anything, the process will be terminated  

```shell
jys@ubuntu:~/unix-programming/course/signal$ ./a.out 
ret: 0
ret: 2
please enter something
Alarm clock
```

### setitimer  

```c
#include <sys/time.h>

int setitimer(int which, const struct itimerval *new_value,
struct itimerval *old_value);

Returns 0 on success, or –1 on error
```

本函数可以替换alarm()，精度为微妙，可以实现周期定时  

a process can establish three different types of timers, by specifying **which** as one of the following:  
**ITIMER_REAL**:  Create a **timer that counts down in real** (i.e., wall clock) time. When the timer expires, a **SIGALRM signal is generated** for the process.  
**ITIMER_VIRTUAL**:  Create a timer that **counts down in process virtual time** (i.e., **user-mode CPU time**). When the timer expires, a **SIGVTALRM** signal is generated for the process.  
**ITIMER_PROF**:  Create a profiling timer. A profiling timer **counts in process time** (i.e., **the sum of both user-mode and kernel-mode CPU time**). When the timer expires, a **SIGPROF** signal is generated for the process.  

The **new_value** and **old_value** arguments are pointers to itimerval structures, defined as follows:  

```c
struct itimerval {
    struct timeval it_interval;  /* Interval for periodic timer */
    struct timeval it_value;     /* Current value (time until next expiration) */
};
```

Each of the fields in the itimerval structure is in turn a structure of type timeval, containing seconds and microseconds (**μs**) fields:  

```c
struct timeval {
    time_t tv_sec; /* Seconds */
    suseconds_t tv_usec; /* Microseconds (long int) */
};
```

The **it_value** substructure of the **new_value** argument specifies the **delay until the timer is to expire**. The **it_interval** substructure specifies **whether this is to be a periodic timer**. If both fields of it_interval are set to **0**, then the timer is **expires just once**, at the time given by it_value. If one or both of the it_interval fields are **nonzero**, then, **after each expiration of the timer, the timer will be reset** to expire again at the specified interval.  

program below will **terminate after 3 secs**, because when time expires, signal **SIGALARM** will terminate it.  

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int ret = -1;
    struct itimerval tmo;

    //first time to set
    //there are 3 secs until the timer is to expire
    tmo.it_value.tv_sec = 3;
    tmo.it_value.tv_usec = 0;

    // period: 2 secs, 
    // after each expiration of the timer, 
    // the timer will be reset to expire again at 2 secs
    tmo.it_interval.tv_sec = 2;
    tmo.it_interval.tv_usec = 0;
    
    //set timer
    ret = setitimer(ITIMER_REAL, &tmo, NULL);
    if (-1 == ret) {
        perror("setitimer");
        return 1;
    }

    printf("please enter any key to continue\n");
    getchar();

    return 0;
}
```

## 信号捕捉  

### 信号处理方式  

1.执行系统默认操作：对于大多数信号来说，系统默认操作是终止该进程  
2.忽略该信号：接收到此信号后没有任何动作  
3.执行自定义信号处理函数：用用户定义的信号处理函数处理该信号  
注意：SIGKILL和SIGSTOP不能忽略，阻塞或捕捉。不能更改它们的处理方式，原因是它们向用户提供了一种进程终止的可靠方法  

### signal（可以弃用）  

```c
#include <signal.h>

void (*signal(int signo, void (*func)(int)))(int);

//Returns: previous disposition of signal (see following) if OK, SIG_ERR on error
```

功能：注册信号处理函数(不可用于SIGKILL, SIGSTOP信号)，即确定收到信号后处理函数的入口地址，此函数不会阻塞  

example for signal:  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

//signal handler 1
void fun1(int signum) {
    printf("signal: %d\n", signum);
}

//signal handler 2
void fun2(int signum) {
    printf("signal: %d\n", signum);
}

int main()
{   
    //register signal handler

    // CTRL + C
    signal(SIGINT, fun1);

    // CTRL + '\'
    signal(SIGQUIT, fun2);

    while(1) {
        sleep(1);
    }

    return 0;
}
```

example for signal and setitimer: at this time, the program will not terminate after 3 secs, because the signal handler will deal with the signal SIGALRM on its defined way.  

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

//signal handler
void fun(int signum) {
    printf("signum: %d\n", signum);
}

int main()
{
    int ret = -1;
    struct itimerval tmo;

    //first time to set
    //there are 3 secs until the timer is to expire
    tmo.it_value.tv_sec = 3;
    tmo.it_value.tv_usec = 0;

    // period: 2 secs, 
    // after each expiration of the timer, 
    // the timer will be reset to expire again at 2 secs
    tmo.it_interval.tv_sec = 2;
    tmo.it_interval.tv_usec = 0;

    //catch signal SIGALRM
    signal(SIGALRM, fun);
    
    //set timer
    ret = setitimer(ITIMER_REAL, &tmo, NULL);
    if (-1 == ret) {
        perror("setitimer");
        return 1;
    }

    printf("please enter any key to continue\n");
    getchar();

    return 0;
}
```

UNIX systems provide **two ways of changing the disposition of a signal: signal() and sigaction()**. The signal() system call, which is described in this section, was the original API for setting the disposition of a signal, and it provides a simpler interface than sigaction(). On the other hand, sigaction() provides functionality that is not available with signal(). Furthermore, **there are variations in the behavior of signal() across UNIX implementations (Section 22.7), which mean that it should never be used for establishing signal handlers in portable programs**. Because of these portability issues, **sigaction() is the (strongly) preferred API for establishing a signal handler**. After we explain the use of sigaction() in Section 20.13, we’ll always employ that call when establishing signal handlers in our example programs.  

### sigaction  

```c
#include <signal.h>

int sigaction(int sig, const struct sigaction *act, struct sigaction *oldact);

//Returns 0 on success, or –1 on error
```

The **sig** argument identifies the signal whose disposition we want to retrieve or change. This argument can be **any signal except SIGKILL or SIGSTOP**.  

The **act** argument is a pointer to a structure specifying a **new disposition for the signal**. If we are **interested only in finding the existing disposition** of the signal, then we can **specify NULL** for this argument. The **oldact** argument is a pointer to a structure of the same type, and is used to return information about the **signal’s previous disposition**. If we are **not interested** in this information, then we can **specify NULL** for this argument. The structures pointed to by **act and oldact are of the following type**:  

```c
struct sigaction {
    void (*sa_handler)(int);    /* addr of signal handler, */
                                /* or SIG_IGN, or SIG_DFL */
    sigset_t sa_mask;           /* additional signals to block */
    int sa_flags;               /* signal options, Figure 10.16 */
    
    /* alternate handler */
    void (*sa_sigaction)(int, siginfo_t *, void *);
};
```

When changing the action for a signal, if the **sa_handler field contains the address of a signal-catching function** (as opposed to either of the constants SIG_IGN or SIG_DFL), then the **sa_mask** field specifies a set of **signals** that are **added to the signal mask of the process before the signal-catching function is called**. If and when the **signal-catching function returns, the signal mask of the process is reset to its previous value**.  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void fun(int signum) {
    printf("get signal %d\n", signum);
}

int main()
{
    int ret = -1;

    struct sigaction act;
    act.sa_handler = fun;
    act.sa_flags = 0;

    // register signal handler
    ret = sigaction(SIGINT, &act, NULL);
    if (-1 == ret) {
        perror("sigaction");
        return 1;
    }
    printf("register signal successfully\n");

    printf("enter any key to continue\n");
    while (1) {
        getchar();
    }

    return 0;
}
```

Normally, the signal handler is called as  

```c
void handler(int signo);
```

but if the **SA_SIGINFO** flag is set, the signal handler is called as  

```c
void handler(int signo, siginfo_t *info, void *context);
```

## 信号集  

在PCB中，有两个非常重要的信号集，阻塞信号集和未决信号集  

### 信号集处理函数  

```c
#include <signal.h>

int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signo);
int sigdelset(sigset_t *set, int signo);
//All four return: 0 if OK, −1 on error

int sigismember(const sigset_t *set, int signo);
//Returns: 1 if true, 0 if false, −1 on error
```

The function **sigemptyset** initializes the signal set pointed to by set so that **all signals are excluded**.  
The function **sigfillset** initializes the signal set so that **all signals are included**.  
The function **sigaddset adds a single signal** to an existing set  
**sigdelset removes a single signal** from a set.  

All applications have to **call either sigemptyset or sigfillset once for each signal set**, before using the signal set, because we cannot assume that the C initialization for external and static variables (0) corresponds to the implementation of signal sets on a given system.  

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

void show_signal_set(sigset_t* s) {
    for (int i = 1; i < 32; ++i) {
        if (sigismember(s, i)) {
            printf("1");
        }
        else {
            printf("0");
        }
    }
    putchar('\n');
}

int main()
{
    sigset_t set;

    //empty the signal set
    sigemptyset(&set);
    show_signal_set(&set);

    //include all signals
    sigfillset(&set);
    show_signal_set(&set);
    
    //delete signals
    sigdelset(&set, 9);
    sigdelset(&set, 14);
    show_signal_set(&set);

    //add signal
    sigaddset(&set, SIGKILL);
    sigaddset(&set, SIGALRM);
    show_signal_set(&set);

    return 0;
}
```

note the loop in show_signal_set()  

```c
for (int i = 1; i < 32; ++i) {  // 1 <= i < 32
```

output:  

```shell
0000000000000000000000000000000
1111111111111111111111111111111
1111111101111011111111111111111
1111111111111111111111111111111
```

### sigprocmask  

信号阻塞集也称为信号屏蔽集或信号掩码。每个进程都有一个信号阻塞集。创建子进程时，子进程将继承父进程的阻塞集。阻塞并不是禁止信号传送，而是暂缓信号传送。若将被阻塞的信号从信号阻塞集中删除，且对应的信号在被阻塞时发生了，进程将会收到相应的信号  

A process can examine its signal mask, change its signal mask, or perform both operations in one step by calling the following function.

```c
#include <signal.h>

int sigprocmask(int how, const sigset_t *restrict set,
sigset_t *restrict oset);

//Returns: 0 if OK, −1 on error
```

First, if **oset** is a non-null pointer, the current signal mask for the process is returned through oset.  
Second, if **set** is a non-null pointer, the how argument indicates how the current signal mask is modified.  
Table below describes the possible values for **how**.
SIG_BLOCK is an inclusive-OR operation, whereas SIG_SETMASK is an assignment.  
Note that **SIGKILL and SIGSTOP can’t be blocked**.  

|how |Description|
:-:|:-:
|SIG_BLOCK | The new signal mask for the process is the **union of its current signal mask and the signal set pointed to by set**. That is, set contains the additional signals that we want to block.|
|SIG_UNBLOCK | The new signal mask for the process is the intersection of its current signal mask and the complement of the signal set pointed to by set. That is, **set contains the signals that we want to unblock**.|
|SIG_SETMASK | The new signal mask for the process is **replaced by the value of the signal set pointed to by set**.|  

### sigpending  

读取进程未决信号集  

```c
#include <signal.h>

int sigpending(sigset_t *set);

//Returns: 0 if OK, −1 on error
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

//signal handler 1
void fun1(int signum) {
    printf("signal: %d\n", signum);
}

//signal handler 2
void fun2(int signum) {
    printf("signal: %d\n", signum);
}

int main()
{   
    int ret = -1;

    //signal set
    sigset_t set;
    sigset_t oset;

    //register signal handler
    // CTRL + C
    signal(SIGINT, fun1);
    // CTRL + '\'
    signal(SIGQUIT, fun2);

    printf("press any key to block siganl 2, 3\n");
    getchar();

    // block signal
    sigemptyset(&oset);
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGQUIT);
    ret = sigprocmask(SIG_BLOCK, &set, &oset);
    if (-1 == ret) {
        perror("sigprocmask");
        return 1;
    }
    printf("successfully block signal 2, 3\n");

    /////////////////////////////////////
    // get pending signals
    printf("press enter key to get pending signal\n");
    getchar();

    sigemptyset(&set);
    ret = sigpending(&set);
    if (-1 == ret) {
        perror("sigpending");
        return 1;
    }
    for (int i = 1; i < 32; ++i) {
        if (sigismember(&set, i)) {
            printf("%d ", i);
        }
    }
    putchar('\n');
    /////////////////////////////////////

    printf("press any key to unblock signal 2, 3\n");
    getchar();

    // unblock signal
    ret = sigprocmask(SIG_SETMASK, &oset, NULL);
    if (-1 == ret) {
        perror("sigprocmask");
        return 1;
    }

    printf("press any key to quit\n");
    getchar();

    return 0;
}
```

```shell
press any key to block siganl 2, 3
^Csignal: 2
^Csignal: 2
^\signal: 3
^\signal: 3

successfully block signal 2, 3
press enter key to get pending signal
^C^C^\^\^\
2 3 
press any key to unblock signal 2, 3

signal: 3
signal: 2
press any key to quit
^Csignal: 2
^\signal: 3

```

## 不可重入，可重入函数  

如果一个函数被设计成这样：不同任务调用这个函数时可能修改其他任务调用这个函数的数据，从而导致不可预料的结果，这样的函数时不安全的函数，也叫不可重入函数  

满足下列条件的函数多数是不可重入的：  
1.函数体内使用了静态的数据结构  
2.函数体内调用了malloc()或者free()  
3.函数体内调用了标准I/O函数（因为它们包含了缓冲区，子进程会继承父进程的缓冲区，因此子进程可能会输出父进程缓冲区中的内容）  

相反，就有可重入函数。是指一个可以被多个任务调用的过程，在调用时不必担心数据出错  

保证函数的可重入性需要：  
1.在写函数时尽可能使用局部变量  
2.对于要使用的全局变量要加以保护（比如采取关中断、信号量等互斥方法），这样构成的函数就是一个可重入的函数  

注意：**信号处理函数应该为可重入函数**  

## SIFCHLD信号  

### SIGCHLD信号产生的条件  

1.子进程终止时  
2.子进程接收到SIGSTOP信号停止时  
3.子进程处于停止态，接收到SIGCONT信号后唤醒时  

### 避免僵尸进程  

we can use wait() or waitpid() in the parent process, but parent process will be hung. So we use signal handler to prevent child from being a zombie  

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

void fun(int signum) {
    pid_t pid;
    
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        printf("child %d resources released\n", pid);
    }
}

int main()
{
    pid_t pid = -1;
    struct sigaction act;
    act.sa_handler = fun;
    act.sa_flags = 0;
    
    //register signal handler
    sigaction(SIGCHLD, &act, NULL);

    //create a child process
    pid =  fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    if (0 == pid) {
        //child process
        printf("child process will sleep for 2 secs\n");
        sleep(2);
        printf("after 2 secs, child exits\n");
        exit(0);
    }
    else {
        // parent
        while (1) {
            printf("parent is working\n");
            sleep(1);
        }
    }

    return 0;
}
```

also, we can use **signal(SIGCHLD, SIG_IGN)** without register any signal handler. In this way, signal will not be sent to parent process  

```c
......
// void fun(int signum) {
//     pid_t pid;

//     while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
//         printf("child %d resources released\n", pid);
//     }
// }

int main()
{
    pid_t pid = -1;
    // struct sigaction act;
    // act.sa_handler = fun;
    // act.sa_flags = 0;
    
    // //register signal handler
    // sigaction(SIGCHLD, &act, NULL);

    signal(SIGCHLD, SIG_IGN);

    ......
}
```

we check if there is a zombie (None !)  

```shell
jys@ubuntu:~/unix-programming/course/signal$ ps -aux | grep a.out
jys         6410  0.0  0.0   2496   508 pts/0    S+   23:13   0:00 ./a.out
jys         6419  0.0  0.0   9040   712 pts/1    S+   23:13   0:00 grep --color=auto a.out
```

## references  

Advanced Programming in the UNIX Environment, 3rd edition, Chapter 10  
The Linux programming interface a Linux and UNIX system programming handbook, 1st edition, Chapter 20, 23  
[相关课程](https://www.bilibili.com/video/BV1Yo4y1D7Ap?p=154&spm_id_from=pageDriver&vd_source=4b75b13c678ed297c8d0ed42e806f46b)  
