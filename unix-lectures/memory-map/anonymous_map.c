#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main()
{
    int fd = -1;
    int ret = -1;
    void* addr = NULL;
    pid_t pid = -1;

    // create anonymous map
    addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    printf("create anonymous map\n");

    // create child process
    pid = fork();
    if (-1 == pid) {
        perror("fork");
        return 1;
    }

    //child-parent communication
    if (0 == pid) {
        //child write
        memcpy(addr, "1234567890", 10);
    }
    else {
        // parent read
        wait(NULL);
        printf("parent process: %s\n", (char* )addr);
    }

    //disconnect mapping
    munmap(addr, 4096);

    return 0;
}