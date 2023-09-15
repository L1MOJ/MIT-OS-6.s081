#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

int main(int argc, char* argv[]) {
    int p[2];
    pipe(p);
    char buf[32] = {'\0'};

    if(fork() == 0) {
        read(p[0], buf, 4);
        close(p[0]);
        printf("%d: received %s\n",getpid(),buf);
        write(p[1], "pong", 4);
        close(p[1]);
        exit(0);
    }
    else {
        write(p[1], "ping", 4);
        close(p[1]);
        wait(0);
        read(p[0], buf, 4);
        close(p[0]);
        printf("%d: received %s\n", getpid(), buf);
        exit(0);
    }
    exit(0);
}