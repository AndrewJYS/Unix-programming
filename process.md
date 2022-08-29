# process  

## getpid, getppid, getpgid  

```c
#include <unistd.h>

pid_t getpid(void);
//Returns: process ID of calling process

pid_t getppid(void);
//Returns: parent process ID of calling process

pid_t getpgid(pid_t pid);
//Returns: process group ID if OK, −1 on error
```

A process group is a collection of one or more processes, usually associated with the same job, that can receive signals **from the same terminal**. Each process group has a **unique process group ID**.  

Each process group can have a process group leader. The **leader** is identified by its **process group ID** being **equal to its process ID**.  

## fork  

```c
#include <unistd.h>

pid_t fork(void);

//Returns: 0 in child, process ID of child in parent, −1 on error
```

This function is **called onc**e but **returns twice**. The only difference in the returns is that the return value **in the child is 0**, whereas the return value **in the parent is the process ID of the new child**. The reason the child’s process ID is returned to the parent is that a process can have more than one child, and there is no function that allows a process to obtain the process IDs of its children.  

child process and parent process share stack, heap, .data, .bss, .text, etc.  

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int g_cnt = 100;

int main()
{
    int var = 88;
    pid_t pid = -1;
    int* p = malloc(sizeof(int));
    if (NULL == p) {
        printf("malloc failed...\n");
        return 1;
    }
    //memset(p, 0, sizeof(p));
    *p = 200;

    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    if (0 == pid) {
        // child process
        sleep(1);
        printf("child process after parent process operation: g_cnt = %d, var = %d, *p = %d\n", g_cnt, var, *p);
    }
    else {
        // father process
        printf("parent process before operation: g_cnt = %d, var = %d, *p = %d\n", g_cnt, var, *p);
        g_cnt++;
        var++;
        (*p)++;
        printf("parent process after operation: g_cnt = %d, var = %d, *p = %d\n", g_cnt, var, *p);
    }

    return 0;
}
```

when we dynamically allocate memory, we need to destroy the pointer before main function exits to prevent memory leak. As for the example above, the child process shared the pointer of parent process, and we need to delete the pointer both in parent and child process.  

By the way, command "valgrind" is employed to scan memory leak.  

```shell
jys@ubuntu:~/unix-programming/course$ valgrind ./fork
......
==3670== HEAP SUMMARY:
==3670==     in use at exit: 4 bytes in 1 blocks
==3670==   total heap usage: 2 allocs, 1 frees, 1,028 bytes allocated
==3670== 
==3670== LEAK SUMMARY:
==3670==    definitely lost: 4 bytes in 1 blocks
......
jys@ubuntu:~/unix-programming/course$ child process after parent process operation: g_cnt = 100, var = 88, *p = 200
==3671== 
==3671== HEAP SUMMARY:
==3671==     in use at exit: 4 bytes in 1 blocks
==3671==   total heap usage: 2 allocs, 1 frees, 1,028 bytes allocated
==3671== 
==3671== LEAK SUMMARY:
==3671==    definitely lost: 4 bytes in 1 blocks
......
```

The modified code is as below  

```c
......
int main()
{
    .......
    if (0 == pid) {
        ......
        free(p);
    }
    else {
        ......
        free(p);
    }
    return 0;
}
```

again, we use "valgrind" to test  

```shell
jys@ubuntu:~/unix-programming/course$ valgrind ./fork
......
==4276== HEAP SUMMARY:
==4276==     in use at exit: 0 bytes in 0 blocks
==4276==   total heap usage: 2 allocs, 2 frees, 1,028 bytes allocated
==4276== 
==4276== All heap blocks were freed -- no leaks are possible
......
jys@ubuntu:~/unix-programming/course$ child process after parent process operation: g_cnt = 100, var = 88, *p = 200
==4277== 
==4277== HEAP SUMMARY:
==4277==     in use at exit: 0 bytes in 0 blocks
==4277==   total heap usage: 2 allocs, 2 frees, 1,028 bytes allocated
==4277== 
==4277== All heap blocks were freed -- no leaks are possible
......
```

## exit, _exit  

```c
#include <unistd.h> // system call
void _exit(int status);

#include <stdlib.h> // library function
void exit(int status);
```

The following actions are performed by exit():  
1.Exit handlers (functions registered with atexit() and on_exit()) are called, in reverse order of their registration.
2.The **stdio stream buffers are flushed**.  
3.The \_exit() system call is invoked, using the value supplied in _status_  

Note that \_exit() will not flush the stdio stream buffer. Hence, the program below will probably not output "hello world"  

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    printf("hello world\n");

    exit(0);
}
```

