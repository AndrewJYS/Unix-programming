# interprocess comunication  

## pipe  

Pipes are the oldest form of UNIX System IPC and are provided by all UNIX systems.  
Pipes have two limitations.  
1.Historically, they have been **half duplex** (i.e., data flows in only one direction).  
2.Pipes can be used **only between processes that have a common ancestor**. Normally, a pipe is created by a process, that process calls fork, and the pipe is used between the parent and the child.  

```c
#include <unistd.h>

int pipe(int fd[2]);

//Returns: 0 if OK, −1 on error
```

Two file descriptors are returned through the fd argument: fd[0] is open for reading, and fd[1] is open for writing. The output of fd[1] is the input for fd[0].  

管道的四种情况：  
1.如果写端没有关闭，管道中没有数据，这时读管道的进程去读管道会阻塞；如果写端没有关闭，管道中有数据，这时读管道的进程将数据读出，下一次读没有数据就会阻塞  
2.管道所有的写端关闭，读进程去读管道的内容，读取全部内容，最后返回0  
3.所有读端没有关闭，如果管道被写满了，写管道的进程写管道会阻塞  
4.所有读端关闭，写管道进程写管道会收到一个信号，然后退出  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SIZE 128

int main()
{
    int ret = -1;
    int fds[2];
    pid_t pid = -1;
    char buf[SIZE];

    // create pipe
    ret = pipe(fds);
    if (-1 == ret) {
        perror("pipe");
        return 1;
    }

    // create child process
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    // child
    if (0 == pid) {
        // close write
        close(fds[1]);

        // read
        memset(buf, 0, SIZE);
        ret = read(fds[0], buf, SIZE);
        if (ret < 0) {
            perror("read");
            exit(-1);
        }

        printf("child process: buf = %s\n", buf);

        //close read
        close(fds[0]);

        //exit child
        //if there is no exit(0), then the child will run the code in the parent process, and write again. at that time, the 'read endian' is closed, then there is error
        exit(0);
    }

    //parent
    //close read
    close(fds[0]);

    //write
    ret = write(fds[1], "ABCDEFGHIJ", 10);
    if (-1 == ret) {
        perror("write");
        return 1;
    }

    printf("parent process write len: %d\n", ret);

    //close write
    close(fds[1]);

    return 0;
}
```

## 关于pipe的属性  

### 设置为非阻塞的方法  

```c
int flags = fcntl(fds[0], F_GETFL);
flags |= O_NONBLOCK;
fcntl(fds[0], F_SETFL, flags);
```

修改后的非阻塞pipe程序如下：  

```c
......
#include <fcntl.h>

#define SIZE 128

int main()
{
    ......

    // child
    if (0 == pid) {
        ......

        //unblocked read endian
        int flags = fcntl(fds[0], F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(fds[0], F_SETFL, flags);

        ......
    }

    //parent
    sleep(1);
    ......

    printf("parent process write len: %d\n", ret);

    .......
}
```

父进程将不会输出，原因是，子进程将管道的读端设置成非阻塞，于是子进程提前退出并关闭了读端，此时父进程苏醒，读端关闭，写端写数据会报错，于是return 1退出main函数  

### 查看管道缓冲区大小的命令  

```shell
ulimit -a
```

可以看到默认pipe有8块，每块512字节，因此总计大小为4K

```shell
jys@ubuntu:~/unix-programming/course$ ulimit -a
......
pipe size            (512 bytes, -p) 8
......
```

### 查看pipe缓冲区大小的函数  

```c
#include <unistd.h>

long fpathconf(int fd, int name);

//return value of limit specified by name, or –1 if limit is indeterminate or an error occurred
````

**name:_PC_PIPE_BUF**  
For a FIFO or a pipe, this yields a value that applies to the referenced file. For a directory, the value applies to a FIFO created in that directory. Behavior for other file types is unspecified.  

```c
printf("pipe size: %ld\n", fpathconf(fds[0], _PC_PIPE_BUF));
```

## FIFO  

FIFO有时也成为命名管道，与pipe主要有下述不同  
1.FIFO在文件系统中作为一个特殊文件存在，但FIFO中的内容却存放在内存中  
2.当使用FIFO的进程退出后，FIFO文件将继续保存在文件系统中，以便以后使用  
3.FIFO有名字，不相关的进程可以通过打开命名管道进行通信  

### 通过命令创建FIFO  

```shell
mkfifo myfifo   myfifo就是FIFO的名字
```

### 通过函数创建FIFO  

```c
#include <sys/stat.h>

int mkfifo(const char *path, mode_t mode);

//return: 0 if OK, −1 on error
```

The specification of the **mode argument** is the **same** as for the **open function**  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    int ret = -1;

    // create an FIFO
    ret = mkfifo("fifo", 0644);
    if (-1 == ret) {
        perror("mkfifo");
        return 1;
    }

    printf("create an FIFO\n");

    return 0;
}
```

## 使用FIFO通信  



## references  

Advanced Programming in the UNIX Environment, 3rd edition, Chapter 15  
The Linux programming interface a Linux and UNIX system programming handbook, 1st edition, Chapter 11  
