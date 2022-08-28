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

    // open file
    fd = open("txt", O_RDWR | O_CREAT, 0644);
    if (-1 == fd) {
        perror("open");
        return 1;
    }
    printf("fd = %d\n", fd);

    //write and lseek operation
    write(fd, "ABCDEFG", 7);

    ret = lseek(fd, 32, SEEK_SET);
    if (-1 == ret) {
        perror("lseek");
        return 1;
    }
    write(fd, "1234567890", 10);

    //read file
    lseek(fd, 0, SEEK_SET);
    memset(buf, 0, SIZE);
    ret = read(fd, buf, SIZE);
    printf("read ret: %d, buf: %s\n", ret, buf);

    //close file
    close(fd);

    return 0;
}