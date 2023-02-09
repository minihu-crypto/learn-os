#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path){
  // static char buf[DIRSIZ+1];
  char *p;

  // 找到路径最后一部分
  for(p = path+strlen(path); p>= path && *p !='/'; p--){
    ;
  }
  p++;
  return p;

  // 不应该以''结尾，无法使用strlen  strcpy等函数
  // Return blank-padded name.
  // if(strlen(p) >= DIRSIZ)
  //   return p;
  // memmove(buf, p, strlen(p));
  // memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  // return buf;
}

void find(char *path, char *filename) {
  char buf[512], *p;  // buf
  int fd; // 
  struct dirent de; //directory structure
  struct stat st; // inode
  if((fd=open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
    case T_FILE:
      // printf("fmtname %s\n", fmtname(path));
      // printf("filename %s\n", filename);
      if(strcmp(fmtname(path), filename)==0){
        printf("%s\n", path);
        // printf("%s\n", fmtname(path));
      }
      break;

    case T_DIR:
      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        printf("ls: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf + strlen(buf);  // 实际长度和path相同
      *p++ = '/';
      // 每次读取目录下一个条目到de中
      while (read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum==0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)  // 
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        find(buf, filename);
      }
      break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(2, "please input a file name.");
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}