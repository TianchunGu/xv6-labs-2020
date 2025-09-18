#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
  // 1. 检查命令行参数数量是否正确
  if (argc != 2) {
    // 2. 如果不正确，向标准错误输出提示信息
    fprintf(2, "usage: sleep <ticks>\n");
    exit(1);  // 3. 退出并返回错误状态码
  }

  // 4. 使用 atoi 将字符串参数转换为整数
  int ticks = atoi(argv[1]);

  // 5. 调用 sleep 系统调用，暂停指定的节拍数
  sleep(ticks);

  // 6. 程序正常结束，退出并返回成功状态码
  exit(0);
}