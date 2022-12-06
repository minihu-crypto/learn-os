#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  int p1[2], p2[2];
  char buf[] = {'A'};
  int n = sizeof(buf);
  pipe(p1); // 父写子读
  pipe(p2); // 子写父读

  if (fork() == 0) {
    // 子进程
    close(p1[1]); // 关闭p1写
    close(p2[0]); // 关闭p2读
    read(p1[0], buf, n);
    fprintf(0, "%d: received ping\n", getpid());
    write(p2[1], buf, n);
  } else {
    // 父进程
    close(p1[0]); // 关闭p1读
    close(p2[1]); // 关闭p2写
    write(p1[1], buf, 1);
    read(p2[0], buf, n);
    fprintf(0, "%d: received pong\n", getpid());
  }
  exit(0);
}