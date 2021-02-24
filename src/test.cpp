#include "thread.h"
#include "timer.h"
#include "setups.h"
#include "mainthr.h"
#include "pcb.h"
#include "semaphor.h"
#include <iostream.h>

extern int userMain (int argc, char* argv[]);
extern Idle* idlethread;
extern MainThread* mt;


int main(int argc, char* argv[]){

	init();

	lock;
	int ret;
	mt = new MainThread();
	running = mt->getmainpcb();
	idlethread = new Idle();
	idlethread->start();
	int i;
	unlock;

	ret = userMain(argc, argv);


	restore();

	return ret;

}
