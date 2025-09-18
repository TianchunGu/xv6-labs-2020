#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
  // 需要两个管道：p2c (parent to child) 和 c2p (child to parent)
  int p2c[2];
  int c2p[2];

  pipe(p2c);
  pipe(c2p);

  int pid = fork();

  if (pid < 0) {
    fprintf(2, "fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    // --- 子进程 ---
    char buf[1];  // 用来接收和发送字节的缓冲区

    // 关闭不用的管道端口
    close(p2c[1]);  // 关闭父->子管道的写入端
    close(c2p[0]);  // 关闭子->父管道的读取端

    // 1. 从父进程读取字节
    if (read(p2c[0], buf, 1) != 1) {
      fprintf(2, "child: read failed\n");
      exit(1);
    }

    // 2. 打印 "ping" 消息
    printf("%d: received ping\n", getpid());

    // 3. 将字节写回给父进程
    if (write(c2p[1], buf, 1) != 1) {
      fprintf(2, "child: write failed\n");
      exit(1);
    }

    // 关闭所有用完的端口
    close(p2c[0]);
    close(c2p[1]);

    exit(0);

  } else {
    // --- 父进程 ---
    char buf[1];  // 用来发送和接收字节的缓冲区

    // 关闭不用的管道端口
    close(p2c[0]);  // 关闭父->子管道的读取端
    close(c2p[1]);  // 关闭子->父管道的写入端

    // 1. 向子进程写入一个字节
    if (write(p2c[1], "a", 1) != 1) {  // 写入任意一个字节即可
      fprintf(2, "parent: write failed\n");
      exit(1);
    }

    // 2. 从子进程读取返回的字节
    if (read(c2p[0], buf, 1) != 1) {
      fprintf(2, "parent: read failed\n");
      exit(1);
    }

    // 3. 打印 "pong" 消息
    printf("%d: received pong\n", getpid());

    // 关闭所有用完的端口
    close(p2c[1]);
    close(c2p[0]);

    exit(0);
  }
}