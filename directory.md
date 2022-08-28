# directory-related  

## getcwd  

```c
#include <unistd.h>

char *getcwd(char *buf, size_t size);

//Returns: buf if OK, NULL on error
```

We must pass to this function the address of a **buffer**, buf, and **its size (in bytes)**.  

## chdir  

```c
#include <unistd.h>

int chdir(const char *pathname);

//return: 0 if OK, −1 on error
```

Each program is run in a separate process, so the **current working directory of the shell** is **unaffected** by the call to **chdir** in the program.  

example for getcwd and chdir  

```c
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
```

## mkdir, rmdir  

```c
#include <sys/stat.h>

int mkdir(const char *pathname, mode_t mode);

//return: 0 if OK, −1 on error
```

```c
#include <unistd.h>

int rmdir(const char *pathname);

//Returns: 0 if OK, −1 on error
```

## opendir, closedir, readdir  

```c
#include <dirent.h>

DIR *opendir(const char *pathname);
//return: pointer if OK, NULL on error

struct dirent *readdir(DIR *dp);
//Returns: pointer if OK, NULL at end of directory or error

int closedir(DIR *dp);
//Returns: 0 if OK, −1 on error
```

The **dirent structure** defined in **\<dirent.h\>** is implementation dependent. Implementations define the structure to contain at least the following two members:  
ino_t d_ino;        \/\* i-node number \*\/  
char d_name[];      \/\* null-terminated filename \*\/  
  
example for opendir and closedir  

```c
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
```

example for readdir, we read content in a selected directory **Circularly**  

```c
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
```

## references  

Advanced Programming in the UNIX Environment, 3rd edition, Chapter 4  
