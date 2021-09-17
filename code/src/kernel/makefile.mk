#一般是设定编译规则的
TARGET = kernel.bin

export CC = gcc
export MAKE = make
AR = ar cr
export RM = -rm -rf

export CFLAGS += -mcmodel=large -fno-builtin -fno-stack-protector -m64 -std=gnu89

# -Wno-trigraphs \
# 		   -fno-strict-aliasing -fno-common \
# 		   -Werror-implicit-function-declaration \
# 		   -Wno-format-security \
		   
#-Wall -Wundef -Wstrict-prototypes
LDFLAGS = Kernel.lds 
export TOPDIR = $(PWD)#声明顶级目录

EXCLUDE_DIR := build include

export DEPENDS:=	-I$(PWD)/include				#头文件搜索路径
export BUILD := $(PWD)/build
GET_SUBDIR1 := $(shell find . -maxdepth 1 -type d)
GET_SUBDIR2 := $(basename $(patsubst ./%,%,$(GET_SUBDIR1)))
GET_SUBDIR3 := $(filter-out $(EXCLUDE_DIR),$(GET_SUBDIR2))

SUBDIRS := $(GET_SUBDIR3)

#指定源文件为当前所有的C
SRCS = $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)
SRCS-y = $(wildcard $(BUILD)/*.o)

all:$(SUBDIRS) $(OBJS) $(TARGET)  
$(LIB):$(OBJS)
	$(AR)	$@ S^
	cp $@ $(LIBPATH)

#执行每级目录下的Makefile
# all: system
# 	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system kernel.bin

# system:	head.o entry.o main.o printk.o trap.o
# 	ld -b elf64-x86-64 -z muldefs -o system head.o entry.o main.o printk.o trap.o -T Kernel.lds 
$(SUBDIRS):ECHO
	$(MAKE) -C $@

ECHO:
	@echo "Compiling " $(SUBDIRS) "..."

$(TARGET): 
	$(MAKE) -C build
# ld -b elf64-x86-64 -z muldefs -o system $^  -T $(LDFLAGS) 
# objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system $@
	

$(OBJS):%.o:%.c 
	$(CC) -c $< -o $(BUILD)/$@ $(CFLAGS) -I$(DEPENDS)


clean:
	@for dir in $(SUBDIRS);\
	do $(MAKE) -C $$dir clean||exit 1;\
	done
	$(MAKE) -C build clean||exit 1
	$(RM) $(TARGET) $(LIB) $(OBJS) system *.o