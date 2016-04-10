#include "include/process.h"
#include "include/list.h"
#include "include/memory.h"
#include "include/common.h"
#include "include/string.h"
//#include  "include/x86.h"
PCB pcb[NR_PCB];
ListHead pcb_head;
ListHead unused_pcb;
ListHead ready;
ListHead sleep;
static PCB idle;
static uint32_t number;
 void set_tss_esp0(int);
PCB *current =&idle;
segment * mm_malloc(uint32_t,uint32_t,uint32_t);
void exe(struct TrapFrame *);
 void ready_pcb(PCB *l);
void init_process() {
	list_init(&pcb_head);
	list_init(&unused_pcb);
	list_init(&ready);
	list_init(&sleep);
	int i;
	for( i = 0; i < NR_PCB; ++ i) {
		list_add_after(&unused_pcb, &pcb[i].list);
	}
	ready_pcb(&idle);
}

PCB *new_process() {
//	if(list_empty(&unused_pcb)) printk("Process full!\n");
	assert(!list_empty(&unused_pcb));
		PCB  *new_pcb = list_entry(unused_pcb.next,PCB,list);
		++number;
		new_pcb->pid=number;
		list_del(&new_pcb->list);
		list_add_after(&pcb_head, &new_pcb->list);
		return new_pcb;
	}
	
void get_pcb(PCB *c)
{
     //current=c;
     ready_pcb(c);
//  exe(&(c->tf));
}
void ready_pcb(PCB *l)
{

		list_del(&l->list);
		if(list_empty(&ready)) list_add_before(&ready,&l->list);
		else list_add_before(&idle.list,&l->list);
	//	exe(&(l->tf));
}
void Sleep (PCB *l,uint32_t Time)
{	
		list_del(&l->list);
		l->time=Time;
		list_add_before(&sleep,&l->list);
}
/*uint32_t fork()
{
	return 1;
}*/
uint32_t fork()
{
	uint32_t vaddr=current->va;
        printk("fork\n");
	printk("current %d\n",current->pid);
	PCB* son=new_process();
        segment *tmp_c=mm_malloc(vaddr,1000,0xa);//权限需要修改
        segment *tmp_d=mm_malloc(vaddr,1000,0x2);
	uint32_t pa_cs=current->pa_cs;
	uint32_t pa_ds=current->pa_ds;
	printk("pa_cs %d\n",pa_cs);
	memcpy((void*)tmp_c->base,(void*)pa_cs,20000);
	memcpy((void *)tmp_d->base,(void*)pa_ds,20000);
        son->va=current->va;
        son->pa_cs=tmp_c->base;	
	son->pa_ds=tmp_d->base;
//	son->tf=(TrapFrame )((int)son->kstack+(int)l->tf-(int)l->kstack);
	int i;
	for (i=0;i<KSTACK_SIZE;++i)
		son->kstack[i]=current->kstack[i];
//	*(son->tf)=l->tf;
	(son->tf).eax=0;
	printk("son_cs %d\n",(current->tf).cs);
	printk("son_ds %d\n",(current->tf).ds);
	ready_pcb(son);
	return son->pid;

}
void schedule(void)
{
  //            printk("^^^\n"); 
	if (current!=&idle)
	       {
		       list_add_tail(&current->list,&ready);
	       }
		assert(!list_empty(&ready)); 
		current=list_entry(ready.next,PCB,list);
		list_del(&current->list);
	//	if(current->pid!=0)
			printk("pid= %d\n",current->pid);
	       if(current->pid!=0)
	       {
		       printk("pid1= %d\n",current->pid);
	set_tss_esp0((int)current->kstack+4096);
		       exe(&(current->tf));
		}
		ready_pcb(current);

}
