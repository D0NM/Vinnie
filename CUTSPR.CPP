#include	<conio.h>
#include	<io.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<ctype.h>
#include	<bios.h>
#include	<assert.h>
#include	<dos.h>
#include <mem.h>

#include "famegraf.h"
#include "mouse.h"

Mouse m;

block sprbuf;

void ReadData(char *data_name,block dest,unsigned long n) {
	int fdz;
	if ( (fdz=open (data_name,O_BINARY | O_RDONLY)) < 0) exit (1);
	if ((int)read (fdz, dest, n) == -1 ) exit (1);
	close (fdz);
}

void main(void) {
	int x,y;

	SetLib("tmp");

	InitSpr(); //инициализируем спрайты

	InitGraph();
	Vga256();

	ReadData("aatemp.pic",(block)SpritesScreen,(unsigned long)320*200);
	CopyToScreen(SpritesScreen);
	NormalScreen();
	sprbuf=(block)SpritesScreen;

	for ( y=0; y<12; ++y ) {
		for ( x=0; x<16; ++x ) {
			GetImg(x*16,y*16,16,16,sprbuf);
			sprbuf+=256;
			Bar(x*16,y*16,16,16);
		}
	}
	PutLib("__fon.bin",(block)SpritesScreen,(unsigned long)16*16*16*12);

	textmode(C80);
    SetLib("");
	return;
}

void fatalerror(char *t) {
	//выход по фатальной ошибке
    SetLib("");
	puts(t);
	exit(1);
}
