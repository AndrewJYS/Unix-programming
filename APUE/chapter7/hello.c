#include <stdio.h>
#include "apue.h"

int main(int argc, char* argv[]) {
    printf("hello world");
    exit(0);
}


// $ strace ./hello
// execve("./hello", ["./hello"], 0x7ffc275242a0 /* 48 vars */) = 0
// brk(NULL)                               = 0x555cef175000
// arch_prctl(0x3001 /* ARCH_??? */, 0x7ffffa743910) = -1 EINVAL (Invalid argument)
// access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
// openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
// fstat(3, {st_mode=S_IFREG|0644, st_size=67410, ...}) = 0
// mmap(NULL, 67410, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fac32999000
// close(3)                                = 0
// openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
// read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\300A\2\0\0\0\0\0"..., 832) = 832
// pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
// pread64(3, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
// pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\30x\346\264ur\f|Q\226\236i\253-'o"..., 68, 880) = 68
// fstat(3, {st_mode=S_IFREG|0755, st_size=2029592, ...}) = 0
// mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fac32997000
// pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
// pread64(3, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
// pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\30x\346\264ur\f|Q\226\236i\253-'o"..., 68, 880) = 68
// mmap(NULL, 2037344, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fac327a5000
// mmap(0x7fac327c7000, 1540096, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x22000) = 0x7fac327c7000
// mmap(0x7fac3293f000, 319488, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x19a000) = 0x7fac3293f000
// mmap(0x7fac3298d000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1e7000) = 0x7fac3298d000
// mmap(0x7fac32993000, 13920, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fac32993000
// close(3)                                = 0
// arch_prctl(ARCH_SET_FS, 0x7fac32998540) = 0
// mprotect(0x7fac3298d000, 16384, PROT_READ) = 0
// mprotect(0x555cee60e000, 4096, PROT_READ) = 0
// mprotect(0x7fac329d7000, 4096, PROT_READ) = 0
// munmap(0x7fac32999000, 67410)           = 0
// fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0x5), ...}) = 0
// brk(NULL)                               = 0x555cef175000
// brk(0x555cef196000)                     = 0x555cef196000
// write(1, "hello world", 11hello world)             = 11
// exit_group(0)                           = ?
// +++ exited with 0 +++
