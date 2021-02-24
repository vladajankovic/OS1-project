#include "mainthr.h"
#include "pcb.h"

Idle* idlethread = 0;
MainThread* mt = 0;

MainThread::MainThread(){
	this->mainpcb = new PCB(8);
}


MainThread::~MainThread(){
	delete mainpcb;
}

void Idle::run(){
	while(1){
		if(running != mt.getID()){
			int i;
			i++;
		}
	}
}
