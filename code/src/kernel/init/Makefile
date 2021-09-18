


SUBDIRS := 

#指定源文件为当前所有的C
SRCS = $(wildcard *.c)
OBJS += $(SRCS:%.c=%.o)


all: system $(OBJS) $(SUBDIRS)
	

#执行每级目录下的Makefile
$(SUBDIRS):ECHO
	$(MAKE) -C $@	

ECHO:
	@echo "Compiling " $(SUBDIRS) "..."	

$(OBJS):%.o:%.c 
	$(CC) -c $< -o $(BUILD)/$@ $(CFLAGS) $(DEPENDS)   -D$(DFLAGS)

system:
	$(CC) -E  head.S > head.s $(DEPENDS)  
	as --64 -o $(BUILD)/head.o head.s
	$(CC) -E  entry.S > entry.s $(DEPENDS) 
	as --64 -o $(BUILD)/entry.o entry.s
	$(CC) -E APU_boot.S > APU_boot.s $(DEPENDS) 
	as $--64 -o  $(BUILD)/APU_boot.o APU_boot.s
clean:
	@for dir in $(SUBDIRS);\
	do $(MAKE) -C $$dir clean||exit 1;\
	done
	$(RM)	 *.o *.s~ *.s *.S~ *.c~ *.h~ system  Makefile~ Kernel.lds~ kernel.bin 