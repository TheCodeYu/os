

SUBDIRS := 

#指定源文件为当前所有的C
SRCS = $(wildcard *.c)
OBJS += $(SRCS:%.c=%.o)


all: $(OBJS) $(SUBDIRS)
	

#执行每级目录下的Makefile
$(SUBDIRS):ECHO
	$(MAKE) -C $@	

ECHO:
	@echo "Compiling " $(SUBDIRS) "..."	

$(OBJS):%.o:%.c 
	$(CC) -c $< -o $(BUILD)/$@ $(CFLAGS) $(DEPENDS)


clean:
	@for dir in $(SUBDIRS);\
	do $(MAKE) -C $$dir clean||exit 1;\
	done
	$(RM)	 *.o *.s~ *.s *.S~ *.c~ *.h~  Makefile~ 