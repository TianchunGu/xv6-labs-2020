#include <stdio.h>     // 为了 perror()
#include <sys/wait.h>  // 为了 wait()
#include <unistd.h>    // 为了 pipe, fork, close, dup, write, execv
#include <iostream>

int main() {
  int p[2];
  char* argv[2];
  argv[0] = (char*)"wc";
  argv[1] = 0;

  // 1. 创建管道
  if (pipe(p) < 0) {
    perror("pipe failed");
    return 1;
  }

  // 2. 创建子进程
  int pid = fork();
  if (pid < 0) {
    perror("fork failed");
    return 1;
  }

  if (pid == 0) {
    // --- 子进程的代码 ---
    // (目标: 执行 wc，使其从管道读取数据)

    // 关闭标准输入 (文件描述符 0)
    close(0);
    // 复制管道的读取端(p[0])到标准输入(0)。
    // 现在，任何从标准输入读取的操作都会从管道中读取。
    dup(p[0]);

    // 关闭原始的管道文件描述符，因为它们已经被复制或不再需要
    close(p[0]);
    close(p[1]);  // 子进程不需要向管道写入

    // 执行 wc 命令
    execv("/bin/wc", argv);

    // 如果 execv 成功，代码不会执行到这里
    perror("execv failed");
    return 1;

  } else {
    // --- 父进程的代码 ---
    // (目标: 向管道写入数据 "hello world\n")

    // 父进程不需要从管道读取，所以关闭读取端
    close(p[0]);
    // 向管道的写入端写入数据
    write(p[1], "hello world\n", 12);
    // 数据写完后，关闭写入端。这会向管道的读取端发送一个 EOF (文件结束)信号。
    close(p[1]);

    // 等待子进程执行完毕
    wait(NULL);
  }

  return 0;
}