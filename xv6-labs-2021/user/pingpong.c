#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
  // `pipe`创建管道； 
  // `fork` 创建进程；
  // `read/write` 读写管道；
  // `getpid`得到自己的进程id
  int p1[2], p2[2];
  pipe(p1); // 父写子读
  pipe(p2); // 父读子写
  // 创建存储数据的空间
  char buf[] = {'A'};
  int n = sizeof(buf);
  if(fork()==0){  // 子
    close(p1[1]);
    close(p2[0]);
    // 读
    read(p1[0], buf, n);
    close(p1[0]);
    fprintf(0, "%d: received ping\n", getpid());
    // 写
    write(p2[1], buf, n);
    close(p2[1]);
  }else{  // 父
    close(p1[0]);
    close(p2[1]);
    // 写
    write(p1[1], buf, n);
    close(p1[1]);
    // 读
    read(p2[0], buf, n);
    close(p2[0]);
    fprintf(0, "%d: received pong\n", getpid());
  }
  exit(0);
}

// int main() {
//   int p1[2], p2[2];
//   char buf[] = {'A'};
//   int n = sizeof(buf);
//   pipe(p1); // 父写子读
//   pipe(p2); // 子写父读

//   if (fork() == 0) {
//     // 子进程
//     close(p1[1]); // 关闭p1写
//     close(p2[0]); // 关闭p2读
//     read(p1[0], buf, n);
//     fprintf(0, "%d: received ping\n", getpid());
//     write(p2[1], buf, n);
//   } else {
//     // 父进程
//     close(p1[0]); // 关闭p1读
//     close(p2[1]); // 关闭p2写
//     write(p1[1], buf, 1);
//     read(p2[0], buf, n);
//     fprintf(0, "%d: received pong\n", getpid());
//   }
//   exit(0);
// }