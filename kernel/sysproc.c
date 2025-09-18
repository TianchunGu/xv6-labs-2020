#include "date.h"
#include "defs.h"
#include "memlayout.h"
#include "param.h"
#include "proc.h"
#include "riscv.h"
#include "spinlock.h"
#include "types.h"

uint64 sys_exit(void) {
  int n;
  if (argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64 sys_getpid(void) {
  return myproc()->pid;
}

uint64 sys_fork(void) {
  return fork();
}

uint64 sys_wait(void) {
  uint64 p;
  if (argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64 sys_sbrk(void) {
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}
// sys_sleep 系统调用
uint64 sys_sleep(void) {
  int n;        // 用于存储需要休眠的时钟节拍数
  uint ticks0;  // 用于记录开始休眠时的时钟节拍
                // 1. 获取用户传入的参数
  if (argint(0, &n) < 0)
    return -1;  // 如果获取参数失败，返回-1表示错误
                // 2. 获取时钟锁，保护全局变量 ticks
  acquire(&tickslock);
  // 3. 记录当前的时钟节拍数
  ticks0 = ticks;
  // 4. 进入循环，直到休眠时间到达
  while (ticks - ticks0 < n) {
    // 4a. 检查进程是否被标记为“killed”
    if (myproc()->killed) {
      release(&tickslock);  // 如果是，释放锁并返回-1
      return -1;
    }
    // 4b. 调用 sleep 函数，进入休眠状态
    sleep(&ticks, &tickslock);
  }
  // 5. 休眠结束，释放时钟锁
  release(&tickslock);
  // 6. 返回0，表示成功
  return 0;
}

uint64 sys_kill(void) {
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64 sys_uptime(void) {
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
