# shell常用命令  

## Bash解析器常用快捷键  

tab键：补齐命令，补齐路径  
clear：清屏  
CTRL+c：中断终端的操作  
CTRL+a：光标移到头部  
CTRL+e：光标移到尾部  
CTRL+h：删除光标前边的**一个**字符  
CTRL+d：删除光标后边的**一个**字符  
CTRL+u：删除光标前边的**所有**字符  
CTRL+k：删除光标后边的**所有**字符  

## linux命令格式  

```shell
command [-options] [parameter1] ...
```

options前面如果是--，那么options通常是个完整的单词，如果是-，通常是单词的缩写  

## 帮助文档查看说明  

内建命令：用 "help 命令"  
外部命令：用 "命令 --help"  

```shell
help cd
ls --help
```

也可以使用man命令，man是linux提供的一个手册，包含了绝大部分的命令和函数的使用说明，其格式如下  

```shell
man [options] command
```

man手册分成很多章节（具体可以通过man man命令来查看章节代号和章节内容摘要），使用man的时候可以指定不同的章节来浏览不同的内容，例如  

```shell
man 1 printf  会显示标准命令printf的说明（也就是printf在man手册上的第1页内容，后面几条命令的数字标记的含义以此类推，如果省略1，那么会跳出默认页文档）  
man 1 ls  会显示标准命令ls的说明  
man 2 read  会显示库函数read的说明
man 3 printf  会显示库函数中的printf使用说明  
man -a printf 浏览printf的所有页的介绍
man -k printf 浏览与printf所有相关的页（比如fprintf）
```

## type命令  

type命令用来查看一个命令是内建命令还是外部命令（内建命令比外部命令效率高）  

