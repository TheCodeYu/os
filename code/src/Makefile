TOOLPATH = ../tools/

export NASM =D:\zhouyu\vm\code\nasm\nasm.exe #使用全路径 汇编编译器

export del = del	#删除文件
#window下del type 删除创建文件	md rd(非空)文件夹


all: compiler


compiler:
	$(MAKE) -C bootloader
	$(MAKE) -C kernel

clean:
	$(MAKE) -C bootloader		clean
	$(MAKE) -C kernel		clean

