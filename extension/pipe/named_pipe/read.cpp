// read.c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  char buf[32] = {0};
  int fd;

  if (argc < 2) {
    printf("Usage:%s <fifo name> \n", argv[0]);
    return -1;
  }

  fd = open(argv[1], O_RDONLY);  // 打开命名管道，O_RDONLY表示只读打开
  if (fd == -1) {
    perror("open fifo for read failed");
    return -2;
  }

  while (1) {
    sleep(1);
    ssize_t read_len = read(fd, buf, sizeof(buf) - 1);
    if (read_len == -1) {
      perror("read from fifo failed");
      break;
    } else if (read_len == 0) {
      printf("fifo write end closed\n");
      break;
    }
    buf[read_len] = '\0';
    printf("buf is %s\n", buf);
    memset(buf, 0, sizeof(buf));  // 设置内存，将buf初始化为0
  }

  close(fd);  // 关闭管道
  return 0;
}