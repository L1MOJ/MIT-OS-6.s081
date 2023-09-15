#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

void find_prime(int read_from) {
    int prime;
    if(!read(read_from, &prime, sizeof(int))) {
        close(read_from);
        return;
    }
    printf("prime %d\n", prime);

    int p[2];
    if(pipe(p) == -1) {
        fprintf(2, "Faild to create pipe.\n");
        exit(1);
    }

    if(fork() == 0) {
        close(p[1]);
        find_prime(p[0]);
        close(p[0]);
        exit(0);
    }
    else {
        int temp;
        close(p[0]);
        while(read(read_from, &temp, sizeof(int))) {
            if(temp % prime) {
                write(p[1], &temp, sizeof(int));
            }
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
    exit(0);
}

int main(int argc, char* argv[]) {
    int p[2];
    if(pipe(p) == -1) {
        fprintf(2, "Failed to create pipe.\n");
        exit(1);
    }

    if(fork() == 0) {
        close(p[1]);
        find_prime(p[0]);
        close(p[0]);
        exit(0);       
    }
    else {
        close(p[0]);

        for(int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
    exit(0);
}
