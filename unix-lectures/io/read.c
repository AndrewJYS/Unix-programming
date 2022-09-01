#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SIZE 128

int main()
{
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

    return 0;
}