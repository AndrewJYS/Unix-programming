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
    char buf[SIZE];

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