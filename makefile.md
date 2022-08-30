# Makefile  

## 简介  

Makefile定义了一系列规则来指定文件的编译顺序和其他操作，它像shell脚本一样，也可以执行操作系统的命令。Makefile带来的好处是自动化编译，一旦写好，只需要一个make命令，整个工程完成自动编译，极大提高了软件开发效率。（make是命令工具，用来解释makefile中的指令。）  

make主要解决两个问题  
（1）大量代码的关系维护
（2）减少重复编译时间（比如在大型文件中修改了一行代码，就没有必要完全重新编译，make能判断哪些文件修改过，可以只对该文件进行重新编译，然后重新链接所有的目标文件，节省编译时间）  

Makefile的命令规则：makefile和Makefile都可以，但推荐使用Makefile  

## Makefile语法规则  

### 一条规则  

```shell
目标：依赖文件列表
<Tab>命令列表
```

1.目标：通常是要产生的文件名称，目标可以是可执行文件或是其他obj文件，也可以是一个动作的名称  
2.依赖文件：用来输入从而产生目标的文件；一个目标通常由几个依赖文件，也可以没有  
3.命令：make执行的动作，一个规则可以含几个命令（可以没有）；由多个命令时，每个命令占一行  

下面给出示例：新建一个文件1.mk，代码如下  

```vim
all:test1
    echo "hello all"
```

使用make编译  

```shell
make -f 1.mk
```

输出如下：  

```shell
make: *** No rule to make target 'test1', needed by 'all'.  Stop.
```

原因是目标all有依赖test1，但是在代码中没有找到test1，修改代码  

```makefile
all:test1 test2
    echo "hello all"

test1:
    echo "hello test1"

test2:
    echo "hello test2"
```

输出如下：  

```shell
echo "hello test1"
hello test1
echo "hello test2"
hello test2
echo "hello all"
hello all
```

## make的命令格式  

```shell
make [-f file] [options] [targets]
```

[-f file]  
make默认在工作目录中寻找名为GNUmakefile, makefile, Makefile的文件作为make的输入文件  
-f可以指定以上名字之外的文件作为make输入文件  

[options]  
-v：显示make工具的版本信息  
-w：在处理makefile之前和之后显示工作路径  
-C dir：读取makefile之前改变工作路径到dir  
-n：只打印要执行的命令但不执行  
-s：执行但不显示执行的命令  

[targets]  
如果使用make命令时没有指定目标，则make工具会默认实现makefile文件中的第一个目标，然后退出  
如果指定了make工具要实现的目标，目标可以是一个或多个，多个目标用空格隔开  

## 编译项目示例  

回顾静态库和动态库的制作时，写的add.c, sub.c, div.c, mul.c, 和main.c，现在对它们进行编译  

先创建Makefile文件  

```makefile
main:add.o sub.o mul.o div.o main.o
    gcc add.o sub.o mul.o div.o main.o -o main

add.o:add.c
    gcc -c add.c -o add.o

sub.o:sub.c
    gcc -c sub.c -o sub.o

mul.o:mul.c
    gcc -c mul.c -o mul.o

div.o:div.c
    gcc -c div.c -o div.o

main.o:main.c
    gcc -c main.c -o main.o
```

编译  

```shell
make
```

输出  

```shell
gcc -c add.c -o add.o
gcc -c sub.c -o sub.o
gcc -c mul.c -o mul.o
gcc -c div.c -o div.o
gcc -c main.c -o main.o
gcc add.o sub.o mul.o div.o main.o -o main
```

使用中间文件.o的目的是，当某个.c文件修改后，重新编译不需要从头编译，例如，我们修改div.c文件后重新编译，只会对div.c重新编译，然后链接  

```shell
jys@ubuntu:~/unix-programming/course/makefile$ make
gcc -c div.c -o div.o
gcc add.o sub.o mul.o div.o main.o -o main
```

## makefile中的变量  

注意到上述例子中，频繁的写.o文件很麻烦，也容易写漏，因此引入变量。Makefile中使用变量有点像C语言中的宏定义，使用该变量就是内容替换。  

### 自定义变量  

1.定义变量方法  
变量名=变量值  

2.引用变量  
$(变量名)  或  ${变量名}  

3.makefile的变量名  
可以用数字开头  
变量是大小写敏感的  
变量一般都在makefile的头部定义  
变量几乎可以在makefile中的任何地方使用  

下面重写上述例子  

