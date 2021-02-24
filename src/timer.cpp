#include "timer.h"
#include "kernsem.h"
#include "mainthr.h"
#include "pcb.h"
#include "kernivt.h"
#include "SCHEDULE.H"
#include <iostream.h>

volatile unsigned cnt = 2;
volatile unsigned lockFlag = 1;
unsigned demandswitch = 0;
extern void tick();
extern void reducewaiting();
extern Idle* idlethread;

unsigned tsp;
unsigned tss;
unsigned tbp;

volatile unsigned flag = 0;

void interrupt timer(){

	if(!demandswitch){
		asm int 60h;
				tick();
		reducewaiting();
	}


	if(cnt == 0) flag = 1;
	if (!demandswitch && flag == 0){
		--cnt;
	}
	if ((cnt == 0 && flag == 0) || demandswitch) {
		if(lck == 0){
			demandswitch = 0;
			asm {
				// cuva sp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			running->bp = tbp;
			running->sp = tsp;
			running->ss = tss;


			if(running->finished == 0 && semflag == 0 && flagevent == 0 ) Scheduler::put(running);
			running = Scheduler::get();
			if(running == 0){
				running->thread = idlethread;
			}

			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;

			cnt = running->timeslice;

			asm {
				mov bp, tbp
				mov sp, tsp   // restore sp
				mov ss, tss
			}

			semflag = 0;
			flagevent = 0;
		}
		else lckflag = 1;

	}

	
	flag = 0;


}

