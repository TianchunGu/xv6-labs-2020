#include <stdlib.h>    // 为了 exit()
#include <sys/wait.h>  // 为了 wait()
#include <unistd.h>    // 为了 fork(), write()
#include <iostream>

int main() {
  // fork() 返回 0 代表当前在子进程中
  // 返回大于 0 的值（子进程的 PID）代表当前在父进程中
  if (fork() == 0) {
    // --- 子进程的代码 ---
    // 向标准输出（文件描述符 1）写入 "hello "
    write(1, "hello ", 6);
    // 子进程正常退出
    exit(0);
  } else {
    // --- 父进程的代码 ---
    // 等待，直到任意一个子进程结束
    wait(0);
    // 子进程结束后，再向标准输出写入 " world\n"
    // 注意: " world\n" 包含 7 个字符
    write(1, " world\n", 7);
  }

  return 0;
}