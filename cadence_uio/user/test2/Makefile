ARCH ?= riscv
CROSS   = riscv64-linux-gnu-
CC      = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
ARCH    = $(CROSS)ar
TARGET = uio_user2 

#存放中间文件的路径
BUILD_DIR = build
#存放源文件的文件夹
SRC_DIR = ./
#存放头文件的文件夹
INC_DIR =  ../../RISCV-DPDK-riscv/include
 
#源文件
# wildcard函数用于获取文件列表，并使用空格分隔开。它的格式如下： $(wildcard 匹配规则)
#SRCS = $(wildcard $(SRC_DIR)/*.c)
SRCS = uio_user2.c
 
#目标文件（*.o）
# patsubst函数功能为模式字符串替换。它的格式如下： $(patsubst 匹配规则, 替换规则, 输入的字符串)
# notdir函数用于去除文件路径中的目录部分。它的格式如下：$(notdir 文件名)
# %是一个通配符，功能类似”*”，如”%.o”表示所 有以”.o”结尾的文件。
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))
 
 
#OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o) $(ASMS:%.S=$(BUILD_DIR)/%.o)
 
#头文件
DEPS = $(wildcard $(INC_DIR)/*.h)
 
#指定头文件的路径
CFLAGS = $(patsubst %, -I%, $(INC_DIR))
 
 
#目标文件
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
 
#*.o文件的生成规则
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
 
#创建一个编译目录，用于存放过程文件
#命令前带“@”,表示不在终端上输出
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)
 
#伪目标
.PHONY: clean cleanall
#按架构删除
clean:
	rm -rf $(BUILD_DIR)
