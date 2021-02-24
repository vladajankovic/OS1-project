#include "thread.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include <iostream.h>

extern unsigned demandswitch;
void interrupt timer();


Thread::Thread(StackSize stackSize, Time timeSlice){
	this->myPCB = new PCB(stackSize, timeSlice, this);
}

Thread::~Thread(){delete this->myPCB;};

ID Thread::getId(){
	return this->myPCB->getID();
}

ID Thread::getRunningId(){
	return PCB::RunningId();
}

Thread* Thread::getThreadById(ID id1){
	PCB::PCBlist* tek = PCB::lista;
	while(tek!=0){
		if(tek->pcb->id == id1)
			return tek->pcb->thread;
		tek = tek->next;
	}
	return 0;
}

void Thread::start(){
	if(this->myPCB->finished != 1){
		Scheduler::put(this->myPCB);
	}
}

void dispatch(){
	asm cli;
	demandswitch = 1;
	timer();
	asm sti;
}

void Thread::waitToComplete(){
	while(this->myPCB->finished != 1){
		dispatch();
	}
}
