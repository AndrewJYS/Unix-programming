#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    pid_t pid = -1;

    // get process group ID of a process’s session leader
    pid = getsid(0);
    if (-1 == pid) {
        perror("getsid");
        return 1;
    }
    printf("sid: %d\n", pid);

    return 0;

}