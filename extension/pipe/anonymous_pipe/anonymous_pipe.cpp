// 匿名管道：C语言示例代码1,在一个进程本身使用匿名管道
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // unix标准头文件，包含pipe、write、read等函数声明

#define STRING "hello world!"
int main(void) {
  int pipefd[2];  // 文件描述符数组，pipefd[0]为读端，pipefd[1]为写端
  char buf[BUFSIZ];

  if (pipe(pipefd) == -1) {  // 建立匿名管道
    perror("pipe() error");
    exit(1);
  }

  if (write(pipefd[1], STRING, strlen(STRING)) < 0) {  // 向管道写数据
    perror("write() error");
    exit(1);
  }
  if (read(pipefd[0], buf, BUFSIZ) < 0) {  // 从管道读数据
    perror("read() error");
    exit(1);
  }

  printf("%s\n", buf);
  return 0;
}