#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(2,
            "error: you should pass 1 paramter, which means the sleep time!\n");
    exit(1);
  }
  int i = atoi(argv[1]);
  if (i <= 0)
    exit(1);
  sleep(i);
  exit(0);
}