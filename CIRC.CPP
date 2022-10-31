#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <fcntl.h>
#include <dos.h>
#include <alloc.h>

#include "famegraf.h"
#include "mouse.h"
#include "dialog.h"

char sin_[36+9]=
	     {   0,  17,  34,  50,  64,  76,  86,  93,  98,
     		99,  98,  93,  86,  76,  64,  49,  34,  17,
		 0, -17, -34, -50, -64, -76, -86, -93, -98,
	       -99, -98, -93, -86, -76, -64, -49, -34, -17,
	         0,  17,  34,  50,  64,  76,  86,  93,  98,
};

#define maxseg 8
struct {
	int smx,smy;	/*отступ от центра*/
	int lx,ly;	/*размеры блочка*/
	block buf;	/*адрес спрайта*/
} seg[maxseg];

#define maxelem 32
struct {
	int x,y;	/*коорд центра на экране*/

	int nseg;	/*N графич примитива*/

	char r;		/*коэфф радиуса поворота ребенка*/
	char spin;	/*угол поворота 0..35*/
	int cx,cy;	/*координаты для ребенка*/
	int nc;		/*номер родителя*/
} elem[maxelem];

#define maxstrip 16
#define st_still 0
#define st_roll 1
#define st_go 2

int nstrip=1;
int striply=1;
struct {
	int x,y; /*начальная позиция*/
	int ly;	/*высота полоски*/
	int sx,sy; /**/
	int st;
	int typ;
} strip[maxstrip];
void show_strip(int y);
void do_strip(int sx);
void readstrip(void);

Mouse m;

char far stroka[255];

block seg_spr;
block tmp;
screen hidscr;
screen bckg;

void init_elem(void);
void init_seg(void);
int makeparent(int n, int nseg, int x, int y, int r,int spin);
int addchild(int n, int nseg, int r,int spin);
int addseg(int smx, int smy, int lx, int ly, block buf);
int spinchild(int n, int spin);
void show_elem(int n);
void splitcopy(int x, int y, int lx, int ly,screen dest, int sx, int sy, screen src);

void main(void) {
	int i,x,y,oldx=0,oldy=0;

	int s_0,s_1,s_2;
	int c_0=0,c_1,c_2,c_3,c_4,c_5;

	SetLib("circ");
	seg_spr=(block)famemalloc((unsigned long)20000);
	GetLib("segspr.bin",seg_spr);
	hidscr=famemalloc((unsigned long)320*200);
	bckg=famemalloc((unsigned long)320*200);
	GetLib("tmp.pic",(block)bckg);
	tmp=(block)famemalloc((unsigned long)1000);
	GetLib("01strip.def",(block)tmp);
	readstrip();

	InitGraph();
	//G_ly=G_lx=6;
	Vga256();
	Cls(0);
	GetLib("normal.col",palette);
	PutPalette(palette);

	init_seg();
	s_0=addseg(8,8,16,16,seg_spr);
	s_1=addseg(16,16,32,32,seg_spr+256);

	init_elem();
	makeparent(
		c_0,
		s_1,
		160,100,
		10,0);	/*r и угол*/

	c_1=addchild(c_0,s_0,7,0);
	c_2=addchild(c_1,s_0,7,0);
	c_3=addchild(c_2,s_0,7,0);
	c_4=addchild(c_3,s_0,7,0);
	c_5=addchild(c_4,s_1,7,0);



	//show_elem(0);
	SetScreen(hidscr);
	i=0;
	while ( !kbhit() ) {
		do_strip(10);
		show_strip(i);
		if ( ++i>striply ) {
			i=0;
		}

		spinchild(c_0,1);
		/*spinchild(c_1,1);
		spinchild(c_2,1);
		spinchild(c_3,1);
		//spinchild(c_4,1);
		//spinchild(c_5,1);
		//WVR();
		//delay(50);*/
		show_elem(0);
		CopyToScreen(hidscr);
	};
	CloseGraph();
	farfree(seg_spr);
	farfree(bckg);
	farfree(hidscr);
	farfree(tmp);
	SetLib("");

}

void fatalerror(char *t) {
	//выход по фатальной ошибке
	CloseGraph();
	SetLib("");
	puts(t);
	exit(1);
}

void init_elem(void) {
	int i;
	for ( i=0; i<maxelem; ++i ) {
		elem[i].nc=-1;
	}
}

void init_seg(void) {
	int i;
	for ( i=0; i<maxseg; ++i ) {
		seg[i].buf=NULL;
	}
}

int makeparent(int n, int nseg, int x, int y, int r=10,int spin=0) {
	elem[n].nc=-2;
	elem[n].x=x;
	elem[n].y=y;
	elem[n].nseg=nseg;
	elem[n].r=r;
	elem[n].spin=spin;
	return n;
}

int addchild(int n, int nseg, int r=10,int spin=0) {
	int i;
	for ( i=0; i<maxelem; ++i ) {
		if ( elem[i].nc==-1 ) {
			break;
		}
	}
	elem[i].nc=n;
	elem[i].nseg=nseg;
	elem[i].r=r;
	elem[i].spin=spin;
	return i;
}

