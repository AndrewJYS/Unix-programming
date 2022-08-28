# file-related  

## stat  

```c
#include <sys/stat.h>

int stat(const char *restrict pathname, struct stat *restrict buf );
int lstat(const char *restrict pathname, struct stat *restrict buf );

//return: 0 if OK, −1 on error
```

The functions fill in the structure 'buf'. The definition of the structure can differ among implementations, but it could look like

```md
struct stat {
mode_t      st_mode;            /* file type & mode (permissions) */
ino_t       st_ino;             /* i-node number (serial number) */
dev_t       st_dev;             /* device number (file system) */
dev_t       st_rdev;            /* device number for special files */
nlink_t     st_nlink;           /* number of links */
uid_t       st_uid;             /* user ID of owner */
gid_t       st_gid;             /* group ID of owner */
off_t       st_size;            /* size in bytes, for regular files */
struct      timespec st_atim;   /* time of last access */
struct      timespec st_mtim;   /* time of last modification */
struct      timespec st_ctim;   /* time of last file status change */
blksize_t   st_blksize;         /* best I/O block size */
blkcnt_t    st_blocks;          /* number of disk blocks allocated */
};
```

example for finding type of file  

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void show_file_type(struct stat* s) {
    if (S_ISREG(s->st_mode))
        printf("is a regular file\n");
    if (S_ISDIR(s->st_mode))
        printf("is a directory\n");
    if (S_ISCHR(s->st_mode))
        printf("is character device\n");
    if (S_ISBLK(s->st_mode))
        printf("is a block device\n");
    if (S_ISLNK(s->st_mode))
        printf("is a symbolic link\n");
    if (S_ISFIFO(s->st_mode)) // pipe
        printf("is a FIFO\n");
    if (S_ISSOCK(s->st_mode))
        printf("is a socket\n");

}

int main(int argc, char** argv) {
    int ret =  -1;
    struct stat s;

    // judge
    if (2 != argc) {
        printf("usage: ./stat_st_mode filename\n");
        return 1;
    }

    //get file info
    ret = stat(argv[1], &s);
    if (-1 == ret) {
        perror("stat");
        return 1;
    }

    //show file info
    show_file_type(&s);

    return 0;
}
```

we test some files  

```shell
jys@ubuntu:~/unix-programming/course$ gcc stat_st_mode.c -o stat_st_mode
jys@ubuntu:~/unix-programming/course$ ./stat_st_mode /home
is a directory
jys@ubuntu:~/unix-programming/course$ ./stat_st_mode /dev/input/mouse1
is character device
jys@ubuntu:~/unix-programming/course$ ./stat_st_mode /dev/sda1
is a block device
jys@ubuntu:~/unix-programming/course$ ./stat_st_mode /txt
stat: No such file or directory
jys@ubuntu:~/unix-programming/course$ ./stat_st_mode txt
is a regular file
jys@ubuntu:~/unix-programming/course$ 
```

example for finding permission of file (append after the code above)  

```c
......

void show_file_type(struct stat* s) {
    ......
}

void show_permission(struct stat* s) {
    printf("user permission: ");
    //user read permission
    if (s->st_mode & S_IRUSR) 
        printf("r");
    else
        printf("-");
    //user write permission
    if (s->st_mode & S_IWUSR)
        printf("w");
    else
        printf("-");
    //user execute permission
    if (s->st_mode & S_IXUSR)
        printf("x");
    else
        printf("-");
}

int main(int argc, char** argv) {
    ......
    //show file permission
    show_permission(&s);
    printf("\n");

    return 0;
}
```

we test some files  

```md
jys@ubuntu:~/unix-programming/course$ ./stat_st_mode txt
is a regular file
user permission: rw-
jys@ubuntu:~/unix-programming/course$ ./stat_st_mode /dev/sda1
is a block device
user permission: rw-
jys@ubuntu:~/unix-programming/course$ ./stat_st_mode /dev/input/mouse1
is character device
user permission: rw-
jys@ubuntu:~/unix-programming/course$ ./stat_st_mode /home
is a directory
user permission: rwx
```

## dup  

```c
#include <unistd.h>

int dup(int fd);
int dup2(int fd, int fd2);

