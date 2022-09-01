#include "div.h"
#include <stdio.h>

int div(int x, int y)
{
    if (y == 0) {
    	printf("error");
    	return 0;
    }
    
    return x / y;
}
