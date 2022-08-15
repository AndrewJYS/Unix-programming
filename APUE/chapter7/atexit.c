#include "apue.h"

static void my_exit1(void);
static void my_exit2(void);
static void my_exit3(void);

int main(void) {
    if (atexit(my_exit3) != 0)
        err_sys("cannot register my_exit3");
    if (atexit(my_exit2) != 0)
        err_sys("cannot register my_exit2");
    if (atexit(my_exit2) != 0)
        err_sys("cannot register my_exit2");
    if (atexit(my_exit1) != 0)
        err_sys("cannot register my_exit1");
    return(0);
}

static void my_exit1(void) {
    printf("first exit handler\n");
}

static void my_exit2(void) {
    printf("second exit handler\n");
}

static void my_exit3(void) {
    printf("third exit handler\n");
}