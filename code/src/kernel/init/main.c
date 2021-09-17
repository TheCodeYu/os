

#include "lib.h"
#include "printk.h"
#include "gate.h"
#include "trap.h"
#include "memory.h"
#include "linkage.h"
#include "interrupt.h"
#include "task.h"

/*
		static var 
*/

struct Global_Memory_Descriptor memory_management_struct = {{0},0};

void Start_Kernel(void)
{
	int *addr = (int *)0xffff800000a00000;
	int i;

	Pos.XResolution = 1280; //使用模式号143 VM上1280*960 b上800*600
	Pos.YResolution = 960;

	Pos.XPosition = 0;
	Pos.YPosition = 0;

	Pos.XCharSize = 8;			//虚拟物理是反的，但是这里只需要根据自己的字符设计的XY填充即可
	Pos.YCharSize = 16;

	Pos.FB_addr = (int *)0xffff800000a00000;
	Pos.FB_length = (Pos.XResolution * Pos.YResolution * 4 + PAGE_4K_SIZE - 1) & PAGE_4K_MASK;

	load_TR(10);

	set_tss64(_stack_start, _stack_start, _stack_start, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00);

	sys_vector_init();

	memory_management_struct.start_code = (unsigned long)& _text;
	memory_management_struct.end_code   = (unsigned long)& _etext;
	memory_management_struct.end_data   = (unsigned long)& _edata;
	memory_management_struct.end_brk    = (unsigned long)& _end;

	color_printk(RED,BLACK,"memory init \n");
	init_memory();

	color_printk(RED,BLACK,"interrupt init \n");
	init_interrupt();

	color_printk(RED,BLACK,"task_init \n");
	task_init();

	while(1)
		;
}


	// color_printk(WHITE,BLACK,"Yellow OS [version: 1.0.0.0]\n");
	// color_printk(WHITE,BLACK,"(C) www.4mychip.com All Rights Reserved\n");