#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 循环创建subprocess,需要写成一个函数,递归调用
// 在prime内部fork,则父子进程共享局部变量
// 需要共享管道用于通信
void findPrime(int p[2]){
  int prime, num;
  if(read(p[0], &prime, 4)==0){
    close(p[0]);
    exit(0);
  }

  fprintf(0, "prime %d\n", prime);
  int p1[2];  // 和下一个子进程通信
  pipe(p1);
  if(fork()==0){
    // 子进程读
    close(p[0]);
    close(p1[1]);
    findPrime(p1);
  }else{
    close(p1[0]);// 从p读，写到p1
    while(read(p[0], &num, 4)){
      if(num % prime != 0){
        write(p1[1], &num, 4);
      }
    }
    close(p[0]);
    close(p1[1]);
  }
}

int main(){
  int p[2];
  pipe(p);
  if(fork()==0){
    close(p[1]);
    findPrime(p);
  }else{
    close(p[0]);
    for(int i = 2; i <= 35; ++i){
      write(p[1], &i, 4);
    }
    close(p[1]);
    wait(0);
  }
  exit(0);
}