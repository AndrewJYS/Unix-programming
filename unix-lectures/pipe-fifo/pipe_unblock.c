#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 128

int main()
{
    int ret = -1;
    int fds[2];
    pid_t pid = -1;
    char buf[SIZE];

    // create pipe
    ret = pipe(fds);
    if (-1 == ret) {
        perror("pipe");
        return 1;
    }

    // create child process
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    // child
    if (0 == pid) {
        // close write
        close(fds[1]);

        //unblocked read endian
        int flags = fcntl(fds[0], F_GETFL);
        flags |= O_NONBLOCK;
        fcntl(fds[0], F_SETFL, flags);

        // read
        memset(buf, 0, SIZE);
        ret = read(fds[0], buf, SIZE);
        if (ret < 0) {
            perror("read");
            exit(-1);
        }

        printf("child process: buf = %s\n", buf);

        //close read
        close(fds[0]);

        //exit child
        exit(0);
    }

    //parent
    sleep(1);
    //close read
    close(fds[0]);

    //write
    ret = write(fds[1], "ABCDEFGHIJ", 10);
    if (-1 == ret) {
        perror("write");
        return 1;
    }

    printf("parent process write len: %d\n", ret);

    //close write
    close(fds[1]);

    return 0;
}