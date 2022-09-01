// inter process communication: read

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
    int fd = -1;
    int ret = -1;
    void* addr = NULL;

    // open a file with read-and-write
    fd = open("txt", O_RDWR);
    if (-1 == fd) {
        perror("open");
        return 1;
    }
    //if no ftruncate, then bus error will be thrown
    ftruncate(fd, 1024);

    // memory mapping
    addr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    printf("mmap ok\n");

    //close file
    close(fd);

    // read mapped memory
    printf("addr: %s\n", (char* )addr);

    // disconnect mmap
    munmap(addr, 1024);

    return 0;
}