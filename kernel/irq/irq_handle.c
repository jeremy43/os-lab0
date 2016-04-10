#include "x86/x86.h"
#include "game.h"
#define SELECTOR_KERNEL(s) ( ((s) << 3) | 0 )
static void (*do_timer)(void);
static void (*do_keyboard)(int);
uint32_t get_gdt_off(uint32_t);
void
set_timer_intr_handler( void (*ptr)(void) ) {
	do_timer = ptr;
}
void
set_keyboard_intr_handler( void (*ptr)(int) ) {
	do_keyboard = ptr;
}
void schedule();
void do_syscall(struct TrapFrame*);
int offset;
uint32_t number;
/* TrapFrame的定义在include/x86/memory.h
 * 请仔细理解这段程序的含义，这些内容将在后续的实验中被反复使用。 */
void
irq_handle(struct TrapFrame *tf) {
	int seg_tmp;
	asm volatile("movl %%es, %0" : "=a"(seg_tmp) :);
	asm volatile("movl %0, %%es\n\t"
			 "movl %0, %%ds\n\t"
			  "movl %0, %%fs\n\t"
			  "movl %0, %%gs\n\t"
			   :
			  : "a"(SELECTOR_KERNEL(SEG_KERNEL_DATA)));
	 offset = get_gdt_off(seg_tmp >> 3);
//	 printk("seg%d\n",seg_tmp>>3);
//	 printk("irq %d\n",tf->irq);
	if(tf->irq < 1000) {
		if(tf->irq == -1) {
			printk("%s, %d: Unhandled exception! %d\n", __FUNCTION__, __LINE__, tf->irq);
		}else if(tf->irq == 0x80) {
			do_syscall(tf);
		}
		else {
			printk("%s, %d: Unexpected exception #%d!\n", __FUNCTION__, __LINE__, tf->irq);
		assert(0);
		}
	}else 

	if (tf->irq == 1000) {
		//printk("Jere\n");
		do_timer();
		if(number==1000)
		{
	         disable_interrupt();
	         schedule();
		 number=0;
		}
		else number++;
	
	}
	else if(tf->irq==1014) return;	
	else if (tf->irq == 1001) {
		uint32_t code = in_byte(0x60);
		uint32_t val = in_byte(0x61);
		out_byte(0x61, val | 0x80);
		out_byte(0x61, val);

		printk("%s, %d: key code = %x\n", __FUNCTION__, __LINE__, code);
		do_keyboard(code);
	} else {
		printk("Undefined %x\n", tf->irq);
		assert(0);
	printk("&&&&&\n");
	}
}
uint32_t Get_seg_off() {
	 return offset;
}
