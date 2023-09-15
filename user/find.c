#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

static char buf[512] = {'\0'};
static char *p = buf;

void
find(char *path, char* name) {
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

*p++ = '/';
while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
        continue;
    memmove(p, de.name, DIRSIZ);
    if(!strcmp(p, name)) {
        printf("%s\n", buf);
    }
    if(stat(buf, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", buf);
        exit(1);
    }

    if(strcmp(p, ".") && strcmp(p, "..") && st.type == T_DIR) {
        p += strlen(de.name);
        find(buf, name);
        p -= strlen(de.name);
    }

    p[DIRSIZ] = 0;

    }
    *p-- = '\0';
    return;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(2, "usage: find <path> <name>\n");
        exit(1);
    }
    char *path = argv[1];
    char *name = argv[2];

    *p++ = '.';
    find(path, name);
    exit(0);
}