#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "semaphor.h"
#include "kernsem.h"
#include "Kernivt.h"

#define lock lck++;
#define unlock {	\
	if(--lck==0 && lckflag == 1) \
		dispatch();\
}

extern volatile int lck;
extern volatile int lckflag;


extern PCB* running;

class PCB{
public:
	unsigned* stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned timeslice;
	unsigned finished;


	PCB(Time time){
		lock;
		timeslice=time; finished = 0; unblocked = 0;
		unlock;
	}
	PCB(StackSize size, Time time, Thread* thread);
	~PCB();
	int getID();
	static int RunningId();

	static void wrapper();

protected:
	friend class Thread;
	friend class KernelSem;
	friend class KernelEv;
	friend class Semaphore;
	friend class Idle;
	friend void interrupt timer();
private:
	Thread* thread;
	int id;
	struct PCBlist{
		PCB* pcb;
		PCBlist* next;
	};
	static PCBlist* lista;
	int unblocked;
};




#endif
