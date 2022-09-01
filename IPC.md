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

假设我们创建了一个FIFO文件，现在用它来给两个进程通信

```c
//写端
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 128

int main()
{
    int fd = -1;
    int ret = -1;
    int i = 0;
    char buf[SIZE];

    // open an FIFO with write-only
    // cannot use  O_CREAT, for fifo is a special file
    fd = open("fifo", O_WRONLY);
    if (-1 == fd) {
        perror("open");
        return 1;
    }
    printf("write-only fifo opened\n");

    //write into fifo
    while (1) {
        memset(buf, 0, SIZE);
        sprintf(buf, "hello world %d", i++);
        ret = write(fd, buf, strlen(buf));
        if (ret <= 0) {
            perror("write");
            break;
        }
        printf("write into fifo length: %d\n", ret);
        sleep(1);
    }

    //close fifo
    close(fd);

    return 0;
}
```

```c
//读端
......

int main()
{
    ......

    //open fifo with read-only
    fd = open("fifo", O_RDONLY);
    if (-1 == fd) {
        perror("open");
        return 1;
    }
    printf("read-only fifo opened\n");

    // read fifo in a loop
    while (1) {
        memset(buf, 0, SIZE);
        ret = read(fd, buf, SIZE);
        if (ret <= 0) {
            perror("read");
            break;
        }

        printf("buf: %s\n", buf);
    }

    //close fifo
    close(fd);
    
    return 0;
}
```

## Memory-Mapped  

```c
#include <sys/mman.h>

void *mmap(void *addr, size_t len, int prot, int flag, int fd, off_t offset );

//Returns: starting address of mapped region if OK, MAP_FAILED on error
```

The **addr** argument lets us specify **the address where we want the mapped region to start**. We **normally** set this value to **0** to **allow the system to choose the starting address**. The return value of this function is the starting address of the mapped area.  

The **fd** argument is the file descriptor specifying the **file that is to be mapped**. We have to **open this file before we can map it into the address space**.  

The **len** argument is the **number of bytes to map**. Although length doesn’t need to be a multiple of the system page size (as returned by sysconf(_SC_PAGESIZE)), the kernel creates mappings in units of this size, so that length is, in effect, rounded up to the next multiple of the page size.  

The **offset** argument specifies the **starting point of the mapping in the file**, and must **be a multiple of the system page size**. To map the entire file, we would specify offset as 0 and length as the size of the file.  

The **prot** argument specifies the **protection of the mapped region**. The protection (**when flags == MAP_SHARED**) specified for a region **can’t allow more access than the open mode of the file**. For example, we can’t specify PROT_WRITE if the file was opened read-only.  

|prot|description|
:-:|:-:
|PROT_NONE   |The region may not be accessed                |
|PROT_READ   |The contents of the region can be read        |
|PROT_WRITE  |The contents of the region can be modified    |
|PROT_EXEC   |The contents of the region can be executed    |

The **flags** argument is a bit mask of options controlling various aspects of the mapping operation. Exactly one of the following values must be included in this mask:
**MAP_PRIVATE**: Create a private mapping. **Modifications to the contents of the region** are **not visible to other processes employing the same mapping**, and, in the case of **a file mapping, are not carried through to the underlying file**.  
**MAP_SHARED**： Create a shared mapping. **Modifications to the contents of the region** are **visible to other processes mapping the same region** with the MAP_SHARED attribute and, in the case of a file mapping, are **carried through to the underlying file**. Updates to the file are not guaranteed to be immediate;

```c
#include <sys/mman.h>

int munmap(void *addr, size_t len);

//Returns: 0 if OK, −1 on error
```

example: note that the **file with size 0 cannot be mapped**! we can **ftruncate** the file or write something into it to **make sure the file size bigger than 0** before memory mapping, otherwise, "**bus error**" will be thrown

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
    int fd = -1;
    int ret = -1;
    void* addr = NULL;

    // open a file with read-and-write
    fd = open("txt", O_RDWR);
    if (-1 == fd) {
        perror("open");
        return 1;
    }
    //if no ftruncate, then bus error will be thrown
    ftruncate(fd, 1024);

    // memory mapping
    addr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) { //must check returned addr
        perror("mmap");
        return 1;
    }
    printf("mmap ok\n");

    //close file
    close(fd);

    //write into mapped memory, also into file
    memcpy(addr, "1234567890", 10);

    // disconnect mmap
    munmap(addr, 1024);

    return 0;
}
```

```shell
jys@ubuntu:~/unix-programming/course/IPC$ gcc mmap.c
jys@ubuntu:~/unix-programming/course/IPC$ ./a.out 
mmap ok
jys@ubuntu:~/unix-programming/course/IPC$ cat txt
1234567890
```

## 共享映射实现父子进程通信  

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main()
{
    int fd = -1;
    int ret = -1;
    pid_t pid = -1;
    void* addr = NULL;

    // open a file with read-and-write
    fd = open("txt", O_RDWR);
    if (-1 == fd) {
        perror("open");
        return 1;
    }
    //if no ftruncate, then bus error will be thrown
    ftruncate(fd, 1024);

    // memory mapping
    addr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    printf("mmap ok\n");

    //close file
    close(fd);

    //create a child process
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }
    
    if (0 == pid) {
        // child process
        //write into mapped memory, also into file
        memcpy(addr, "1234567890", 10);
    }
    else {
        //parent
        wait(NULL);
        printf("addr:%s\n", (char* )addr);
    }

    // disconnect mmap both for child and parent process
    munmap(addr, 1024);

    return 0;
}
```

## 一般进程间使用mmap通信  

```c
// inter process communication: read
......

int main()
{
    ......
    // read mapped memory
    printf("addr: %s\n", (char* )addr);
    ......
}
```

```c
// inter process communication: write
......

int main()
{
    ......
    //write into mapped memory, also into file
    memcpy(addr, "1234567890", 10);
    ......
}
```

## 利用匿名映射实现父子进程通信  

仍然使用mmap函数，但是参数有所改变, flags中使用**MAP_ANONYMOUS | MAP_SHARED**  

**MAP_ANONYMOUS**: The **mapping is not backed by any file**; its contents are initialized to zero. The fd argument is ignored; however, some implementations require **fd to be -1** if MAP_ANONYMOUS (or MAP_ANON) is specified, and portable applications should ensure this. The **offset argument should be zero**. The use of **MAP_ANONYMOUS in conjunction with MAP_SHARED** is supported on Linux only since kernel 2.4.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main()
{
    int fd = -1;
    int ret = -1;
    void* addr = NULL;
    pid_t pid = -1;

    // create anonymous map
    addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    printf("create anonymous map\n");

    // create child process
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    //child-parent communication
    if (0 == pid) {
        //child write
        memcpy(addr, "1234567890", 10);
    }
    else {
        // parent read
        wait(NULL);
        printf("parent process: %s\n", (char* )addr);
    }

    //disconnect mapping
    munmap(addr, 4096);

    return 0;
}
```

## references  

Advanced Programming in the UNIX Environment, 3rd edition, Chapter 14, 15  
The Linux programming interface a Linux and UNIX system programming handbook, 1st edition, Chapter 11, 49  
