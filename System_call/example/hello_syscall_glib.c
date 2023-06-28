#define _GNU_SOURCE
#include <sys/syscall.h>
// implemented in libc.so
long syscall(long number, ...);
void main() {
syscall(SYS_write, 1, "SYSCALL: Hello World!\n", 22);
}
