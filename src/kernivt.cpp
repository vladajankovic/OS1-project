#include "kernivt.h"
#include "kernsem.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include <dos.h>

unsigned int int65h = 0x65;
pint oldroutine = 0;

IVTEntry* IVTEntry::tabela[256] = {0};

volatile unsigned int flagevent = 0;

IVTEntry::IVTEntry(unsigned char num, pint inter){
	lock;
	this->entry = num;
	this->newrout = inter;
	this->kernelevent = 0;

	IVTEntry::tabela[num] = this;

	asm cli;
	this->oldrout = getvect(num);
	setvect(num, this->newrout);
	asm sti;

	unlock;

}

IVTEntry::~IVTEntry(){
	lock;
	asm cli;
	setvect(entry, this->oldrout);
	asm sti;
	oldfun();
	IVTEntry::tabela[this->entry] = 0;

	unlock;
}

IVTEntry* IVTEntry::getentry(unsigned char num){
	return IVTEntry::tabela[num];
}

void IVTEntry::oldfun(){
	asm cli;
	oldroutine = this->oldrout;
	setvect(int65h, oldroutine);
	asm int 65h;
	asm sti;
}

void IVTEntry::signal(){
	this->kernelevent->signal();
}

//##########################################################

KernelEv::KernelEv(unsigned char ivtno, Event* ev){
	lock;
	this->event = ev;
	this->num = ivtno;
	this->myThread = running->thread;
	IVTEntry::getentry(ivtno)->kernelevent = this;
	this->val = 0;
	this->blkflag = 0;
	unlock;
}

KernelEv::~KernelEv(){
	lock;
	this->myThread = 0;
	this->event = 0;
	unlock;
}

void KernelEv::wait(){
	if(this->myThread == running->thread){
		if(--this->val == -1){
			lock;
			flagevent = 1;
			blkflag = 1;
			unlock;
			dispatch();
		}
		else
			this->val = 0;
	}
}

void KernelEv::signal(){
	if(this->val < 0){
		this->val = 0;
		if(blkflag == 1){
			PCB::PCBlist* tek = PCB::lista;
			while(tek != 0){
				if(tek->pcb->thread == this->myThread)
					break;
				tek = tek->next;
			}
			Scheduler::put(tek->pcb);
			blkflag = 0;
		}
	}
	else this->val = 1;
}