```shell
type -a cd
输出：cd is a shell builtin （cd是内建命令） 

type -a ls
输出：（没有出现内建命令字样，就是外部命令）  
ls is aliased to `ls --color=auto'
ls is /usr/bin/ls
ls is /bin/ls
```

有些命令即是内建命令，又是外部命令，此时系统会优先使用内建命令，因为内建命令效率高  

## 目录相关的命令  

```shell
pwd: 查看当前工作目录  
```

```shell
cd: 改变工作目录  
cd(什么都不加)：切换到当前目录的家目录  
cd ~：切换到当前目录的家目录  
cd -：切换到上一次进入的目录  
cd .：切换到当前目录  
cd ..：切换到当前目录的上一级目录  
```

```shell
mkdir test  在当前目录中创建一个test目录  
mkdir /tmp/test  在根目录下的tmp目录下创建一个test目录  
mkdir file{1..100}  在当前目录创建file1, file2, ..., file100目录  
mkdir "file{1..100}"  在当前目录创建file{1..100}目录  
mkdir a b 在当前目录创建a和b目录  
mkdir -p a/b/c 在当前目录下新建a/b/c目录（如果目录a和b不存在，缺省-p会报错，加上-p则会递归创建）
```

```shell
rmdir的示例和mkdir一一对应，不再详述  
如果创建了a/b/c，此时使用如下两个命令  
rmdir a/b/c  只会删除目录c  
rmdir -p a/b/c  会递归删除目录c, b, a  
```

## 文件相关命令  

### ls  

```shell
ls [-options...] [directory_name...]  
```

-a 显示所有文件及目录 (. 开头的隐藏文件也会列出)  
-l 除文件名称外，亦将文件型态、权限、拥有者、文件大小等资讯详细列出  
-r 将文件以相反次序显示(原定依英文字母次序)  
-t 将文件依建立时间之先后次序列出  
-A 同 -a ，但不列出 "." (目前目录) 及 ".." (父目录)  
-F 在列出的文件名称后加一符号；例如可执行档则加 "*", 目录则加 "/"  
-R 若目录下有文件，则以下之文件亦皆依序列出  

options也可以组合，例如：  

-al 显示所有文件包括隐藏文件的列表
-lh 显示计算大小以kb为单位的文件列表  
-ld 查看目录本身的属性  

**注意：**  
如果a是个目录，那么ls -l a 会显示a目录下的文件，要查看目录本身的信息，需要用ld选项  
如果a是个普通文件，那么ls -l a 会显示a的信息  

### touch  

该命令有两个作用，一是用于把已存在文件的时间标签更新为系统当前的时间（默认方式），它们的数据将原封不动地保留下来；二是用来创建新的空文件。  

### cp  

```shell
cp [options] source(可以是多个文件) dest  
```

-a：此选项通常在复制目录时使用，它保留链接、文件属性，并复制目录下的所有内容。其作用等于dpR参数组合。  
-d：复制时保留链接。这里所说的链接相当于 Windows 系统中的快捷方式。  
-f：覆盖已经存在的目标文件而不给出提示。  
-i：与 -f 选项相反，在覆盖目标文件之前给出提示，要求用户确认是否覆盖，回答 y 时目标文件将被覆盖。  
-p：除复制文件的内容外，还把修改时间和访问权限也复制到新文件中。  
-r：若给出的源文件是一个目录文件，此时将复制该目录下所有的子目录和文件。  
-l：不复制文件，只是生成链接文件。  
-v：显示拷贝进度  

同样的，选项也可以组合  

### rm  

```shell
rm [options...] [name...(可以是多个文件)]  
```

-i 删除前逐一询问确认。（由于删除后很难恢复，因此最好加上-i）  
-f 即使原档案属性设为只读，亦直接删除，无需逐一确认。  
-r 将目录及以下之档案亦逐一删除。  

rm不能删除一个非空目录，如要删除，需要加上-r  

### mv  

mv（英文全拼：move file）命令用来为文件或目录改名、或将文件或目录移入其它位置。  

```shell
mv [options] source dest  
mv [options] source... directory  
```

-b: 当目标文件或目录存在时，在执行覆盖前，会为其创建一个备份。  
-i: 如果指定移动的源目录或文件与目标的目录或文件同名，则会先询问是否覆盖旧文件，输入 y 表示直接覆盖，输入 n 表示取消该操作。  
-f: 如果指定移动的源目录或文件与目标的目录或文件同名，不会询问，直接覆盖旧文件。  
-n: 不要覆盖任何已存在的文件或目录。  
-u：当源文件比目标文件新或者目标文件不存在时，才执行移动操作。  
-v：显示移动进度  

由于覆盖后文件无法恢复，最好加上-i  

## 文件查看命令  

### cat  

将文件内容一次性输出到终端，如果文件太长则无法全部显示  

```shell
cat /etc/passwd 查看/etc/passwd中内容  
cat -n /etc/passwd 查看/etc/passwd中内容 对输出的所有行编号  
cat -b /etc/passwd 查看/etc/passwd中内容 对非空行编号  
```

### less  

将文件内容分页显示到终端，可以自由上下浏览  

### head  

从文件头部查看前n行的内容，如果没有指定行数，默认显示前10行内容  

```shell
head -20 /etc/passwd  显示/etc/passwd前20行内容  
head -n 20 /etc/passwd 与head -20 /etc/passwd 等价  
```

### tail  

tail从文件尾部显示n行的内容，默认10行，用法与head类似  

## du和df命令  

du用于查看某个目录的大小（注意与ls中检查目录大小的不同，du是检查目录实际所占空间，而ls是返回目录属性的大小）  

```shell
du [options] [filename]  
```

-a：递归显示指定目录中各文件和子目录中文件占用的数据块  
-s：显示指定文件或目录占用的数据块  
-b：以字节为单位显示磁盘占用情况  
-h：以kb, mb, gb为单位  

df用于检测文件系统的磁盘占用和空余情况，可以显示所有文件系统对节点和磁盘块的使用情况  

-a：显示所有文件系统的磁盘使用情况  
-m：以1024字节为单位  
-h：以kb，mb，gb为单位  

## find  

通常用来在特定的目录下搜索符合条件的文件，也可以用来搜索特定用户属主的文件  

### 按文件名查询  

```shell
find + 路径 -name + 文件名  
```

示例如下：  

```shell
find /home -name "a.txt"  
find /home -name "*.txt"  
find / -maxdepth 2 -name grub.conf  （最多查2级目录）
find / -mindepth 2 -name grub.conf  （最少查2级目录）
```

### 按文件大小查询  

```shell
find + 路径 + -size + 范围  
```

范围的填写按照如下规则  
+表示大于  
-表示小于  
没有符号表示等于  
M必须大小（表示Mb）  
k必须小写（表示kb）  

示例如下：

```shell
find /home -size +100M
find /home -size +50k -size -100k  大于50k但小于100k
```

### 按照文件类型查询  

```shell
find + 路径 + -type + 类型  
```

f 普通文件  
d 目录  
l 符号链接  
b 块设备文件  
c 字符设备类型  
s 网络套接字  
p 管道  

示例如下：  

```shell
find /home -type p
```

### 其他  

```shell
find . -perm 777  查找当前目录下权限为777的目录
find / -empty  查找空目录或空白文件
find . -name "*.conf" | xargs rm -rf  删除当前目录下所有以.conf结尾的文件
```

## grep  

grep是linux系统中的文本搜索工具，grep允许对文本文件进行模式查找，如果找到匹配模式，则打印包含模式的所有行  

```shell
grep [-options] '搜索内容串' 文件名
```

-v：显示不包含匹配文本的所有行（求反操作）  
-n：显示匹配行和行号  
-i：忽略大小写  
-r：递归遍历目录  

## 管道  

管道（|）：一个命令的输出可以通过管道作为另一个命令的输入  

```shell
ifconfig | grep "en33"  在ifconfig的输出中匹配"en33"
```

## 压缩包管理  

### tar  

tar是unix/linux中最常用的备份工具，此命令可以把一系列文件归档到一个大文件中，也可以把档案文件解开以恢复数据  

```shell
tar [-options] 打包文件名 文件
```

-c：生成档案文件，创建打包文件  
-v：列出归档解档的详细过程，显示进度  
-f：指定档案文件名称，f后面一定是.tar文件，所以必须放选项最后  
-t：列出档案包含的文件  
-x：解开档案文件  
**注意**：除了参数f要放在最后，其他参数的顺序随意  

常用方法示例如下：  

```shell
tar -cvf test.tar 1.txt 2.txt 3.txt  
tar -xvf test.tar
tar -tvf test.tar 可以查看test.tar中包含的文件
```

### gzip  

tar和gzip命令结合使用实现文件的打包和压缩。tar只负责打包文件，但不压缩，用gzip压缩tar打包后的文件，其扩展名一般用xxxx.tar.gz（压缩后会删除原文件）  

```shell
gzip [-options] 被压缩文件
```

-d：解压（也可以用gunzip命令）  
-r：压缩目录下的所有文件  

如果用tar和gzip实现文件的打包和压缩，需要两步。若是使用tar的-z参数，可以更加便捷，-z表示使用gzip命令（解压缩同理只需一步），示例如下  

```shell
tar -czvf test.tar.gz a b c d e  将a b c d e五个文件打包并压缩
tar -xzvf test.tar.gz 解压
```

如果要解压到某个目录下  

```shell
tar -xzvf test.gz -C my_file  将test.gz解压到my_file文件夹中
```

**注意**：和gzip命令类似的命令是bzip2

### bzip2  

bzip2命令和gzip命令类似，也用于压缩和解压缩文件，一般与tar一起使用。但tar通过参数可以隐含调用bzip2，从而简化命令，示例如下：  

```shell
tar -cjvf test.tar.bz2 a b c d e  将a b c d e五个文件打包并压缩
tar -xjvf test.tar.bz2 解压
```

还有一个万能解压命令，既可以解压gz文件，也可以解压bz2文件  

```shell
tar -xvf 压缩文件名
```

## 文件权限管理  

### chmod  

改变文件权限模式  

```shell
chmod u/g/o/a +/-/= rwx 文件
```

u：表示文件的所属者  
g：所属组  
o：其他  
a：以上三者都是  
+：增加权限  
-：减少权限  
=：设置权限  

示例：  

```shell
chmod o+w a
chmod g-x a
chmod o=x a （设置为只有x权限）
chmod u=rw, g=rw, o=r a
```

上面的设置方法是“字母法”，下面介绍“数字法”  

7: rwx  
6: rw-  
5: r-x  
4: r--  
3: -wx  
2: -w-  
1: --x  
0: ---

示例如下：  

```shell
chmod u=7, g=5, o=4 a
等同于
chmod 0754 a  （不要忘记前面的0，表示8进制的开始符）