//Both return: new file descriptor if OK, −1 on error
```

The new file descriptor returned by dup is guaranteed to be the **lowest-numbered** available file descriptor.  

With dup2, we specify the value of the new descriptor with the fd2 argument. If fd2 is **already open**, it is **first closed**. If **fd equals fd2**, then dup2 **returns fd2 without closing it**.  

example for dup():  

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = -1;
    int newfd = -1;

    // open file
    fd = open("txt", O_RDWR | O_CREAT, 0644);
    if (-1 == fd) {
        perror("open");
        return 1;
    }
    printf("fd = %d\n", fd);

    //copy file descriptor
    newfd = dup(fd);
    if (-1 == newfd) {
        perror("dup");
        return 1;
    }
    printf("newfd = %d\n", newfd);

    //write
    write(fd, "ABCDEFG", 7);
    write(newfd, "1234567", 7);

    //close file
    close(fd);
    close(newfd);

    return 0;
}
```

## fcntl  

```c
#include <fcntl.h>

int fcntl(int fd, int cmd, ... /* int arg */ );

//Returns: depends on cmd if OK (see following), −1 on error
```

The fcntl function is used for **five different purposes**.  
1.**Duplicate an existing descriptor** (cmd = F_DUPFD or F_DUPFD_CLOEXEC)  
2.Get/set file descriptor flags (cmd = F_GETFD or F_SETFD)  
3.**Get/set file status flags** (cmd = F_GETFL or F_SETFL)  
4.Get/set asynchronous I/O ownership (cmd = F_GETOWN or F_SETOWN)  
5.Get/set record locks (cmd = F_GETLK, F_SETLK, or F_SETLKW)  
  
**F_DUPFD**: Duplicate the file descriptor fd. The new file descriptor is returned as the value of the function. It is the **lowest-numbered** descriptor that is **not already open**, and that is **greater than or equal to the third argument** (taken as an integer).  
  
**F_GETFL**: **Return** the **file status flags** for fd as the value of the function  
  
**F_SETFL**: **Set** the **file status flags** to the value of the third argument (taken as an integer). The only flags that can be changed are **O_APPEND, O_NONBLOCK, O_SYNC, O_DSYNC, O_RSYNC, O_FSYNC, and O_ASYNC**.  

| File status flag | Description |
:-:|:-:
|O_RDONLY    |open for reading only|
|O_WRONLY    |open for writing only|
|O_RDWR      |open for reading and writing|
|O_EXEC      |open for execute only|
|O_SEARCH    |open directory for searching only|
|**O_APPEND**    |append on each write|
|**O_NONBLOCK**  |nonblocking mode|
|**O_SYNC**      |wait for writes to complete (data and attributes)|
|**O_DSYNC**     |wait for writes to complete (data only)|
|**O_RSYNC**     |synchronize reads and writes|
|**O_FSYNC**     |wait for writes to complete (FreeBSD and Mac OS X only)|
|**O_ASYNC**     |asynchronous I/O (FreeBSD and Mac OS X only)|
  
example for Duplicate an existing descriptor  

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd = -1;
    int newfd = -1;
    int ret = -1;

    // open file
    fd = open("txt", O_RDWR | O_CREAT, 0644);
    if (-1 == fd) {
        perror("open");
        return -1;
    }
    printf("fd = %d\n", fd);

    // copy file descriptor
    newfd = fcntl(fd, F_DUPFD, 0);
    if (-1 == newfd) {
        perror("fcntl");
        return 1;
    }
    printf("newfd = %d\n", newfd);

    //wtite, the two file descriptors share offset
    write(fd, "1234567", 7);
    write(newfd, "ABCDEFG", 7);

    //close
    close(fd);
    close(newfd);

    return 0;
}
```

example for Get/set file status flags  

```c
......

int main()
{
    int fd = -1;
    int ret = -1;

    // open file
    fd = open("txt", O_RDWR | O_CREAT, 0644);
    if (-1 == fd) {
        perror("open");
        return -1;
    }
    printf("fd = %d\n", fd);

    //get file status
    ret = fcntl(fd, F_GETFL);
    if (-1 == ret) {
        perror("fcntl");
        return 1;
    }
    if (ret & O_APPEND) {
        printf("before append\n");
    } else {
        printf("before not append\n");
    }

    //set file status
    ret = ret | O_APPEND;
    ret = fcntl(fd, F_SETFL, ret);
    if (-1 == ret) {
        perror("fcntl");
        return 1;
    }

    //get file status
    ret = fcntl(fd, F_GETFL);
    if (-1 == ret) {
        perror("fcntl");
        return 1;
    }
    if (ret & O_APPEND) {
        printf("after append\n");
    } else {
        printf("after not append\n");
    }

    //close file
    close(fd);

    return 0;
}
```

## references  

Advanced Programming in the UNIX Environment, 3rd edition, Chapter 3, 4  
