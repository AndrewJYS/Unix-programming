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

If a **child has already terminated and is a zombie**, **wait returns immediately** with that child’s status. **Otherwise, it blocks the caller until a child terminates**. If the caller blocks and **has multiple children, wait returns when one terminates**.  
  
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

## zombie  

If a parent creates a child, but **fails to perform a wait()**, then an entry for the **zombie child will be maintained indefinitely in the kernel’s process table**. If a large number of such zombie children are created, they will eventually fill the kernel process table, preventing the creation of new processes. Since the **zombies can’t be killed by a signal**, the **only way to remove them from the system is to kill their parent (or wait for it to exit)**, at which time the zombies are adopted and waited on by init, and consequently removed from the system.

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = -1;

    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    // child process
    if (0 == pid) {
        for (int i = 0; i < 5; ++i) {
            printf("child do things %d\n", i);
            sleep(1);
        }
        printf("child exit\n");
        exit(0);
    }
    // int status = -1;
    // wait(&status);

    //parent process
    sleep(100);
    // when parent is sleeping, the exited child becomes a zombie
    // after parent sleeping, resources of chid will be collected by parent

    return 0;
}
```

## exec  

one use of the **fork** function is to create a new process (the child) that then causes another program to be executed by calling one of the **exec** functions. When a process calls one of the exec functions, **that process is completely replaced by the new program**, and the new program **starts executing at its main function**. The **process ID does not change** across an exec, because a new process is not created; exec merely replaces the current process — its text, data, heap, and stack segments — with a brand-new program from disk.  

```c
#include <unistd.h>

int execl(const char *pathname, const char *arg0, ... /* (char *)0 */ );
int execv(const char *pathname, char *const argv[]);
int execle(const char *pathname, const char *arg0, ...
/* (char *)0, char *const envp[] */ );
int execve(const char *pathname, char *const argv[], char *const envp[]);
int execlp(const char *filename, const char *arg0, ... /* (char *)0 */ );
int execvp(const char *filename, char *const argv[]);
int fexecve(int fd, char *const argv[], char *const envp[]);

//All seven return: −1 on error, no return on success
```

The **first difference** in these functions is that **the first four take a pathname argument**, **the next two take a filename argument**, and **the last one takes a file descriptor argument**. When a filename argument is specified,  
• If filename **contains a '/'**, it is **taken as a pathname**.  
• **Otherwise**, the executable file is searched for in the directories specified by the **PATH** environment variable.  
The PATH variable contains a list of directories, called path prefixes, that are separated by ':'.  

The **next difference** concerns the **passing of the argument list (l stands for list and v stands for vector)**. The functions execl, execlp, and execle require each of the command-line arguments to the new program to be specified as separate arguments. We **mark the end of the arguments with a null pointer**. For the other four functions (execv, execvp, execve, and fexecve), we have to **build an array of pointers to the arguments**, and the address of this array is the argument to these three functions.  

example for execlp:  

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("hello world\n");
    execlp("ls", "ls", "-l", "/home", NULL);

    printf("hello world\n");
    
    return 0;
}
```

Output is as below, note that **the second "hello world" is not output**, because, execlp replaces completely the previous process.  

```shell
jys@ubuntu:~/unix-programming/course$ ./a.out
hello world
total 4
drwxr-xr-x 27 jys jys 4096 Aug 28 23:26 jys
```

example for execlp, execv  

```c
//execlp
execlp("/bin/ls", "ls", "-l", "/home", NULL);

//execv
char *args[] = {"ls", "-l", "/home", NULL};
execv("/bin/ls", args);

//execvp
char *args[] = {"ls", "-l", "/home", NULL};
execv("ls", args);
```

## terminal & shell  

Both a conventional terminal and a terminal emulator have an **associated terminal driver** that **handles input and output** on the device. (In the case of a terminal emulator, the device is a pseudoterminal.)  

A **shell** is a **command-line interpreter** that reads user input and executes commands. The user **input to a shell is normally from the terminal** (an interactive shell) or **sometimes from a file** (called a shell script).  

## Process Groups  

A process group is a collection of **one or more processes**, usually associated with the same job, that can **receive signals from the same terminal**. Each process group has a **unique process group ID**.  

```c
#include <unistd.h>

pid_t getpgrp(void);

//Returns: process group ID of calling process
```

```c
#include <unistd.h>

pid_t getpgid(pid_t pid);

//Returns: process group ID if OK, −1 on error
```

If pid is 0, the process group ID of the calling process is returned. Thus  

```c
getpgid(0);
```

is equivalent to  

```c
getpgrp();
```

Each process group can have a process group leader. **The leader is identified by its process group ID being equal to its process ID**. It is possible for a process group **leader** to **create a process group, create processes in the group, and then terminate**. The process **group still exists**, **as long as at least one process is in the group**, regardless of whether the group leader terminates. This is called the process group lifetime--the period of time that begins when the group is created and ends when the last remaining process leaves the group. The last remaining process in the process group can either terminate or enter some other process group.  

A process **joins an existing process group or creates a new process group** by calling **setpgid**  

```c
#include <unistd.h>

int setpgid(pid_t pid, pid_t pgid);

//Returns: 0 if OK, −1 on error
```

This function sets the process group ID to pgid in the process whose process ID equals pid. If the two arguments are equal, the process specified by pid becomes a process group leader. If pid is 0, the process ID of the caller is used. Also, if pgid is 0, the process ID specified by pid is used as the process group ID.  

## Sessions  

A session is a collection of **one or more process groups**.  

A process establishes a new session by calling the **setsid** function.  

```c
#include <unistd.h>

pid_t setsid(void);

// Returns: process group ID if OK, −1 on error
```

If the **calling process** is **not a process group leader**, this function creates a new session. Three things happen.  
1.The process becomes the session leader of this new session.  
2.The process becomes the process group leader of a new process group.  
3.The process has no controlling terminal. If the process had a controlling terminal before calling setsid, that association is broken.  
This function **returns an error if the caller is already a process group leader**. To ensure this is not the case, the usual practice is to call fork and have the parent terminate and the child continue.  

The **getsid** function returns the process group ID of a process’s session leader.  

```c
#include <unistd.h>

pid_t getsid(pid_t pid);

//Returns: session leader’s process group ID if OK, −1 on error
```

If **pid is 0**, getsid returns the process group ID of the calling process’s session leader  

example:  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    pid_t pid = -1;

    // get process group ID of a process’s session leader
    pid = getsid(0);
    if (-1 == pid) {
        perror("getsid");
        return 1;
    }
    printf("sid: %d\n", pid);

    return 0;

}
```

## references  

Advanced Programming in the UNIX Environment, 3rd edition, Chapter 1, 8, 9  
The Linux programming interface a Linux and UNIX system programming handbook, 1st edition, Chapter 25, 26, 62  
[相关课程](https://www.bilibili.com/video/BV1Yo4y1D7Ap)  
