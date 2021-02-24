#ifndef _KERNIVT_H_
#define _KERNIVT_H_

typedef void interrupt (*pint)(...);

extern volatile unsigned int flagevent;

#define PREPAREENTRY(numEntry, callOldIntrInd)\
void interrupt intr##numEntry(...);\
IVTEntry newEntry##numEntry(numEntry, intr##numEntry);\
void interrupt intr##numEntry(...){\
newEntry##numEntry.signal();\
if (callOldIntrInd)\
	newEntry##numEntry.oldfun();\
dispatch();\
}\

class KernelEv;
class IVTEntry{
public:
	IVTEntry(unsigned char numEntry, pint inter);
	~IVTEntry();

	void signal();
	void oldfun();
	static IVTEntry* getentry(unsigned char numEntry);

protected:
	friend class KernelEv;

private:
	unsigned char entry;
	pint oldrout;
	pint newrout;
	KernelEv* kernelevent;
	static IVTEntry* tabela[256];

};


class Event;
class KernelSem;
class Thread;

class KernelEv{
public:
	KernelEv(unsigned char ivtNo, Event* ev);
	~KernelEv();

	void wait();

protected:
	friend class Event;
	friend class IVTEntry;
	void signal();
private:
	int val;
	Event* event;
	Thread* myThread;
	unsigned char num;
	unsigned int blkflag;
};

#endif
