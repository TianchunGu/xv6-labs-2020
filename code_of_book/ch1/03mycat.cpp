#include <stdio.h>   // 为了 fprintf()
#include <stdlib.h>  // 为了 exit()
#include <unistd.h>  // 为了 read() 和 write()
#include <iostream>

int main() {
  char buf[512];
  int n;

  // 无限循环，直到 break 或 exit
  for (;;) {
    // 从标准输入(文件描述符 0)读取数据到 buf
    n = read(0, buf, sizeof buf);

    // 如果读取到文件结尾 (例如按下了 Ctrl+D)，n 会是 0
    if (n == 0) {
      break;
    }

    // 如果读取发生错误，n 会是 -1
    if (n < 0) {
      // 向标准错误(文件描述符 2)输出错误信息
      fprintf(stderr, "read error\n");
      exit(1);  // 退出程序，返回状态 1 表示有错误
    }

    // 将读取到的 n 个字节写入标准输出(文件描述符 1)
    // 并检查是否所有字节都成功写入
    if (write(1, buf, n) != n) {
      fprintf(stderr, "write error\n");
      exit(1);
    }
  }

  return 0;  // 正常退出
}