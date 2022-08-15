#include "apue.h"

int main(void) {
    float data[10];
    for (int i = 0; i < 10; i++)
        data[i] = i + 1;

    if(fwrite(&data[2], sizeof(float), 4, stdout) != 4)
        err_sys("input error");
    
    exit(0);
}