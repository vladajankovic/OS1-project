#include "pcb.h"
#include <dos.h>
#include <iostream.h>

volatile int lck = 0;
volatile int lckflag = 0;



PCB* running;
static int ids = 0;
PCB::PCBlist* PCB::lista = 0;

PCB::PCB(unsigned long size, unsigned time, Thread* thr){

	lock;

	if(size>65536) size = 65536;

	this->thread = thr;
	unsigned* newstack = new unsigned[size/sizeof(unsigned)];
	newstack[size-1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	newstack[size-2] = FP_SEG(wrapper);
	newstack[size-3] = FP_OFF(wrapper);

	sp = FP_OFF(newstack+size-12);
	bp = FP_OFF(newstack+size-12);
	ss = FP_SEG(newstack+size-12);
#endif
	stack = newstack;
	finished = 0;
	unblocked = 0;
	timeslice = time;
	id = ++ids;

	PCBlist* novi = new PCBlist;
	novi->pcb = this;
	novi->next = lista;
	lista = novi;


	unlock;
}

PCB::~PCB(){
	lock;
	delete[] this->stack;
	PCB::PCBlist* tek1 = this->lista;
	PCB::PCBlist* tek2 = this->lista;
	while(tek1 != 0){
		if(tek1->pcb->id == this->id)
			break;
		tek1 = tek1->next;
	}
	if(tek1 == lista){
		lista = lista->next;
		tek1->pcb = 0;
		tek1->next = 0;
		delete tek1;
	}
	else{
		while(tek2->next != tek1)
			tek2 = tek2->next;
		tek2->next = tek1->next;
		tek1->pcb = 0;
		tek1->next = 0;
		delete tek1;
	}
	unlock;
}

int PCB::getID(){
	return this->id;
}

int PCB::RunningId(){
	return running->getID();
}

void PCB::wrapper(){
	running->thread->run();
	running->thread->myPCB->finished = 1;
	dispatch();
}
