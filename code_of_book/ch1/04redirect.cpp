#include <fcntl.h>     // 为了 open() 和 O_RDONLY
#include <stdio.h>     // 为了 perror()
#include <sys/wait.h>  // 为了 wait()
#include <unistd.h>    // 为了 fork(), close(), execvp()
#include <iostream>

int main() {
  // 准备 execvp 的参数数组
  // argv[0] 是要执行的程序名
  // 数组必须以空指针 (0 或 NULL) 结尾
  char* argv[2];
  argv[0] = (char*)"cat";
  argv[1] = 0;

  // 创建一个子进程
  int pid = fork();

  if (pid < 0) {
    // fork 失败
    perror("fork failed");
    return 1;
  }

  if (pid == 0) {
    // --- 这里是子进程的代码 ---

    // 1. 关闭标准输入 (文件描述符 0)
    close(0);

    // 2. 打开 input.txt 文件。
    // open 会返回当前可用的最小文件描述符，因为我们刚关闭了 0，所以这里会返回
    // 0。 这就巧妙地将 input.txt 和标准输入关联了起来。
    if (open("input.txt", O_RDONLY) < 0) {
      perror("open failed");  // 如果文件打开失败，打印错误并退出
      return 1;
    }

    // 3. 执行 cat 命令。
    // cat 命令现在会从它的标准输入（也就是
    // input.txt）读取数据，并输出到它的标准输出（屏幕）。
    execvp("cat", argv);

    // 如果 execvp 执行成功，它永远不会返回。如果执行到这里，说明 execvp
    // 失败了。
    perror("execvp failed");
    return 1;

  } else {
    // --- 这里是父进程的代码 ---
    // 等待子进程执行结束
    wait(NULL);
    printf("Child process finished.\n");
  }

  return 0;
}

// 创建 input.txt 文件: 因为代码会尝试打开这个文件，所以你必须先创建它。
// echo "Hello from input.txt!" > input.txt
// echo "This is the second line." >> input.txt