## wait, waitpid  

```c
#include <sys/wait.h>

pid_t wait(int *statloc);
pid_t waitpid(pid_t pid, int *statloc, int options);

//Both return: process ID if OK, 0 (see later), or −1 on error
```

When a **process terminates**, either normally or abnormally, the **kernel notifies the parent** by sending the **SIGCHLD signal** to the parent.  

For now, we need to be aware that a process that calls _wait_ or _waitpid_ can  
1.Block, if all of its children are still running  
2.Return immediately with the termination status of **a child** (note that **as long as one** child process terminates, the _wait_ or _waitpid_ function will return. so if we want to **wait** for **all** child processes **exiting**, we should **call the function several times**), if a child has terminated and is waiting for its termination status to be fetched  
3.Return immediately with an error, if it doesn’t have any child processes  
  
The differences between these two functions are as follows:  
The **wait** function can **block the caller** until a child process terminates, whereas **waitpid** has an **option** that **prevents** it from **blocking**.  
  
Traditionally, the integer status **statloc** that these two **functions return** has been defined by the implementation, **with certain bits indicating the exit status (for a normal return), other bits indicating the signal number (for an abnormal return)**, one bit indicating whether a core file was generated, and so on. POSIX.1 specifies that the termination status is to be looked at using various macros that are defined in \<sys/wait.h\>. **Four mutually exclusive macros** tell us **how the process terminated**, and they all begin with WIF. **Based on which of these four macros is true, other macros are used to obtain the exit status, signal number, and the like.**  

|Macro | Description |
:-:|:-:
|**WIFEXITED(status)** | **True if** status was returned for a child that **terminated normally**. In this case, we can execute **WEXITSTATUS(status)** to fetch the low-order 8 bits of the argument that the **child passed to exit, \_exit, or \_Exit**.|
|**WIFSIGNALED(status)** | **True if** status was returned for a child that **terminated abnormally, byreceipt of a signal that it didn’t catch.** In this case, we can execute **WTERMSIG(status)** to fetch the **signal number that caused the termination**. |
|**WIFSTOPPED(status)** | **True if** status was returned for **a child that is currently stopped**. In this case, we can execute **WSTOPSIG(status)** to fetch the **signal number that caused the child to stop**.|
|WIFCONTINUED(status) | **True if** status was returned for a **child** that has been **continued** after a job control stop (XSI option; waitpid only).|  
  
example for wait and related macros  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int ret = -1;
    int status = -1;
    pid_t pid = -1;

    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    //child process
    if (0 == pid) {
        for (int i = 0; i < 15; ++i) {
            printf("child process %d do thing %d\n", getpid(), i + 1);
            sleep(1);
        }
        exit(10);
    }

    //parent process
    printf("parent waits for child exiting\n");
    ret = wait(&status);
    if (-1 == ret) {
        perror("wait");
        return 1;
    }
    printf("parent collect child's resources\n");

    if (WIFEXITED(status)) {
        printf("child exit status: %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
        // kill -9 (childpid)
        printf("child is killed by signal %d\n", WTERMSIG(status));
    }
    else if (WIFSTOPPED(status)) {
        // kill -l      -------kill command options
        // kill -19 childpid       -------stop child
        // kill -18 childpid       -------continue child
        printf("child is stopped bt signal %d\n", WSTOPSIG(status));
    }

    return 0;
}
```

as for pid_t waitpid(pid_t pid, int *statloc, int options), the argument pid has many choices  

1.If pid is **greater than 0**, wait for the **child whose process ID equals pid**.  
2.If pid **equals 0**, wait for **any child in the same process group as the caller** (parent).  
3.If pid is **less than –1**, wait for **any child whose process group identifier equals the absolute value of pid**.  
4.If pid **equals –1**, wait for **any child**. The call wait(&status) is equivalent to the call waitpid(–1, &status, 0).  

In addition, if **options == WNOHANG**, then If no child specified by pid has yet changed state, then **return immediately**, instead of blocking. In this case, the return value of waitpid() is 0. If the calling process has no children that match the specification in pid, waitpid() fails with the error ECHILD.  

## references  

Advanced Programming in the UNIX Environment, 3rd edition, Chapter 8  
The Linux programming interface a Linux and UNIX system programming handbook, 1st edition, Chapter 25, 26  
