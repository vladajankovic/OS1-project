#include "Event.h"
#include "kernivt.h"
#include "pcb.h"

Event::Event(IVTNo ivtNo){
	this->myImpl = new KernelEv(ivtNo, this);
}


Event::~Event(){
	delete this->myImpl;
}

void Event::wait(){
	this->myImpl->wait();
}

void Event::signal(){
	this->myImpl->signal();
}

