#include <stdlib.h>
#include <stdio.h>
#include <dos.h>

#include "keys.h"

//void chkey(char butt);
//void interrupt NewInt9(...);
void far (*mainkey)(unsigned char);

void chkey(unsigned char butt) {
	//printf("Сканкод: %d\n",butt);
	//return;
}

void Keys::on(void) {
	if ( newfunc!=NULL ) {
		setvect(0x09, newfunc); /* уст новый вектор прер */
	}
}

void Keys::off(void) {
	if ( oldfunc!=NULL ) {
		setvect(0x09, oldfunc); /* уст новый вектор прер */
	}
}

void Keys::set(void far (*prog)(unsigned char)) {
	asm cli;
	mainkey=prog;
	asm sti;
	return;
}

void interrupt NewInt9(...) {
	char butt;

	asm {
		cli
		in	al,60h	;считаем введенный символ
		mov	butt,al

		in	al,61h	;инициализация клавиатуры
		mov	ah,al
		or	al,80h
		out	61h,al
		mov	al,ah
		out	61h,al
		sti
	}

	(*mainkey)(butt);   //вызоп п/п обработки клавиши

	asm {               ;конец прерывания
		mov	al,20h
		out	20h,al
	}
	return;
}
