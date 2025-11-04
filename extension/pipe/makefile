# 定义编译器（C++用g++）
CXX = g++
# 编译选项（开启警告和优化）
CXXFLAGS = -Wall -Wextra -O2

# 定义两个可执行目标，对应两个源文件
TARGET1 = pipe_demo                  # 对应 anonymous_pipe.cpp
TARGET2 = pipe_father_son            # 对应 anonymous_pipe_father_son.cpp

# 默认目标：编译所有可执行文件
all: $(TARGET1) $(TARGET2)

# 编译第一个目标
$(TARGET1): anonymous_pipe.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# 编译第二个目标
$(TARGET2): anonymous_pipe_father_son.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# 清理所有生成的可执行文件
clean:
	rm -f $(TARGET1) $(TARGET2)

# 伪目标声明
.PHONY: all clean