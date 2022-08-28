#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd = -1;

    //read-only, if the file does not exist, then throw error
    //fd = open("txt", O_RDONLY);

    //write-only, if the file exists, the open it;
    //if the file does not exist, then create a new file, and set permission 0644
    //fd = open("txt", O_WRONLY | O_CREAT, 0644);

    //write-only, if the file exists, then throw error
    //if the file does not exist, then create a new file, and set permission 0644
    //fd = open("txt", O_WRONLY | O_CREAT | O_EXCL, 0644);

    //read-and-write, ...
    //fd = open("txt", O_RDWR | O_CREAT, 0644);

    //write-only, if the file does not exist, then create a new file, and set permission 0644
    //if the file exists, then clear the file
    //fd = open("txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);

    //if the file does not exist, then throw error
    //if the file exists, then set mode 'APPEND when writing'
    fd = open("txt", O_WRONLY | O_APPEND);

    if (-1 == fd)
    {
        perror("open");
        return 1;
    }

    printf("fd = %d\n", fd);

    close(fd);

    return 0;
}