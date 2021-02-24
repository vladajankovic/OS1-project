#include "kernsem.h"
#include "pcb.h"
#include "timer.h"
#include "SCHEDULE.H"
#include <iostream.h>

volatile unsigned semflag = 0;
static int sidsem = 0;
static Semlist* slista = 0;

KernelSem::KernelSem(int init){
	lock;
	this->val = init;
	this->head = 0;
	this->id = ++sidsem;

	Semlist* novi = new Semlist;
	novi->sem = this;
	novi->next = slista;
	slista = novi;
	unlock;
}

KernelSem::~KernelSem(){
	lock;
	for(int i = 0; i<-val; i++){
		get();
	}
	Semlist* tek1 = slista;
	Semlist* tek2 = slista;
	while(tek1 != 0){
		if(tek1->sem->id == this->id)
			break;
		tek1 = tek1->next;
	}
	if(tek1 == slista){
		slista = slista->next;
		tek1->sem = 0;
		tek1->next = 0;
		delete tek1;
	}
	else{
		while(tek2->next != tek1)
			tek2 = tek2->next;
		tek2->next = tek1->next;
		tek1->sem = 0;
		tek1->next = 0;
		delete tek1;
	}
	unlock;
}

int KernelSem::wait(Time waiting){

	if(--val < 0){
		lock;
		add(running, waiting);
		semflag = 1;
		unlock;
		dispatch();
	}

	if(running->unblocked == 1){
		running->unblocked = 0;
		return 0;
	}

	return 1;
}

int KernelSem::signal(int n){
	lock;
	if(n < 0){
		unlock;
		return n;
	}
	if(n == 0){
		get();
		++val;
		unlock;
		return 0;
	}
	int unblocked = 0;
	for(int i = 0; i<n; i++){
		unblocked = unblocked + get();
		++val;
	}
	unlock;
	return unblocked;
}

void KernelSem::add(PCB* pcb1, Time waiting1){
	Blockqueue* novi = new Blockqueue;
	novi->pcb = pcb1;
	novi->waiting = waiting1;
	novi->next = head;
	head = novi;
	//####################################################################### Razmotri Fifo raspored ################################################################
}

int KernelSem::get(){
	Blockqueue* tek = head;
	if(tek == 0) return 0;
	head = head->next;
	Scheduler::put(tek->pcb);
	tek->pcb = 0;
	tek->next = 0;
	delete tek;
	return 1;
}

void KernelSem::reducetime(){
	Blockqueue* tek1 = head;
	while(tek1 != 0){
		if(tek1->waiting == 0){
			tek1 = tek1->next;
			continue;
		}
		--tek1->waiting;
		if(tek1->waiting == 0){
			tek1->pcb->unblocked = 1;
			Scheduler::put(tek1->pcb);
			if(tek1 == head){
				head = head->next;
				tek1->pcb = 0;
				tek1->next = 0;
				delete tek1;
				tek1 = head;
			}
			else{
				Blockqueue* tek2 = head;
				while(tek2->next != tek1)
					tek2 = tek2->next;
				tek2->next = tek1->next;
				tek1->pcb = 0;
				tek1->next = 0;
				delete tek1;
				tek1 = tek2->next;
			}
		}
		else{
			tek1 = tek1->next;
		}
	}
}

void reducewaiting(){
	Semlist* tek = slista;
	while(tek != 0){
		tek->sem->reducetime();
		tek = tek->next;
	}
}
