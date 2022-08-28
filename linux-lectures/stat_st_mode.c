#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void show_file_type(struct stat* s) {
    if (S_ISREG(s->st_mode))
        printf("is a regular file\n");
    if (S_ISDIR(s->st_mode))
        printf("is a directory\n");
    if (S_ISCHR(s->st_mode))
        printf("is character device\n");
    if (S_ISBLK(s->st_mode))
        printf("is a block device\n");
    if (S_ISLNK(s->st_mode))
        printf("is a symbolic link\n");
    if (S_ISFIFO(s->st_mode)) // pipe
        printf("is a FIFO\n");
    if (S_ISSOCK(s->st_mode))
        printf("is a socket\n");

}

void show_permission(struct stat* s) {
    printf("user permission: ");
    //user read permission
    if (s->st_mode & S_IRUSR) 
        printf("r");
    else
        printf("-");
    //user write permission
    if (s->st_mode & S_IWUSR)
        printf("w");
    else
        printf("-");
    //user execute permission
    if (s->st_mode & S_IXUSR)
        printf("x");
    else
        printf("-");
}

int main(int argc, char** argv) {
    int ret =  -1;
    struct stat s;

    // judge
    if (2 != argc) {
        printf("usage: ./stat_st_mode filename\n");
        return 1;
    }

    //get file info
    ret = stat(argv[1], &s);
    if (-1 == ret) {
        perror("stat");
        return 1;
    }

    //show file type
    show_file_type(&s);
    //show file permission
    show_permission(&s);
    printf("\n");

    return 0;
}