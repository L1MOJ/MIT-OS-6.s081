#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

#define MAXARG 32
#define MSGSIZE 16
int main(int argc, char* argv[]) {
    sleep(10);
    int xargc = 0;
    char* xargv[MAXARG];
    //Read standard input
    char buf[MSGSIZE];
    read(0, buf, MSGSIZE);

    for(int i = 1; i < argc; i++) { 
        xargv[xargc] = argv[i];
        xargc++; 
    }
    char* p = buf;

    for (int i = 0; i < MSGSIZE; i++) {
        if(buf[i] == '\n') {
            //Child
            if(fork() == 0) {
                buf[i] = 0;
                xargv[xargc] = p;
                xargc++;
                xargv[xargc] = 0;
                xargc++;

                exec(xargv[0], xargv);
                  
            }
            else {
                p = &buf[i+1];
                wait(0);
            }
        }
    }
    wait(0);
    exit(0);
}
