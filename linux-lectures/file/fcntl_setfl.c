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