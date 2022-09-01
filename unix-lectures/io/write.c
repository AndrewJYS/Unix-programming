#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
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

    //write into file
    ret = write(fd, str, strlen(str));
    if (-1 == ret)
    {
        perror("write");
        return 1;
    }

    printf("write len: %d\n", ret);

    // close file
    close(fd);

    return 0;
}