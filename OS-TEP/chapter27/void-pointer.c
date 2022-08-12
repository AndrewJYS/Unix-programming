#include <stdio.h>

void displayString(char* args) {
    printf("%s\n", args);
}

void displayInt(int* args) {
    printf("%d\n", *args);
}

void displayInt2(void* args) {
    int m = (int)(long long) args;
    printf("%d\n", m);
}

int main(int argc, char* argv[]) {
    displayString("AAAA");
    char* s = "BBBB";
    displayString(s);

    //displayInt(1234);  //error
    int i;
    i = 2000;
    displayInt(&i);
    displayInt2((void* )(long long) i);

    return 0;
}