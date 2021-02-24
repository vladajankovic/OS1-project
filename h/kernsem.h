#ifndef _KERNSEM_H_
#define _KERNSEM_H_

#include "semaphor.h"

extern volatile unsigned semflag;

struct Semlist{
	KernelSem* sem;
	Semlist* next;
};

extern Semlist* slista;

class PCB;

struct Blockqueue{
	PCB* pcb;
	Time waiting;
	Blockqueue* next;
};

class KernelSem{
public:

	KernelSem(int init);
	~KernelSem();

	int wait(Time maxWaitTime);
	int signal(int n);

	void reducetime();

protected:
	friend class Semaphore;

private:
	int id;
	int val;
	Blockqueue* head;
	void add(PCB*, Time waiting);
	int get();

};


#endif
