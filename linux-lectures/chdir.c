#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

# define SIZE 128

int main()
{
    int ret = -1;
    char buf[SIZE];

    //get current working memory
    memset(buf, 0, sizeof(buf));
    if (NULL == getcwd(buf, SIZE)) {
        perror("getcwd");
        return 1;
    }
    printf("buf: %s\n", buf);

    // change current working memory
    ret = chdir("/home/jys");
    if (-1 == ret) {
        perror("chdir");
        return 1;
    }

    //get current working memory
    memset(buf, 0, sizeof(buf));
    if (NULL == getcwd(buf, SIZE)) {
        perror("getcwd");
        return 1;
    }
    printf("buf: %s\n", buf);

    return 0;
}