#include "apue.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int fd;
    if ((fd = creat("dup.test", FILE_MODE)) < 0)
        err_sys("create error");
    
    printf("%d\n", fd);
    int rc = dup(fd);
    printf("%d\n", fd);
    printf("%d\n", rc);
    return 0;
}