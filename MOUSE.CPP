#include <stdlib.h>

#include "mouse.h"

int Mouse::init(void)
{
	asm MOV	AX,0H
	asm INT 33H
	if (_AX==0xFFFF) return _BX;
	return 0;
}

void Mouse::on(void)
{
	if ( present ) {
		asm MOV AX,01H
		asm INT 33H
	}
}

void Mouse::off(void)
{
	if ( present ) {
		asm MOV AX,02H
		asm INT 33H
	}
}

void Mouse::image(int x,int y,char* form)
{
	int i,j;
	void far* m;

	if ( present ) {
		j=32;
		for (i=0;i<32;++i) {
			mask[i]=form[i]&0xff;
			mask[j++]=form[i];
		}

		m=&mask;

		asm PUSH AX
		asm PUSH BX
		asm PUSH CX
		asm PUSH DX
		asm PUSH ES

		asm MOV AX,09H
		asm MOV BX,x
		asm MOV CX,y
		asm LES DX,m
		asm INT 33H

		asm POP ES
		asm POP DX
		asm POP CX
		asm POP BX
		asm POP AX
	}
}

void Mouse::status(void)
{
	int b;

	if ( present ) {

		asm PUSH AX
		asm PUSH BX
		asm PUSH CX
		asm PUSH DX
		asm PUSH ES

		asm MOV AX,03H
		asm INT 33H

		b=_BX;

		x=_CX/2;
		y=_DX;

		left.press=b&0x0001;
		right.press=b&0x0002;
		center.press=b&0x0004;

		asm POP ES
		asm POP DX
		asm POP CX
		asm POP BX
		asm POP AX

		if (left.press&&right.press&&center.press) exit(EXIT_FAILURE);
	}
}

void Mouse::set(int xs,int ys)
{
	if ( present ) {
		xs=xs*2;
		asm PUSH AX
		asm PUSH BX
		asm PUSH CX
		asm PUSH DX
		asm PUSH ES

		asm MOV AX,04H
		asm MOV CX,xs
		asm MOV DX,ys
		asm INT 33H

		asm POP ES
		asm POP DX
		asm POP CX
		asm POP BX
		asm POP AX
	}
}

void Mouse::pressed(void)
{
	int b;

	if ( present ) {
		asm PUSH AX
		asm PUSH BX
		asm PUSH CX
		asm PUSH DX
		asm PUSH ES

		asm MOV AX,05H
		asm MOV BX,0
		asm INT 33H

		b=_BX;

		left.x=_CX/2;
		left.y=_DX;

		left.press=b;

		asm MOV AX,05H
		asm MOV BX,1
		asm INT 33H

		b=_BX;

		right.x=_CX/2;
		right.y=_DX;

		right.press=b;

		asm MOV AX,05H
		asm MOV BX,2
		asm INT 33H

		b=_BX;

		center.x=_CX/2;
		center.y=_DX;

		center.press=b;

		asm POP ES
		asm POP DX
		asm POP CX
		asm POP BX
		asm POP AX
	}
}

void Mouse::released(void)
{
	if ( present ) {
		asm PUSH AX
		asm PUSH BX
		asm PUSH CX
		asm PUSH DX
		asm PUSH ES

		asm MOV AX,06H
		asm MOV BX,0
		asm INT 33H

		if (_BX>0) {
			left.x=_CX/2;
			left.y=_DX;
			left.press=0;
		}

		asm MOV AX,06H
		asm MOV BX,1
		asm INT 33H

		if (_BX>0) {
			right.x=_CX/2;
			right.y=_DX;
			right.press=0;
		}

		asm MOV AX,06H
		asm MOV BX,2
		asm INT 33H

		if (_BX>0) {
			center.x=_CX/2;
			center.y=_DX;
			center.press=0;
		}

		asm POP ES
		asm POP DX
		asm POP CX
		asm POP BX
		asm POP AX
	}
}

void Mouse::wait(void)
{	//ждем нажатия и отпускания кнопки мышки
	if ( present ) {
		do {status();} while ( left.press!=0 || right.press!=0 );
		do {status();} while ( left.press==0 && right.press==0 );
		do {status();} while ( left.press!=0 || right.press!=0 );
	}
}
