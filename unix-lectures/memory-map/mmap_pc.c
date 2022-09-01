// parent and child communication with mmap

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main()
{
    int fd = -1;
    int ret = -1;
    pid_t pid = -1;
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

    //create a child process
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }
    
    if (0 == pid) {
        // child process
        //write into mapped memory, also into file
        memcpy(addr, "1234567890", 10);
    }
    else {
        //parent
        wait(NULL);
        printf("addr:%s\n", (char* )addr);
    }

    // disconnect mmap
    munmap(addr, 1024);

    return 0;
}