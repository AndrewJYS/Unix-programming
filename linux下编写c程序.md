# linux下编写C程序  

## gcc编译  

-g：包含调试信息  
-On（n=0-3）：编译优化（n越大，优化越大）  
-Wall：显示所有警告  
-Werror：将警告当作错误处理  
-D：编译时定义宏  
-o：表示指定生成可执行文件的名字  

```shell
gcc test.c -Wall test
gcc test.c -o test
```

## 静态链接和动态链接  

静态链接：由链接器在链接时将库的内容加入可执行文件中  
动态链接：链接器在链接时仅仅建立与所需库函数之间的链接关系，在程序运行时才将所需资源调入可执行程序  
编译时默认选择动态链接  

```shell
gcc test.c -o test   动态链接
gcc -static test.c -o test  静态链接
```

## 静态库  

### 静态库的制作  

静态库可以认为是一些目标代码的集合，是在可执行程序运行前就已经加入执行代码中，成为执行程序的一部分。按照习惯，一般以".a"为文件后缀名。静态库的名命名一般分为三个部分：  

前缀：lib  
库名称：自己定义  
后缀：.a  
因此最后的静态库的名字应该是:libXXX.a  

将源文件（.h, .c等)通过预编译，编译，汇编生成目标文件，再经过编号，索引，压缩打包，成为静态库  

假设我们有add.c, add.h, sub.c, sub.h, mul.c, mul.h, div.c, div.h（add.c, add.h如下，其他以此类推）这些代码文件  

```c
#ifndef __ADD_H__
#define __ADD_H__

int add(int x, int y);

#endif // __ADD_H__
```

```c
#include "add.h"

int add(int x, int y)
{
    return x + y;
}
```

将它们制作成静态库，第一步操作如下：  

```shell
gcc -c add.c -o add.o
gcc -c sub.c -o sub.o
gcc -c mul.c -o mul.o
gcc -c div.c -o div.o
```

第二步需要ar命令，其中-r表示更新，-c表示创建，-s表示建立索引  

```shell
ar -rcs libtest.a add.o sub.o mul.o div.o
```

### 静态库的使用  

会给客户头文件（add.h, sub.h, mul.h, div.h)，这些头文件，和main.c，还有静态库会一起编译。编译后，可以只留下main可执行文件，也可以正常执行了  

```shell
gcc main.c -I./ -L./ -ltest -o main

-I: 头文件目录  
-L：链接库的目录  
-l：指定要链接的库名（去除lib,去除后缀）  
```

## 动态库  

### 动态库的制作  

动态库在程序编译时并不会链接到目标代码中，而是在程序运行时被载入。不同的应用程序如果调用相同的库，那么在内存里只需要有一个共享库的实例，规避了空间浪费问题  

动态库在程序运行时被载入，也解决了静态库对程序的更新，部署，和发布带来的麻烦。用户只要更新动态库即可，实现了增量更新  

按照习惯，动态库一般以".so"为文件后缀名。动态库的名命名一般分为三个部分：  

前缀：lib  
库名称：自己定义  
后缀：.so  
因此最后的动态库的名字应该是:libXXX.so  

假设我们有add.c, add.h, sub.c, sub.h, mul.c, mul.h, div.c, div.h（内容同[静态库](静态库)中的说明），现在要制作成动态库，第一步如下：  

```shell
gcc -fPIC -c add.c
gcc -fPIC -c sub.c
gcc -fPIC -c mul.c
gcc -fPIC -c div.c
```

第二步，生成共享库  

```shell
gcc -shared add.o sub.o mul.o div.o -o libtest.so
```

可以通过nm命令查看对应的函数  

```shell
nm libtest.so | grep add
```

### 动态库测试  

动态库使用过程中，用它的绝对路径

1.临时配置  

会给客户头文件（add.h, sub.h, mul.h, div.h)，和动态库，main.c一起编译。编译后，动态库不能删除，头文件可以删除  

```shell
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jys/share   （PATH动态库所在文件夹）
gcc main.c -L. -I. -ltest -o main

-I: 头文件目录  
-L：链接库的目录  
-l：指定要链接的库名（去除lib,去除后缀） 
```

2.永久配置  

将上面的export命令写入配置文件  

```shell
vim ~/.bashrc
.bashrc文件末添加export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jys/share 
source ~/.bashrc
gcc main.c -L. -I. -ltest -o main
```

3.符号链接  

```shell
sudo ln -s /home/jys/share/libtest.so /lib/libtest.so
gcc main.c -L. -I. -ltest -o main
```

## 参考  

[相关课程](https://www.bilibili.com/video/BV1Yo4y1D7Ap?p=37&vd_source=4b75b13c678ed297c8d0ed42e806f46b)  
