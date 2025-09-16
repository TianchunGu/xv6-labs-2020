#include <stdio.h>   // 为了 perror()，用于错误处理
#include <unistd.h>  // 为了 dup() 和 write()
#include <iostream>

int main() {
  int fd;

  // 复制文件描述符 1 (标准输出 stdout)
  // fd 将会是当前可用的最小的整数文件描述符 (通常是 3)
  fd = dup(1);
  if (fd < 0) {
    // 如果 dup 失败，它会返回 -1
    perror("dup failed");
    return 1;
  }

  // 1. 向原始的标准输出文件描述符写入
  write(1, "hello ", 6);

  // 2. 向复制出来的文件描述符写入
  // 因为 fd 和 1 指向同一个文件（屏幕），所以这部分内容会紧接着上一句输出
  write(fd, "world\n", 6);

  // 关闭复制出来的文件描述符是一个好习惯
  close(fd);

  return 0;
}