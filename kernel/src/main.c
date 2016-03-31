#include "game.h"
#include "irq.h"
#include "x86/x86.h"
#include "device/timer.h"
#include "device/palette.h"
#include "assert.h"

void init_serial();
void init_timer();
void init_idt();
void init_intr();
void set_timer_intr_handler(void(void));
void set_keyboard_intr_handler(void(int));
void timer_event();
void load();
void init_process();
void init_seg();
void keyboard_event(int);
void main_loop();
void
kernel_init(void) {
	init_serial();
	init_timer();
	init_idt();
	init_intr();
	set_timer_intr_handler(timer_event);
	set_keyboard_intr_handler(keyboard_event);

	//printk("game start!\n");
	enable_interrupt();
	init_seg();
	init_process();
	load();
//	main_loop();
	assert(0); /* main_loop是死循环，永远无法返回这里 */
}
