#include "semaphor.h"
#include "kernsem.h"
#include "pcb.h"

Semaphore::Semaphore(int init){
	this->myImpl = new KernelSem(init);
}

Semaphore::~Semaphore(){
	delete this->myImpl;
}

int Semaphore::val() const{
	return this->myImpl->val;
}

int Semaphore::wait(Time maxTimeToWait){
	return this->myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	return this->myImpl->signal(n);
}
