#ifndef _TIMER_H_
#define _TIMER_H_

extern volatile unsigned cnt;
extern unsigned demandswitch;

void interrupt timer();

#endif
