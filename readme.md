1.bootloader

  bootloader = boot + loader;bios自检结束后根据启动选项选择启动设备，检测0磁头0磁道1扇区，以0xaa55结尾即认为是boot引导扇区(512B)
  bios将boot加载到0x7c00并跳转到此处执行,作为一级助推器加载loader到内存
  此时在实模式下：物理地址=CS<<4+IP(跳转时CS=0，IP=0x7c00)
  boot中加入文件系统(FAT12)，方便搜索文件并加载而不是将loader等文件写到固定扇区位置