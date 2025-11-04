> [视频链接](https://www.bilibili.com/video/BV1BT421C7rZ/)
>

## 什么是管道？
**进程间的通信方式**有五种，分别为：<font style="color:#DF2A3F;background-color:#FBF5CB;">管道</font>、信号量、共享内存、消息队列和套接字，

<font style="color:#DF2A3F;background-color:#FBF5CB;">管道</font>：本质上就是一个文件，前面的进程以<font style="color:#DF2A3F;">写方式</font>打开文件，后面的进程以<font style="color:#DF2A3F;">读方式</font>打开。这样前面写完后面读，于是就实现了通信。即把一个进程的输出直接连接在另外一个进程的输入。

虽然实现形态上是一个特殊文件，对管道的读写可以使用普通的`read()`，`write()`函数，但管道<font style="color:#DF2A3F;">不属于任何文件系统</font>（管道本身并不占用磁盘或者其他外部存储的空间）。

在Linux的实现上，它占用的是<font style="color:#DF2A3F;">内存空间，只存在于内存中</font>。所以，Linux上的管道就是一个操作方式为文件的**内存缓冲区**。即<font style="color:#DF2A3F;background-color:#FBF5CB;">管道本质上是内核的一块缓存</font>。

管道分为有名管道和无名管道两种,它们的区别是

+ **无名管道**：又称为**匿名管道**（Anonymous Pipe），只能在<font style="color:#DF2A3F;">亲缘进程</font>（<font style="color:#DF2A3F;">是指有同一个祖先</font>如父子进程、兄弟进程）之间进行通信。
+ **有名管道**：又称为**命名管道**（Named Pipe），可以在<font style="color:#DF2A3F;">任意</font>两个进程之间进行通信。

## 管道通信模式
通信模式分为：

+ **单工**：数据只在一个方向上传输，不能实现双方通信
+ **半双工**（切换的单工）：充许数据在两个方向上传输，但是<font style="color:#DF2A3F;">同一时间</font>数据只能在同一个方向上传输
+ **全双工**：充许数据在两个方向上同时传输

<font style="color:#DF2A3F;">管道是半双工的</font>，数据只能向一个方向流动；需要双方通信时，需要建立起两个管道。

### 管道通信补充
1. 读阻塞（进程阻塞）：当管道中没有数据可读时，管道为空，会产生读阻塞。
2. 写阻塞：当管道已满，再往管道中写入数据时，会产生写阻塞。直到有空间可以写入时，再写。
3. 如果所有指向管道写端的文件描述符都关闭了，而仍然有进程从管道的读端读数据,那么管道中剩余的数据都被读取后，再次`read`会返回0，就像读到文件未尾一样。
4. 如果所有指向管道读端的文件描述符都关闭了，这时有进程向管道的写端`write`，那么该进程会收到信号SIGPIPE，通常会导致进程异常终止。 
5. 管道破裂：只有写端，没有读端。
6. 管道中不能使用`lseek()`等文件的定位操作。

## 管道的特点
1. 管道是半双工的。
2. 管道提供流式服务，<font style="color:#DF2A3F;">字节流通信，没有消息边界</font>，多个进程同时发送的字节流混在一起，则无法分辨消息，所有管道一般用于两个进程之间通信。
3. 管道的内容读完后不会保存。
4. 一般而言，进程退出，管道释放，所以管道的生命周期跟随进程。
5. 一般而言，内核会对管道操作进行同步与互斥。
6. 匿名管道一般用于亲缘进程间通信，命名管道一般用于两个无关联进程间通信。

## 匿名管道（PIPE）
是一种<font style="color:#DF2A3F;">亲缘进程</font>间的通信方法。匿名管道存在于kernel（内核）中，A，B必须具有亲缘关系进程。同一时刻，只能有一个写端或一个读端。

父子进程间，只要是`fork()`出来的，就会完美复制父进程的数据。如果在`fork()`之前创建管道，开获取管道的操作接口，子进程就能使用管道。

### 如何操作匿名管道
#### 创建
> pipe函数用来创建匿名管道
>

```cpp
// 函数原型
int pipe(int fd[2]);
```

参数fd：文件描述符数组，用于存放管道的文件描述符。其中<font style="color:#DF2A3F;">fd[0]表示读端，fd[1]表示写端</font>

返回值：成功返回0，失败返回错误代码-1

#### 操作
read读

write写

#### 关闭
close

### 匿名管道：C语言示例代码1
> 在<font style="color:#DF2A3F;">一个进程本身</font>使用匿名管道
>

```cpp
// 匿名管道：C语言示例代码1,在一个进程本身使用匿名管道
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>  // unix标准头文件，包含pipe、write、read等函数声明
#include <string.h>

#define STRING "hello world!"
int main(void)
{
    int pipefd[2];//文件描述符数组，pipefd[0]为读端，pipefd[1]为写端
    char buf[BUFSIZ];

    if(pipe(pipefd) == -1){    // 建立匿名管道
        perror("pipe() error");
        exit(1);
    }

    if(write(pipefd[1], STRING, strlen(STRING)) < 0) { // 向管道写数据
        perror("write() error");
        exit(1);
    }
    if(read(pipefd[0], buf, BUFSIZ) < 0) { // 从管道读数据
        perror("read() error");
        exit(1);
    }

    printf("%s\n", buf);
    return 0;
}
```

```cpp
d98ae94f5287# ls
anonymous_pipe.cpp  makefile
d98ae94f5287# make
gcc -Wall -Wextra -O2   -o pipe_demo anonymous_pipe.cpp
d98ae94f5287# ./pipe_demo 
hello world!
```

这个程序创建了一个管道，并且对管道写了一个字符串之后从管道读取，并打印在标准输出上。用一个图来说明这个程序的状态就是这样的。

![](https://cdn.nlark.com/yuque/0/2025/png/33636091/1762248300613-62a8a40a-8707-46db-8a90-77af051bc39e.png)

 

### 匿名管道：C语言示例代码2
```cpp
/*匿名管道：C语言示例代码2，在父子进程中使用匿名管道*/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

void read_pipe(int fd)
{
    char message[100];
    read(fd, message, 100); //从管道读取数据到message中
    printf("read pipe message:%s", message);
}

void write_pipe(int fd)
{
    char * message = "this is Tuesday!\n";
    write(fd, message, strlen(message) + 1); //将message写到管道
}

int main()
{
    int fd[2];
    pid_t pid;      //进程id
    int stat_val;

    if(pipe(fd))
    {
        printf("create pipe failed!\n");
    }

    //创建子进程，该子进程是调用进程的几乎完全副本
    //如果调用fork()的进程是父进程，则返回子进程的进程ID（PID）。如果调用fork()的进程是子进程，则返回0。
    pid = fork();

    switch(pid)
    {
        case -1:
            printf("fork error!\n");
            break;

        case 0:
            close(fd[1]);
            read_pipe(fd[0]);
            break;

        default:
            close(fd[0]);
            write_pipe(fd[1]);

            wait(&stat_val);  //等待子进程结束
            break;
    }

    return 0;
}
```

```cpp
d98ae94f5287# ./pipe_father_son
read pipe message:this is Tuesday!
```

如图中描述，fork产生的<font style="color:#DF2A3F;">子进程会继承父进程对应的文件描述符</font>。利用这个特性，父进程先pipe创建管道之后，子进程也会得到同一个管道的读写文件描述符。从而实现了父子两个进程使用一个管道可以完成半双工通信。

![](https://cdn.nlark.com/yuque/0/2025/png/33636091/1762248983317-af89b34c-8654-41f3-b9e1-a6994b1c8062.png)

![](https://cdn.nlark.com/yuque/0/2025/png/33636091/1762249552371-1796a49b-00da-4d1e-af8f-d04ec8d43a5d.png)

## 命名管道
**命名管道（Named Pipe）**是一种独立进程之间通信的机制，用于在无关的进程之间进行数据传输。

与匿名管道不同，命名管道不需要亲缘关系的进程之间，也不需要共享同一终端。任意进程可以通过打开命名管道的读取端和写入端来与其进行通信。

命名管道通过在文件系统中创建一个特殊的文件来实现通信。这个特殊的文件被称为**FIFO**（First-in First-out）或命名管道。

### 命名管道通信原理
和匿名管道一样，想让双方通信，必须先让双方看到**同一份资源**！它和匿名管道本质是一样的，只是看到资源的方式不同。

匿名管道是通过父子进程继承来看到**同一份资源**的，也叫做管道文件，这个文件是**纯内存级**的，所以没有名字，叫做匿名管道。

而命名管道是在磁盘上有一个特殊的文件，这个文件可以被打开，但是打开后<font style="color:#DF2A3F;">不会将内存中的数据刷新到磁盘</font>。在磁盘上就有了路径，而路径是唯一的，所以双方就可以通过文件的路径来看到同一份资源即管道文件。

### 命名管道的特点
1. 命名管道可以使互不相关的两个独立进程实现彼此通信。
2. 命名管道可以通过路径名来指出，并且在文件系统中是可见的。在建立管道之后，两个进程就可以把它当作普通文件进行读写，使用非常方便。
3. FIFO严格遵循先进先出原则，对管道及FIFO的读总是从开始处返回数据，对它们的写则把数据添加到未尾。命名管道不支持如`lseek()`等文件的定位操作。 
4. 命名管道依然在内核态内存中。
5. 命名管道在文件系统中有节点（即在文件系统中可以找到）。
6. 命名管道严格遵循先进先出原则
7. 命名管道不能使用文件重定位的函数`lseek()`
8. 命名管道可以用在亲缘和非亲缘进程间（一般用于非亲缘进程间通信）。

### 如何操作命名管道
#### 创建命名管道文件
mkfifo即是命令也是函数

mknod也可以创建管道文件

#### 打开命名管道
open

#### 读/写
read/write

#### 关闭
close

### 示例代码
#### write
```cpp
/*
C语言示例代码-命名管道
创建两个无关联的进程，一个进程创建命名管道并写数据，另一个进程通过管道读数据。
*/
//write.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char * argv[])
{
    int ret;
    char buf[32] = {0};
    int fd;

    if (argc < 2)
    {
        printf("Usage:%s <fifo name> \n", argv[0]);
        return -1;
    }

    //access函数：检查调用进程是否可以对指定的文件执行某种操作，成功执行时，返回0。失败返回-1
    //F_OK：测试文件是否存在
    if (access(argv[1], F_OK) == -1)
    {
        ret = mkfifo(argv[1], 0666); //创建命名管道文件，参数0666是文件的权限，表示任何有权限的用户
        if (ret == -1)
        {
            printf("mkfifo is error \n");
            return -2;
        }
        printf("mkfifo is ok \n");
    }

    fd = open(argv[1], O_WRONLY);   //打开命名管道，O_WRONLY表示只写打开
    if (fd == -1) {
        perror("open fifo for write failed");
        return -3;
    }

    while (1)
    {
        sleep(1);
        ssize_t write_len = write(fd, "hello", 5);
        if (write_len == -1) {
            perror("write to fifo failed");
            break;
        } else if (write_len != 5) {
            printf("incomplete write, only %zd bytes written\n", write_len);
            break;
        }
    }

    close(fd);
    return 0;
}
```

#### read
```cpp
//read.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char * argv[])
{
    char buf[32] = {0};
    int fd;

    if (argc < 2)
    {
        printf("Usage:%s <fifo name> \n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDONLY); //打开命名管道，O_RDONLY表示只读打开
    if (fd == -1) {
        perror("open fifo for read failed");
        return -2;
    }

    while (1)
    {
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
        memset(buf, 0, sizeof(buf)); //设置内存，将buf初始化为0
    }

    close(fd);      //关闭管道
    return 0;
}
```

## 管道命令
在Linux shell中执行命令，经常会将上一个命令的输出作为下一个命令的输入，由多个命令配合完成一件事情。而这就是通过管道来实现的。<font style="color:#DF2A3F;">| 这个竖线就是管道符号</font>。

```cpp
ls-l | grep string//grep是抓取指令
```

+ Is命令（其实也是一个进程）会把当前目录中的文件都列出来但它不会直接输出，而是把要输出到屏幕上的数据通过管道输出到grep这个进程中，作为grep这个进程的输入；
+ 然后这个进程对输入的信息进行筛选（grep的作用），把存在string的信息的字符串（以行为单位）打印在屏幕上。

![](https://cdn.nlark.com/yuque/0/2025/png/33636091/1762251565585-9d7e7d63-4a14-47d0-8844-3f4c9c98dad9.png)



