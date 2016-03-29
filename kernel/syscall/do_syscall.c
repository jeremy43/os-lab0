//#include "irq.h"
#include "x86/x86.h"
#include "string.h"
#include "common.h"
#include"device/timer.h"
//#include <sys/syscall.h>
enum {hhhh,SYS_write1,SYS_time1,SYS_keyboard1, SYS_draw};
//void add_irq_handle(int, void (*)(void));
//void mm_brk(uint32_t);
void serial_printc(char);
bool query_key(int);
/*int fs_open(const char *pathname, int flags);
int fs_read(int fd, void *buf, int len);
int fs_write(int fd, void *buf, int len);
int fs_lseek(int fd, int offset, int whence);
int fs_close(int fd);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}
*/
static void sys_write(struct TrapFrame *tf) {
//#ifdef HAS_DEVICE
	int i;
	for(i = 0; i < tf->ecx; ++ i)
		serial_printc(*(char *)(tf->ebx + i));
//#else				
//	asm volatile (".byte 0xd6" : : "a"(2), "c"(tf->ecx), "d"(tf->edx));
//#endif
	tf->eax = tf->ecx;
}
int get_time();
static void sys_keyboard(struct TrapFrame *tf)
{
	int i;
	bool * key = (bool *)tf->ebx;
	for(i = 0;i < 4; ++ i)
	   if( query_key(i)) key[i]=1;
	   else key[i]=0;
}

static void sys_draw(struct TrapFrame *tf) {
	memcpy((void*)0xA0000, (void*)tf->ebx, 320*200);
}
void do_syscall(struct TrapFrame *tf) {
	switch(tf->eax) {
		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
	/*	case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;*/

//		case SYS_brk: /*panic("@@@");*/sys_brk(tf); break;
		case SYS_write1: sys_write(tf); break;
		case SYS_time1:tf->eax=get_time(); break;
	        case SYS_keyboard1:sys_keyboard(tf);break;
		case SYS_draw: sys_draw(tf);break;
/*		case SYS_open : 
		tf->eax = fs_open((char *)tf->ebx, tf->ecx); break;
		case SYS_read : 
//			Log("%x %x %x %x %x\n", tf->ebx, tf->ecx, tf->edx, tf->edi, tf->esi);
			tf->eax = fs_read(tf->ebx, (void *)tf->ecx, tf->edx); break;
		case SYS_lseek : tf->eax = fs_lseek(tf->ebx, tf->ecx, tf->edx); break;
		case SYS_close : tf->eax = fs_close(tf->ebx); break;

*/		/* TODO: Add more system calls. */

		default: printk("Unhandled system call: id = %d", tf->eax);
	}
}

