#include "apue.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

char buf[] = "abcdefghij";

int main(void) {
    int fd;
    if ((fd = creat("dup.test", FILE_MODE)) < 0)
        err_sys("create error");
    
    printf("%d\n", fd);
    int rc = dup(fd);
    printf("%d\n", fd);
    printf("%d\n", rc);
    
    if (write(rc, buf, 10) != 10)
    	err_sys("write error");
    	
    return 0;
}
