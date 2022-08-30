#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

int main()
{
    DIR* dir = NULL;

    //open directory
    dir = opendir("test");
    if (NULL == dir) {
        perror("opendir");
        return 1;
    }
    printf("successfully open directory");

    // close directory
    if (closedir(dir) < 0) {
        perror("closedir");
        return 1;
    }

    return 0;
}