int addseg(int smx, int smy, int lx, int ly, block buf) {
	/*добавление графического примитива*/
	int i;
	for ( i=0; i<maxseg; ++i ) {
		if ( seg[i].buf==NULL ) {
			break;
		}
	}
	seg[i].lx=lx;
	seg[i].ly=ly;
	seg[i].smx=smx;
	seg[i].smy=smy;
	seg[i].buf=buf;
	return i;
}

int spinchild(int n, int spin=1) {
	int i;
	elem[n].spin+=spin;
	if ( elem[n].spin<0 ) {
		elem[n].spin+=36;
	} else if ( elem[n].spin>35 ) {
		elem[n].spin-=36;
	}
	for ( i=0; i<maxelem; ++i ) {
		if ( elem[i].nc==n ) {
			spinchild(i,spin);
		}
	}
	return elem[n].spin;
}

void show_elem(int n) {
	int i;
	i=elem[n].nseg;
	PutCMas(elem[n].x-seg[i].smx,
		elem[n].y-seg[i].smy,
		seg[i].lx,seg[i].ly,seg[i].buf);

	for ( i=0; i<maxelem; ++i ) {
		if ( elem[i].nc==n ) {
			elem[i].x=elem[n].x+sin_[elem[i].spin]/elem[i].r;
			elem[i].y=elem[n].y+sin_[elem[i].spin+9]/elem[i].r;
			show_elem(i);
		}
	}
}











void readstrip(void) {
	//считывание описания для scroll-strip
	// 01strip.def
	int i,n;
	block pos=tmp;

	striply=nstrip=0;

	for (i=0; i<maxstrip; ++i ) {
		//считаем очередн строку
		if ( nstrip>maxstrip ) {
			fatalerror("strip.def-err");
		}
		memcpy(&stroka,pos,250);
		//0 0  27   -1 0   1
		n=sscanf(stroka,"%d %d %d %d %d %d",
		&strip[i].x,&strip[i].y,&strip[i].ly,
		&strip[i].sx,&strip[i].sy,&strip[i].typ);

		strip[i].st=0;

		if ( n<6 ) {
			//когда описание кончилось - выход
			break;
		}
		++nstrip;
		striply+=strip[i].ly;
		//идем на след строку
		while ( *pos>27 || *pos==9 ) ++pos; while ( *pos<27 ) ++pos;
	}
	striply-=170;
}

void splitcopy(int x, int y, int lx, int ly,screen dest, int sx, int sy, screen src) {
	//вывод блока со сдвигом
	int lx1,lx2;
	if ( sx+lx>=320 ) {
		lx1=320-sx;
	} else {
		lx1=lx;
	}

	lx2=lx-lx1;

	CopyBlock(x, y, lx1, ly, dest, sx, sy, src);
	if ( lx2 )
		CopyBlock(x+lx1, y, lx2, ly, dest, 0, sy, src);
}

void show_strip(int y) {
	static int i,j,py;

	j=py=0;
	for ( i=0; i<nstrip; ++i ) {
		py+=strip[i].ly;
		if ( py>y ) {
			j=py-y;
			break;
		}
	}
	/*1я строка*/
	splitcopy(16, 16, 272, min(160,strip[i].ly), CurrentScreen, strip[i].x, strip[i].y+strip[i].ly-j, bckg);
	if ( nstrip<=1) return;
	py=j;
	for ( ++i; i<nstrip; ++i ) {
		/*1я строка*/
		if ( py+(j=strip[i].ly)>168 ) {
			break;
		}
		splitcopy(16, 16+py, 272, j, CurrentScreen, strip[i].x, strip[i].y, bckg);
		py+=j;
	}
	/*последняя строка*/
	if ( (j=168-py)>0 ) {
		splitcopy(16, 16+py, 272, min(strip[i].ly,j), CurrentScreen, strip[i].x, strip[i].y, bckg);
	}
}

void do_strip(int sx=0) {
	int i;
	for ( i=0; i<nstrip; ++i ) {
		switch ( strip[i].typ ) {
			case st_still:
				break;
			case st_roll:
				strip[i].x-=strip[i].sx;
				if ( strip[i].x>319 ) {
					strip[i].x-=320;
				} else if ( strip[i].x<0 ) {
					strip[i].x+=320;
				}
				break;
			case st_go:

				if ( sx<0 ) {
					if ( (strip[i].st+=sx)<-strip[i].sx ) {
						strip[i].x+=(strip[i].st/strip[i].sx);
						strip[i].st=0;

					}
				} else if ( sx>0 ) {
					if ( (strip[i].st+=sx)>strip[i].sx ) {
						strip[i].x+=(strip[i].st/strip[i].sx);
						strip[i].st=0;

					}
				}

				/*if ( sx<0 ) {
					strip[i].x-=(strip[i].sx-sx);
				} else if ( sx>0 ) {
					strip[i].x+=(strip[i].sx+sx);
				}*/
				if ( strip[i].x>319 ) {
					strip[i].x-=320;
				} else if ( strip[i].x<0 ) {
					strip[i].x+=320;
				}
				break;
		}
	}
}