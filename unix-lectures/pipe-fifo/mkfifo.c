#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    int ret = -1;

    // create an FIFO
    ret = mkfifo("fifo", 0644);
    if (-1 == ret) {
        perror("mkfifo");
        return 1;
    }

    printf("create an FIFO\n");

    return 0;
}