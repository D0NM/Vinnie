//М.Братусь 12-06-95 12:43pm

#include <stdlib.h>
#include <alloc.h>
#include <io.h>
#include <dos.h>
#include <fcntl.h>
#include <string.h>
#include <stat.h>
#include "famegraf.h"

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

block oldfnt;
block newfnt;

unsigned char leftbit(unsigned char c);
unsigned char rightbit(unsigned char c);

void main (void) {
	unsigned int i,j,k,l,r;

	oldfnt=(block)famemalloc((unsigned long)2048);
	newfnt=(block)famemalloc((unsigned long)2048+256);
	_fmemset(newfnt,0,(unsigned long)2048+256);
	SetLib("tmp");
	GetLib("tmp.fnt",oldfnt);

	for( i=0; i<256; i++ ) {
		r=l=8;
		for( j=0; j<8; j++ ) {
			l=min(l,leftbit(oldfnt[i*8+j]));
			r=min(r,rightbit(oldfnt[i*8+j]));
		}
		//сдвиг и перенос шрифтов
		for( j=0; j<8; j++ ) {
			newfnt[i*8+j]=((oldfnt[i*8+j])<<l);
		}
		newfnt[2048+i]=8-(l+r);
		if (newfnt[2048+i]<2) {
			//это мелкие символы сдвинем вправо на 1
			newfnt[2048+i]=4;
			for( j=0; j<8; j++ ) {
				newfnt[i*8+j]=( ((oldfnt[i*8+j])>>1)&127);
			}
		}
		printf("%u [%d<->%d] = %d\n",i,l,r,newfnt[2048+i]);
	}
	PutLib("tmp.fit",newfnt,(unsigned long)2048+256);
    SetLib("");
}

unsigned char leftbit(unsigned char c) {
	int i;
	for( i=0; i<7; i++ ) {
		if( (c<<i)&128 ) {
			return i;
		}
	}
	return 8;
}

unsigned char rightbit(unsigned char c) {
	int i;
	for( i=0; i<7; i++ ) {
		if( (c>>i)&1 ) {
			return i;
		}
	}
	return 8;
}

void fatalerror(char *t) {
	//выход по фатальной ошибке
	puts(t);
	exit(1);
}
