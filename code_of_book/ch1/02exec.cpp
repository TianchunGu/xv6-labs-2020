#include <stdio.h>   // 为了 printf()
#include <unistd.h>  // 为了 execv()
#include <iostream>

int main() {
  // 创建一个参数数组
  // argv[0] 是程序名本身
  // argv[1] 是第一个参数
  // 数组必须以空指针 (0 或 NULL) 结尾
  char* argv[3];
  argv[0] = (char*)"echo";   // 程序名
  argv[1] = (char*)"hello";  // 传递给 echo 的参数
  argv[2] = 0;               // 参数列表结束标记

  // 使用 execv 执行 /bin/echo 命令
  // execv 会用新的程序（/bin/echo）替换当前进程的内存空间
  execv("/bin/echo", argv);

  // 重要提示：
  // 如果 execv 调用成功，它永远不会返回，因为当前进程已经被新程序替换了。
  // 因此，下面的 printf 语句只有在 execv 调用失败时才会被执行。
  printf("exec error\n");

  return 1;  // 如果执行到这里，说明出错了，返回一个非零值
}