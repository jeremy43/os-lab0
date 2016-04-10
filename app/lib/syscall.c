#include "../include/common.h"
//#include <sys/syscall.h>
//#include <sys/stat.h>
enum {hhh=0,SYS_write1,SYS_time1,SYS_keyboard1, SYS_draw,SYS_fork};
 //__attribute__((__noinline__))
int syscall(int id, int val1, int val2, int val3) {
	int ret;
//	Log("%x %x %x %x", args[0], args[1], args[2], args[3]);	
	asm volatile("int $0x80": "=b"(ret) : "a"(id), "b"(val1), "c"(val2), "d"(val3));
        //printf("ret= %d\n",ret);
	return ret;
}

void  write(char *buf, int len) {
	 syscall(SYS_write1, (int)buf, len, 0); 
}
void fork()
{
	syscall(SYS_fork,0,0,0);
}
int time(void)
{
	return syscall(SYS_time1,0, 0, 0);
}

void UPDATE_kbd(int * key)
{
	 syscall(SYS_keyboard1, (int)key, 0, 0);
          asm volatile("cli");

}

void realdraw(uint8_t *buf) {
	syscall(SYS_draw, (int)buf, 0, 0);
}
/*off_t lseek(int fd, off_t offset, int whence) {
	return syscall(SYS_lseek, fd, offset, whence);
	nemu_assert(0);
	return 0; 
}

void *sbrk(int incr) {
	extern char end;	
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0) {
		heap_end = &end;
	}
	nemu_assert(syscall(SYS_brk, heap_end + incr) == 0);
	prev_heap_end = heap_end;
	heap_end += incr;

	return prev_heap_end;
}

int close(int fd) {
	return syscall(SYS_close, fd);
	nemu_assert(0);
	return 0; 
}

int fstat(int fd, struct stat *buf) {
	buf->st_mode = S_IFCHR;
	return 0;
}

int isatty(int fd) {
	return 0;
}*/
