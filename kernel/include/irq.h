#ifndef __IRQ_H__
#define __IRQ_H__
#include<types.h>
/* 中断处理相关函数 */
void init_idt(void);
void init_intr(void);

void set_timer_intr_handler( void (*ptr)(void) );
void set_keyboard_intr_handler( void (*ptr)(int) );
  /* typedef struct TrapFrame {
	uint32_t edi, esi, ebp, old_esp, ebx, edx, ecx, eax;
	int32_t irq;
	uint32_t error_code;
	uint32_t eip, cs, eflags;
	uint32_t esp, ss;
	uint32_t gs, fs, es, ds;
 } TrapFrame;*/
#endif
