#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>

void show_file_type(unsigned short t)
{
    if (t == DT_BLK) 
        printf("This is a block device.");
    if (t == DT_CHR) 
        printf("This is a character device.");
    if (t == DT_DIR) 
        printf("This is a directory.");
    if (t == DT_FIFO) 
        printf("This is a named pipe (FIFO).");
    if (t == DT_LNK) 
        printf("This is a symbolic link.");
    if (t == DT_REG) 
        printf("This is a regular file.");
    if (t == DT_SOCK) 
        printf("This is a UNIX domain socket.");
    if (t == DT_UNKNOWN) 
        printf("The file type could not be determined.");
}

int main()
{
    DIR* dir = NULL;
    struct dirent* d = NULL;

    //open directory
    if ( (dir = opendir("/home/jys/unix-programming/course")) == NULL) {
        perror("opendir");
        return 1;
    }
    printf("successfully open the directory\n");

    //read the directory in a loop
    while (1) {
        d = readdir(dir);
        if (NULL == d) {
            perror("readdir");
            break;
        }
        show_file_type(d->d_type);
        printf("  d_name:%s\n", d->d_name);
    }

    //close the directory
    closedir(dir);

    return 0;
}