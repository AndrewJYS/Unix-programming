# daemon  

## 创建守护进程  

```md
1.创建子进程，父进程退出（必须）。  
--所有工作在子进程中进行，形式上脱离了控制终端  
2.在子进程中创建新的会话  
--setsid()  
--使子进程完全独立出来，脱离控制  
3.改变当前目录为根目录（不是必须）  
--chdir()  
--防止占用可以卸载的文件系统  
--也可以换成其他路径  
4.重设文件权限掩码（不是必须）  
--umask()  
--防止继承的文件创建屏蔽字拒绝某些权限  
--增加守护进程的灵活性  
--补充：创建文件的最大权限是0666, 创建目录的最大权限是0777  
5.关闭文件描述符（不是必须）  
--继承的打开文件不会用到，浪费系统资源，无法卸载
6.开始执行守护进程核心工作（必须）  
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    pid_t pid = -1;
    int ret = -1;

    //1.create child, parent exit
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }
    if (pid > 0) {
        exit(0);
    }

    //2.create new session
    pid = setsid();
    if (-1 == pid) {
        perror("setsid");
        return 1;
    }

    //3.change current working directory
    ret = chdir("/");
    if (-1 == ret) {
        perror("chdir");
        return 1;
    }

    //4.set file umask
    umask(0);

    //5.lose file descriptor
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //6.execute daemon
    //output current time into txt.log
    while (1) {
        system("date >> /tmp/txt.log");
        sleep(10);
    }

    return 0;
}
```

动态地查看文件的变化  

```shell
tail -f /tmp/txt.log
```

## references  

Advanced Programming in the UNIX Environment, 3rd edition, Chapter 13  
The Linux programming interface a Linux and UNIX system programming handbook, 1st edition, Chapter 37  
[相关课程](https://www.bilibili.com/video/BV1Yo4y1D7Ap)
