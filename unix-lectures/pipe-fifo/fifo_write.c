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