#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("sleep <time required> \n");
        exit(1);
    }
    else {
        int s_time = atoi(argv[1]);
        sleep(s_time);
    }
    exit(0);
}