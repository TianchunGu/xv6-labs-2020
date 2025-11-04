/*匿名管道：C语言示例代码2，在父子进程中使用匿名管道*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void read_pipe(int fd) {
  char message[100];
  read(fd, message, 100);  // 从管道读取数据到message中
  printf("read pipe message:%s", message);
}

void write_pipe(int fd) {
  char* message = "this is Tuesday!\n";
  write(fd, message, strlen(message) + 1);  // 将message写到管道
}

int main() {
  int fd[2];
  pid_t pid;  // 进程id
  int stat_val;

  if (pipe(fd)) {
    printf("create pipe failed!\n");
  }

  // 创建子进程，该子进程是调用进程的几乎完全副本
  // 如果调用fork()的进程是父进程，则返回子进程的进程ID（PID）。如果调用fork()的进程是子进程，则返回0。
  pid = fork();

  switch (pid) {
    case -1:
      printf("fork error!\n");
      break;

    case 0:
      close(fd[1]);
      read_pipe(fd[0]);
      break;

    default:
      close(fd[0]);
      write_pipe(fd[1]);

      wait(&stat_val);  // 等待子进程结束
      break;
  }

  return 0;
}