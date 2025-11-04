# 定义编译器（C++ 使用 g++）
CXX = g++
# 编译选项：开启警告、优化
CXXFLAGS = -Wall -Wextra -O2

# 定义两个可执行文件目标
TARGET_READ = read
TARGET_WRITE = write

# 默认目标：编译所有程序
all: $(TARGET_READ) $(TARGET_WRITE)

# 编译 read 程序（依赖 read.cpp）
$(TARGET_READ): read.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# 编译 write 程序（依赖 write.cpp）
$(TARGET_WRITE): write.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# 清理编译生成的可执行文件
clean:
	rm -f $(TARGET_READ) $(TARGET_WRITE)

# 声明伪目标（避免与同名文件冲突）
.PHONY: all clean