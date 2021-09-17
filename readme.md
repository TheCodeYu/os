### 1.bootloader

  bootloader = boot + loader;bios自检结束后根据启动选项选择启动设备，检测0磁头0磁道1扇区，以0xaa55结尾即认为是boot引导扇区(512B)
  bios将boot加载到0x7c00并跳转到此处执行,作为一级助推器加载loader到内存
  此时在实模式下：物理地址=CS<<4+IP(跳转时CS=0，IP=0x7c00)
  boot中加入文件系统(FAT12)，方便搜索文件并加载而不是将loader等文件写到固定扇区位置
  loader在内核执行前准备好所有数据，如硬件检测，模式切换(实模式->32位保护模式->64位IA-32e长模式)等

### 2.内核

  内核执行头程序由loader加载后转移控制权到内核时执行，需要用特殊的编译连接方法

undefined reference to `__stack_chk_fail' 编译选项增加-fno-stack-protector

inline内联函数找不到定义解决：1，inline前加static；2在所需的文件前使用extern引入

relocation truncated to fit: R_X86_64_PC32 against undefined symbol `__switch_to'：

### 3.制作不同大小的img

先用edimg工具把MBR写进去，再用WinImage根据MBR生成

4.物理平台：

选用不同的ModeInfo，使用loader查看各个支持的modeinfo，选择一个可以用的，head.S中配置页表地址