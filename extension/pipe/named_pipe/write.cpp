/*
C语言示例代码-命名管道
创建两个无关联的进程，一个进程创建命名管道并写数据，另一个进程通过管道读数据。
*/
// write.c
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int ret;
  char buf[32] = {0};
  int fd;

  if (argc < 2) {
    printf("Usage:%s <fifo name> \n", argv[0]);
    return -1;
  }

  // access函数：检查调用进程是否可以对指定的文件执行某种操作，成功执行时，返回0。失败返回-1
  // F_OK：测试文件是否存在
  if (access(argv[1], F_OK) == -1) {
    ret = mkfifo(
        argv[1],
        0666);  // 创建命名管道文件，参数0666是文件的权限，表示任何有权限的用户
    if (ret == -1) {
      printf("mkfifo is error \n");
      return -2;
    }
    printf("mkfifo is ok \n");
  }

  fd = open(argv[1], O_WRONLY);  // 打开命名管道，O_WRONLY表示只写打开
  if (fd == -1) {
    perror("open fifo for write failed");
    return -3;
  }

  while (1) {
    sleep(1);
    ssize_t write_len = write(fd, "hello", 5);
    if (write_len == -1) {
      perror("write to fifo failed");
      break;
    } else if (write_len != 5) {
      printf("incomplete write, only %zd bytes written\n", write_len);
      break;
    }
  }

  close(fd);
  return 0;
}