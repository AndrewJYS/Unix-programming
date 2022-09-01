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