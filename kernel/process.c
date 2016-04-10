#include "include/process.h"
#include "include/list.h"
#include "include/common.h"
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
/*uint32_t fork(PCB *l)
{
	uint32_t vaddr=l->va;

        segment *tmp=mm_malloc(vaddr,1000,0x2);//权限需要修改
	uint32_t pa_cs=l->pa_cs;
	uint32_t pa_ds=l->pa_ds;
	memcpy(tmp->base,pa_cs,20000);
	memcpy(tmp
	PCB* son=new_process();
	TrapFrame *tf1;
	tf1=&l->tf;
	son->tf=*tf1;
//	*(son->tf)=l->tf;
	(son->tf).eax=0;
	set_tss_esp0((int)son->kstack+4096);
	return son->pid;

}*/
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
//		printk("pid= %d\n",current->pid);
	       if(current->pid==1)exe(&(current->tf));
		ready_pcb(current);
}
