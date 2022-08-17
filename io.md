# introduction to I/O system calls  

The functions described here are often referred to as **unbuffered I/O**, in contrast to the standard I/O routines.  

## open  

```c
#include <fcntl.h>

int open(const char *path, int oflag, ... /* mode_t mode */ );

//return: file descriptor if OK, −1 on error
```

|flag|descriptions|
:-:|:-:
| O_RDONLY | Open for reading only. |
| O_WRONLY | Open for writing only. |
| O_RDWR | Open for reading and writing. |
| O_APPEND | Append to the end of file on each write. |
| O_CREAT | Create the file if it doesn’t exist. This option requires **a third argument** to the open function  — the **mode**, which specifies the access permission bits of the new file. |
| O_EXCL | Generate an error if O_CREAT is also specified and the file already exists. This test for whether the file already exists and the creation of the file if it doesn’t exist is an atomic operation.|

Examples:  

```c
int fd;

//read-only, if the file does not exist, then throw error
fd = open("txt", O_RDONLY);

//write-only, if the file exists, the open it;
//if the file does not exist, then create a new file, and set permission 0644
fd = open("txt", O_WRONLY | O_CREAT, 0644);

//write-only, if the file exists, then throw error
//if the file does not exist, then create a new file, and set permission 0644
fd = open("txt", O_WRONLY | O_CREAT | O_EXCL, 0644);

//read-and-write, ...
fd = open("txt", O_RDWR | O_CREAT, 0644);

//write-only, if the file does not exist, then create a new file, and set permission 0644
//if the file exists, then clear the file
fd = open("txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);

//if the file does not exist, then throw error
//if the file exists, then set mode 'APPEND when writing'
fd = open("txt", O_WRONLY | O_APPEND);
```

## close  

```c
#include <unistd.h>

int close(int fd);

//Returns: 0 if OK, −1 on error
```

## read  

```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t nbytes);

//Returns: number of bytes read, 0 if end of file, −1 on error
```

There are several cases in which the number of bytes actually read is **less than** the amount requested, the most commom situation is as follows:  

When reading from a regular file, if the end of file is reached before the requested number of bytes has been read. For example, if 30 bytes remain until the end of file and we try to read 100 bytes, read returns 30. The next time we call read, it will return 0 (end of file).  

Examples:  

```c
int fd = -1;
int ret = -1;
char buf[SIZE];

// 1.open file
fd = open("txt", O_RDONLY, 0644);
if (-1 == fd)
{
    perror("open");
    return 1;
}

printf("fd = %d\n", fd);

//2.read file
memset(buf, 0, SIZE);
ret = read(fd, buf, SIZE);
if (-1 == ret)
{
    perror("read");
    return 1;
}

printf("read len: %d;  %s\n", ret, buf);

//3.close file
close(fd);
```

## write  

```c
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t nbytes);

//Returns: number of bytes written if OK, −1 on error
```

The return value is **usually equal to** the nbytes argument; otherwise, an error has occurred. A common cause for a write error is either filling up a disk or exceeding the file size limit for a given process.  

examples:  

```c
int fd = -1;
int ret = -1;
char* str = "hello world";

// open file
fd = open("txt", O_WRONLY | O_CREAT, 0644);
if (-1 == fd)
{
    perror("open");
    return 1;
}

printf("fd = %d\n", fd);

// write into file
ret = write(fd, str, strlen(str));
if (-1 == ret)
{
    perror("write");
    return 1;
}

printf("write len: %d\n", ret);

// close file
close(fd);
```

## lseek  

## references  

Advanced Programming in the UNIX Environment, 3rd edition, Chapter 3  
