#include "include/process.h"
#include "include/list.h"

PCB pcb[NR_PCB];
ListHead pcb_head;
ListHead unused_pcb;
ListHead ready;
ListHead sleep;
static PCB idle;
static uint32_t number;
PCB *current =&idle;
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
	
PCB * running_process()
{
		return current;
}
void ready_pcb(PCB *l)
{


		list_del(&l->list);
		if(list_empty(&ready)) list_add_before(&ready,&l->list);
		else list_add_before(&idle.list,&l->list);
}
void Sleep (PCB *l,uint32_t Time)
{
		list_del(&l->list);
		l->time=Time;
		list_add_before(&sleep,&l->list);
}
void schedule(void)
{

		assert(!list_empty(&ready)); 
		current=list_entry(ready.next,PCB,list);
		ready_pcb(current);
}
