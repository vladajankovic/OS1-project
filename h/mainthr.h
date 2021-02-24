#ifndef _MAINTHR_H_
#define _MAINTHR_H_

#include <iostream.h>
#include "thread.h"

class PCB;

class MainThread{
public:

	MainThread();
	~MainThread();
	PCB* getmainpcb(){
		return mainpcb;
	}

protected:
	friend class PCB;

private:
	PCB* mainpcb;

};


class Idle: public Thread{
public:
	Idle():Thread(1024, 1){};
	~Idle(){};

protected:
	virtual void run();
};


#endif
