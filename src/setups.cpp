#include "setups.h"
#include "timer.h"
#include <dos.h>
#include <iostream.h>

unsigned oldisr08seg;
unsigned oldisr08off;

void init(){

	asm{
		cli
		push ax
		push es

		mov ax, 0
		mov es, ax

		mov ax, word ptr es:0020h
		mov word ptr oldisr08off, ax
		mov word ptr es:0180h, ax
		mov ax, word ptr es:0022h
		mov word ptr oldisr08seg, ax
		mov word ptr es:0182h, ax

		mov word ptr es:0020h, offset timer
		mov word ptr es:0022h, seg timer

		pop es
		pop ax
		sti
	}
	cout<<"Promena prekidne rutine"<<endl;

}

void restore(){

	asm{
		cli
		push ax
		push es

		mov ax, 0
		mov es, ax

		mov ax, word ptr oldisr08off
		mov word ptr es:0020h, ax
		mov ax, word ptr oldisr08seg
		mov word ptr es:0022h, ax

		pop es
		pop ax
		sti
	}

	cout<<"vracena prekidna rutina"<<endl;

}
