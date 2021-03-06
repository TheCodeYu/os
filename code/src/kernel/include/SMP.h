#ifndef __SMP_H__

#define __SMP_H__

#include "spinlock.h"

extern unsigned char _APU_boot_start[];
extern unsigned char _APU_boot_end[];

extern spinlock_T SMP_lock;

extern int global_i;

void SMP_init();

void Start_SMP();

#endif
