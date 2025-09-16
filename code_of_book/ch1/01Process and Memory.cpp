#include <stdio.h>     // 为了 printf()
#include <stdlib.h>    // 为了 exit()
#include <sys/wait.h>  // 为了 wait()
#include <unistd.h>    // 为了 fork()
#include <iostream>

using namespace std;

int main() {
  // fork()在父进程中返回子进程的PID
  // 在子进程中返回0
  int pid;

  pid = fork();
  if (pid > 0) {
    printf("parent: child=%d\n", pid);
    pid = wait((int*)0);
    printf("child %d is done\n", pid);
  } else if (pid == 0) {
    printf("child: exiting\n");
    exit(0);
  } else {
    printf("fork error\n");
  }
  return 0;
}