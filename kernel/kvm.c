#include "include/x86/x86.h"
//#include "include/device.h"
#include "include/string.h"
#define SECTSIZE 512
#define SELECTOR_USER(s)          ( ((s) << 3) | DPL_USER )
SegDesc gdt[NR_SEGMENTS];       // the new GDT
TSS tss;

void
 set_segment(SegDesc *ptr, uint32_t pl, uint32_t type, uint32_t base, uint32_t limit) {
    ptr->lim_15_0  = limit & 0xFFFF;
    ptr->base_15_0   = base & 0xFFFF;
    ptr->base_23_16  = (base >> 16) & 0xFF;
    ptr->type = type;
    ptr->s = 1;
    ptr->dpl = pl;
    ptr->p =1;
    ptr->lim_19_16 = (limit >> 16 ) & 0xF;
    ptr->avl = 0;
    ptr->rsv1 = 0;
    ptr->db = 1;
    ptr->g= 1;
    ptr->base_31_24  = (base >> 24) & 0xFF;
 }

void
init_seg() { // setup kernel segements
        memset(gdt,0,sizeof(gdt));
	gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_KERN);
	gdt[SEG_KDATA] = SEG(STA_W,         0,       0xffffffff, DPL_KERN);
	gdt[SEG_UCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_USER);
	gdt[SEG_UDATA] = SEG(STA_W,         0,       0xffffffff, DPL_USER);
	gdt[SEG_TSS] = SEG16(STS_T32A,      &tss, sizeof(TSS)-1, DPL_KERN);
        gdt[SEG_TSS].s = 0;
	set_gdt(gdt, sizeof(gdt));
        ltr(SELECTOR_USER(SEG_TSS));
    /*
	 * 初始化TSS
	 */
	/*设置正确的段寄存器*/
	asm volatile("movw %%ax,%%es":: "a" (KSEL(SEG_KDATA)));
	asm volatile("movw %%ax,%%ds":: "a" (KSEL(SEG_KDATA)));
	asm volatile("movw %%ax,%%ss":: "a" (KSEL(SEG_KDATA)));

	lldt(0);
}
void
set_tss_esp0(int esp)
{
	tss.esp0=esp;
}
void
enter_user_space(void) {
    /*
     * Before enter user space 
     * you should set the right segment registers here
     * and use 'iret' to jump to ring3
     * 进入用户空间
     */
}

void
load_umain(void) {
    /*
     * Load your app here
     * 加载用户程序
     */
}

/*void
waitdisk(void) {
	while((in_byte(0x1F7) & 0xC0) != 0x40); 
}

void
readsect(void *dst, int offset) {
	int i;
	waitdisk();
	out_byte2(0x1F2, 1);
	out_byte2(0x1F3, offset);
	out_byte2(0x1F4, offset >> 8);
	out_byte2(0x1F5, offset >> 16);
	out_byte2(0x1F6, (offset >> 24) | 0xE0);
	out_byte2(0x1F7, 0x20);

	waitdisk();
	for (i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = in_long(0x1F0);
	}
}*/
