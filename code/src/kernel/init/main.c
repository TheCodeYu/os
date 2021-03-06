

#include "lib.h"
#include "printk.h"
#include "gate.h"
#include "trap.h"
#include "memory.h"
#include "linkage.h"
#include "interrupt.h"
#include "task.h"
#include "cpu.h"
#if  APIC
#include "APIC.h"
#else
#include "8259A.h"
#endif
#include "keyboard.h"
#include "mouse.h"
#include "disk.h"
#include "SMP.h"
#include "spinlock.h"
#include "HPET.h"

#include "atomic.h"
#include "semaphore.h"
//使用模式号143 VM上1280*960 b上800*600
	//虚拟物理是反的，但是这里只需要根据自己的字符设计的XY填充即可

void Start_Kernel(void)
{
	struct INT_CMD_REG icr_entry;
	unsigned int * tss = NULL;
	
	Pos.XResolution = 1280;
	Pos.YResolution = 960;

	Pos.XPosition = 0;
	Pos.YPosition = 0;

	Pos.XCharSize = 8;
	Pos.YCharSize = 16;

	Pos.FB_addr = (int *)0xffff800003000000;
	Pos.FB_length = (Pos.XResolution * Pos.YResolution * 4 + PAGE_4K_SIZE - 1) & PAGE_4K_MASK;

	spin_init(&Pos.printk_lock);

	load_TR(10);

	set_tss64(TSS64_Table,_stack_start, _stack_start, _stack_start, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00);

	sys_vector_init();

	init_cpu();
	
	memory_management_struct.start_code = (unsigned long)& _text;
	memory_management_struct.end_code   = (unsigned long)& _etext;
	memory_management_struct.end_data   = (unsigned long)& _edata;
	memory_management_struct.end_brk    = (unsigned long)& _end;

	color_printk(RED,BLACK,"memory init \n");
	init_memory();

	color_printk(RED,BLACK,"slab init \n");
	slab_init();

	color_printk(RED,BLACK,"frame buffer init \n");
	frame_buffer_init();
	color_printk(WHITE,BLACK,"frame_buffer_init() is OK \n");

	color_printk(RED,BLACK,"pagetable init \n");	
	pagetable_init();
		
	color_printk(RED,BLACK,"interrupt init \n");

	#if  APIC
		APIC_IOAPIC_init();
	#else
		init_8259A();
	#endif

	color_printk(RED,BLACK,"schedule init \n");
	schedule_init();

	color_printk(RED,BLACK,"Soft IRQ init \n");
	softirq_init();

	color_printk(RED,BLACK,"keyboard init \n");
	keyboard_init();

	color_printk(RED,BLACK,"mouse init \n");
	mouse_init();
	
	color_printk(RED,BLACK,"ICR init \n");	

	SMP_init();

	//prepare send INIT IPI
	
	icr_entry.vector = 0x00;
	icr_entry.deliver_mode =  APIC_ICR_IOAPIC_INIT;
	icr_entry.dest_mode = ICR_IOAPIC_DELV_PHYSICAL;
	icr_entry.deliver_status = APIC_ICR_IOAPIC_Idle;
	icr_entry.res_1 = 0;
	icr_entry.level = ICR_LEVEL_DE_ASSERT;
	icr_entry.trigger = APIC_ICR_IOAPIC_Edge;
	icr_entry.res_2 = 0;
	icr_entry.dest_shorthand = ICR_ALL_EXCLUDE_Self;
	icr_entry.res_3 = 0;
	icr_entry.destination.x2apic_destination = 0x00;
	
	wrmsr(0x830,*(unsigned long *)&icr_entry);	//INIT IPI

	//prepare send Start-up IPI

	for(global_i = 1;global_i < 4;global_i++)
	{
		spin_lock(&SMP_lock);

		_stack_start = (unsigned long)kmalloc(STACK_SIZE,0) + STACK_SIZE;
		tss = (unsigned int *)kmalloc(128,0);
		set_tss_descriptor(10 + global_i * 2,tss);
		set_tss64(tss,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start,_stack_start);
	
		icr_entry.vector = 0x20;
		icr_entry.deliver_mode = ICR_Start_up;
		icr_entry.dest_shorthand = ICR_No_Shorthand;
		icr_entry.destination.x2apic_destination = global_i;
	
		wrmsr(0x830,*(unsigned long *)&icr_entry);	//Start-up IPI
		wrmsr(0x830,*(unsigned long *)&icr_entry);	//Start-up IPI
	}

	icr_entry.vector = 0xc8;
	icr_entry.destination.x2apic_destination = 1;
	icr_entry.deliver_mode = APIC_ICR_IOAPIC_Fixed;

	wrmsr(0x830,*(unsigned long *)&icr_entry);

	icr_entry.vector = 0xc9;
	wrmsr(0x830,*(unsigned long *)&icr_entry);

	color_printk(RED,BLACK,"Timer init \n");
	timer_init();

	color_printk(RED,BLACK,"HPET init \n");
	HPET_init();

	color_printk(RED,BLACK,"\n\n\n\ntask init \n");
	sti();
	task_init();

	while(1)
	{
		if(p_kb->count)
			analysis_keycode();
		if(p_mouse->count)
			analysis_mousecode();
		//hlt();
	}
}
