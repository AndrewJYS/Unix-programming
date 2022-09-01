#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    unsigned int ret = 0;

    // set alarm first time
    ret = alarm(5);
    printf("ret: %d\n", ret);
    
    sleep(3);

    // set alarm second time
    ret = alarm(4);
    printf("ret: %d\n", ret);
    
    printf("please enter something\n");
    getchar();

    return 0;
}