/* start.S的主要功能是切换在实模式工作的处理器到32位保护模式。为此需要设置正确的
 * GDT、段寄存器和CR0寄存器。C语言代码的主要工作是将磁盘上的内容装载到内存中去。
 * 磁盘镜像的结构如下：
	 +-----------+------------------.        .-----------------+
	 |   引导块   |  游戏二进制代码       ...        (ELF格式)     |
	 +-----------+------------------`        '-----------------+
 * C代码将游戏文件整个加载到物理内存0x100000的位置，然后跳转到游戏的入口执行。 */

//#include "../include/boot.h"
#include "../include/x86/x86.h"
#include "../include/process.h"
#include "../include/memlayout.h"
#include "../include/memory.h"
#include "../include/string.h"
#define FL_IF 0x00000200
#define SECTSIZE 512
#define GDT_ENTRY(n)    ((n) << 3)
void readseg(unsigned char *, int, int);
void set_tss_esp0(int);
void get_pcb(PCB*);
//void mm_malloc(pde_t *pgdir, void *va, unsigned long size);
segment* mm_malloc(uint32_t,uint32_t, uint32_t);
void
load(void) {
	PCB* current=new_process();
	struct ELFHeader *elf;
	struct ProgramHeader *ph, *eph;
	unsigned char *pa, *i;

	/* 因为引导扇区只有512字节，我们设置了堆栈从0x8000向下生长。
	 * 我们需要一块连续的空间来容纳ELF文件头，因此选定了0x8000。 */
       // elf = (struct ELFHeader*)0x08000;
	/* 读入ELF文件头 */
	uint8_t temp[4096];
	elf=(void*)temp;
	readseg((unsigned char*)elf, 4096, 100*1024);
	printk("fuck\n");
	//        memcpy(game_pgdir, entry_pgdir, 4096);
	//lcr3(((uintptr_t)game_pgdir) - KERNBASE);
	/* 把每个program segement依次读入内存 */
	ph = (struct ProgramHeader*)((char *)elf + elf->phoff);
	eph = ph + elf->phnum;
         segment *tmp[3];	
	int p_flag[2]={0xa,0x2};
     	int cnt=-1;
	int va;
	for(; ph < eph; ph ++)
       if(ph->type==1)	{
		va = (uint32_t)ph->vaddr; /* 获取虚拟地址 */
		cnt++;
		tmp[cnt]=mm_malloc(va,ph->memsz,p_flag[cnt]);
		//printk("die\n");
//		boot_map_region(current->updir, pa, ph->memsz, PTE_W | PTE_U);
	         pa = (unsigned char*)tmp[cnt]->base;
		readseg(pa, ph->filesz, ph->off+ 1024*100); /* 读入数据 */
		for (i = pa + ph->filesz; i < pa + ph->memsz; *i ++ = 0);
	}
	//enable_interrupt();
	printk("LOAD\n");
	//((void(*)(void))elf->entry)();

	 uint32_t eflags=read_eflags();
	TrapFrame *tf=&current->tf;
	set_tss_esp0((int)current->kstack+KSTACK_SIZE);
       tf->eip=elf->entry;
        tf->cs = GDT_ENTRY(1);
	tf->eflags=eflags | FL_IF;
	tf->ss = GDT_ENTRY(2);
	printk("&&&\n");
	tf->esp = 0x00300000;
	get_pcb(current);
}
//	 tf->esp = 0x2000000 - tmp[1]->base + va;
void exe(TrapFrame *tf)
{
       asm volatile("movl %0, %%esp" : :"a"((int)tf));
         asm volatile("popa");
         asm volatile("addl %0, %%esp" : :"a"(8));

	 asm volatile("mov 24(%esp), %eax\n\t"				    
			 "movl %eax, %ds\n\t"				                              "movl %eax, %es\n\t"			                             "movl %eax, %fs\n\t"
  			 "movl %eax, %gs\n\t");
	           asm volatile("iret");
           printk("563\n");
}

void
waitdisk(void) {
	while((in_byte(0x1F7) & 0xC0) != 0x40); /* 等待磁盘完毕 */
}

/* 读磁盘的一个扇区 */
void
readsect(void *dst, int offset) {
	int i;
	waitdisk();
	out_byte(0x1F2, 1);
	out_byte(0x1F3, offset);
	out_byte(0x1F4, offset >> 8);
	out_byte(0x1F5, offset >> 16);
	out_byte(0x1F6, (offset >> 24) | 0xE0);
	out_byte(0x1F7, 0x20);
	waitdisk();
	for (i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = in_long(0x1F0);
	}
}

/* 将位于磁盘offset位置的count字节数据读入物理地址pa */
void
readseg(unsigned char *pa, int count, int offset) {
	unsigned char *epa;
	epa = pa + count;
	pa -= offset % SECTSIZE;
	offset = (offset / SECTSIZE) + 1;
	for(; pa < epa; pa += SECTSIZE, offset ++)
		readsect(pa, offset);
}