```makefile
OBJS = add.o sub.o mul.o div.o main.o

main:$(OBJS)
    gcc $(OBJS) -o main

add.o:add.c
    gcc -c add.c -o add.o

sub.o:sub.c
    gcc -c sub.c -o sub.o

mul.o:mul.c
    gcc -c mul.c -o mul.o

div.o:div.c
    gcc -c div.c -o div.o

main.o:main.c
    gcc -c main.c -o main.o

clean:
    rm -rf $(OBJS) main
```

```shell
jys@ubuntu:~/unix-programming/course/makefile$ make
gcc -c add.c -o add.o
gcc -c sub.c -o sub.o
gcc -c mul.c -o mul.o
gcc -c div.c -o div.o
gcc -c main.c -o main.o
gcc add.o sub.o mul.o div.o main.o -o main
jys@ubuntu:~/unix-programming/course/makefile$ make clean
rm -rf add.o sub.o mul.o div.o main.o main
jys@ubuntu:~/unix-programming/course/makefile$ 
```

### 三个自动变量  

```md
$@：表示目标
$<：表示第一个依赖
$^：表示所有依赖
```

这些变量不能单独使用，必须在命令中使用  

```makefile
OBJS = add.o sub.o mul.o div.o main.o

main:$(OBJS)
    gcc $^ -o $@

add.o:add.c
    gcc -c $< -o $@

sub.o:sub.c
    gcc -c $< -o $@

mul.o:mul.c
    gcc -c $< -o $@

div.o:div.c
    gcc -c $< -o $@

main.o:main.c
    gcc -c $< -o $@

clean:
    rm -rf $(OBJS) main
```

### 模式规则  

使用模式匹配进一步优化上述代码  

```makefile
OBJS = add.o sub.o mul.o div.o main.o

main:$(OBJS)
    gcc $^ -o $@

# 模式匹配 所有的.o文件都依赖对应的.c
# 将所有的.c生成对应.o文件
%.o:%.c
    gcc -c $< -o $@

clean:
    rm -rf $(OBJS) main
```

## makefile中的两个函数  

add.o sub.o mul.o div.o main.o，一个个列举容易遗漏，因此引入函数  

**wildcard**：查找指定目录下的指定类型的文件  
**patsubst**：匹配替换  

```makefile
# 获取当前目录下所有的.c文件
SRC=$(wildcard ./*.c)

# 将SRC中所有出现的.c匹配到对应的.o
OBJS=$(patsubst %.c, %.o, $(SRC))

main:$(OBJS)
    gcc $^ -o $@

%.o:%.c
    gcc -c $< -o $@

clean:
    rm -rf $(OBJS) main
```

## 伪目标  

```shell
jys@ubuntu:~/unix-programming/course/makefile$ make
gcc -c mul.c -o mul.o
gcc -c main.c -o main.o
gcc -c add.c -o add.o
gcc -c div.c -o div.o
gcc -c sub.c -o sub.o
gcc mul.o main.o add.o div.o sub.o -o main
jys@ubuntu:~/unix-programming/course/makefile$ touch clean
jys@ubuntu:~/unix-programming/course/makefile$ make clean
make: 'clean' is up to date.
```

如果目录中已经存在clean文件，那么make clean失效，因此引入伪目标  

```makefile
.PHONY:clean
```

声明为伪目标后，makefile将不再判断目标是否在当前目录存在，或该目标是否需要更新  

```makefile
# version-5
SRC=$(wildcard ./*.c)
OBJS=$(patsubst %.c, %.o, $(SRC))

main:$(OBJS)
    gcc $^ -o $@

%.o:%.c
    gcc -c $< -o $@

.PHONY:clean
clean:
    rm -rf $(OBJS) main
```

## 其他  

在命令中的特殊符号如下：  

-：若本条命令出错，make也会执行后续的命令，比如"-rm main.o"  
@：不显示命令本身，只显示结果，比如“@echo clean done"  

```makefile
SRC=$(wildcard ./*.c)
OBJS=$(patsubst %.c, %.o, $(SRC))

main:$(OBJS)
    gcc $^ -o $@

%.o:%.c
    @gcc -c $< -o $@

.PHONY:clean
clean:
    -rm -rf $(OBJS) main
```

```shell
jys@ubuntu:~/unix-programming/course/makefile$ make
gcc mul.o main.o add.o div.o sub.o -o main
jys@ubuntu:~/unix-programming/course/makefile$ make clean
rm -rf  ./mul.o  ./main.o  ./add.o  ./div.o  ./sub.o main
```

## 参考  

[相关课程](https://www.bilibili.com/video/BV1Yo4y1D7Ap?p=65&spm_id_from=pageDriver&vd_source=4b75b13c678ed297c8d0ed42e806f46b)  