chmod -R 0754 test （不仅改变test文件夹的权限，还递归修改test内所有文件的权限）
```

### chown  

用于修改文件所有者  

```shell
chown 用户名 文件或目录名  
```

### chgrp  

修改文件所属组  

```shell
chgrp 用户组名 文件或目录名
```

## 软件安装  

### 在线  

```shell
sudo apt install tree  安装tree
sudo apt remove tree 卸载tree
sudo apt update 列出所有可更新的软件清单
sudo apt upgrade 更新系统已安装的软件包
sudo apt update <package_name> 更新指定的软件包
sudo apt clean 清除作为更新过程的一部分而下载的冗余包文件
```

### 离线  

```shell
sudo dpkg -i test.deb    安装test.deb（i表示install)
sudo dpkg -r tree      卸载tree（r表示remove）
```

## 重定向  

```shell
cat /etc/passwd > a.txt  将cat /etc/passwd输出到a.txt，而且会覆盖a.txt中的内容
cat /etc/passwd >> /home/a.txt 将cat /etc/passwd输出追加到到/home/a.txt
llll 2> err.txt  将llll命令的错误信息重定向到err.txt
```

## ln  

软链接：  
(1)可以应用于目录  
(2)可以跨文件系统  
(3)不会增加被链接文件的链接次数  
(4)大小为指定的**绝对路径**所包含的字符总数  
(5)有自己的inode号  
(6)权限无关紧要  

硬链接：  
(1) 硬链接，以文件副本的形式存在。但不占用实际空间。  
(2) 不允许给目录创建硬链接。  
(3) 硬链接只有在同一个文件系统中才能创建。  
(4) 删除其中一个硬链接文件并不影响其他有相同 inode 号的文件。  

```shell
ln 源文件 链接文件    硬链接
ln -s 源文件 链接文件    软链接
```

## 进程相关命令  

### ps  

-a：显示终端上的所有进程，包括其他用户的进程  
-u：显示进程的详细状态  
-x：显示没有控制终端的进程  
-w：显示加宽，以便显示更多信息  
-r：只显示正在运行的进程  

常用组合如下：  

```shell
ps -aux  显示所有包含其他用户的进程
ps -ef （与ps -aux等价）
ps -ef | grep sleep  筛查进程名为sleep的进程
```

### top  

可以动态地显示运行的进程（按q可以退出）  

### kill  

```shell
kill 12345          杀死进程  
kill -KILL 123456   强制杀死进程  
kill -9 123456      彻底杀死进程  
```

### killall  

可以根据进程名杀死所有进程  

```shell
killall -9 sleep
```

## 其他命令  

### 计算器  

```shell
bc
```

示例如下：  

```shell
jys@ubuntu:~/unix-programming/course$ bc
bc 1.07.1
Copyright 1991-1994, 1997, 1998, 2000, 2004, 2006, 2008, 2012-2017 Free Software Foundation, Inc.
This is free software with ABSOLUTELY NO WARRANTY.
For details type `warranty'. 
2 + 5
7
3 ^ 4
81
```

## 参考  

Advanced Programming in the UNIX Environment, 3rd edition  
[相关课程](https://www.bilibili.com/video/BV1Yo4y1D7Ap?p=37&vd_source=4b75b13c678ed297c8d0ed42e806f46b)  
[软链接与硬链接](https://blog.csdn.net/qq_26129413/article/details/110228234)  
