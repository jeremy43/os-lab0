#include "include/process.h"
#include "include/list.h"

PCB pcb[NR_PCB];
ListHead pcb_head;
ListHead unused_pcb;
void init_process() {
	list_init(&pcb_head);
	list_init(&unused_pcb);
	int i;
	for( i = 0; i < NR_PCB; ++ i) {
		list_add_after(&unused_pcb, &pcb[i].list);
	}
}

PCB *new_process() {
//	if(list_empty(&unused_pcb)) printk("Process full!\n");
	assert(!list_empty(&unused_pcb));
	ListHead *new_pcb = unused_pcb.next;
	list_del(new_pcb);
	list_add_after(&pcb_head, new_pcb);
	return list_entry(new_pcb, PCB, list);
}
