#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

int main(int argc, char* argv[]){
    char buf[512];
    char *fullargv[MAXARG];
    if (argc < 2){
        fprintf(2,"Usage: xargs command\n");
        exit(1);
    }
    if (argc + 1 > MAXARG){
        fprintf(2, "Usage: too many args\n");
        exit(1);
    }
    for(int i = 1; i < argc; i++){
        fullargv[i-1] = argv[i];
    }
    fullargv[argc] = 0; // 一共有argc-1个参数

    // 还有部分通过管道输入的，在标准输入里
    while(1){ // 循环执行一行
        int i = 0;
        while(1){   // 读标准输入
            int len = read(0, &buf[i], 1);  // 一次读一个字符
            if(len <= 0 || buf[i]=='\n')break;
            i++;
        }
        if(i==0) break; // 没有标准输入
        buf[i] = 0; //分隔符
        fullargv[argc-1] = buf;
        if(fork()==0){
            exec(fullargv[0], fullargv);
            exit(0);
        }else{
            wait(0);
        }
    }
    exit(0);
}