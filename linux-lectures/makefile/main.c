#include <stdio.h>
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"

int main()
{
    printf("5 + 5 = %d\n", add(5, 5));
    printf("5 - 5 = %d\n", sub(5, 5));
    printf("5 * 5 = %d\n", mul(5, 5));
    printf("5 / 5 = %d\n", div(5, 5));
    
    return 0;
}