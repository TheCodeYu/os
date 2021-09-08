name=bootloader

all: boot.bin loader.bin

loader.bin:
	$(NASM) loader.asm -o loader.bin

boot.bin:
	$(NASM) boot.asm -o boot.bin

clean:
	$(del) *.bin *.asm~ Makefile~ loader.bin boot.bin