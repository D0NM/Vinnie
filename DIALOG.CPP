#include <stdlib.h>
#include <conio.h>
#include <bios.h>
#include <dos.h>
#include <ctype.h>

#include "Mouse.h"
#include "dialog.h"
#include "famegraf.h"

extern Mouse m;

#define clic 100

void Panel::init(void)
{
	int i;
	num=0;

	for ( i=0; i<MaxPanel; ++i ) {
		pan[i].typ=pan[i].status=0;
	}
}

void Panel::show(void)
{ //?뢮? ?? ??࠭ ???? ??????
	int i;
	for ( i=0; i<MaxPanel; ++i ) {
		show(i);
	}
}

void Panel::show(int i)
{ //?뢮? ?? ??࠭ ????? ?????? (?????. ??? ?? ?뢮?????)
	char_fgd=16; char_bkgd=0; //梥? ᨬ???? ? 䮭?
	if ( i<MaxPanel && i>=0 && pan[i].typ==1 ) {//?᫨ ???? ??? ??????
		m.off();
		if ( pan[i].status==1 ) {
			//??????
			WPut1(pan[i].x-1, pan[i].y-1, pan[i].lx+2, pan[i].ly+2);
			if ( pan[i].ico ) {
				PutMas(pan[i].x+1, pan[i].y+1, pan[i].lx, pan[i].ly,pan[i].pic);
			} else {
				MoveXY(pan[i].x+2, pan[i].y+2);
				vputs(pan[i].txt);
			}
		} else {
			//?? ??????
			WPut(pan[i].x-1, pan[i].y-1, pan[i].lx+2, pan[i].ly+2);
			if ( pan[i].ico ) {
				PutMas(pan[i].x, pan[i].y, pan[i].lx, pan[i].ly,pan[i].pic);
			} else {
				MoveXY(pan[i].x+1, pan[i].y+1);
				vputs(pan[i].txt);
			}
		}
		m.on();
	}
}

int Panel::addr(int x, int y, int lx,int ly, char *txt, void (*prog)(void), ...) {
	//?????????? ࠡ?祩 ??????⭮? ???????
	int i=-1;
	for ( i=0; i<MaxPanel; ++i ) {
		if (pan[i].typ==0) {
			++num;

			pan[i].x=x;
			pan[i].y=y;
			pan[i].lx=lx;
			pan[i].ly=ly;
			pan[i].txt=txt;
			pan[i].prog=prog;

			pan[i].typ=2; //㦥 ???? ??????⭠? ???????
			pan[i].status=0;
			//------------------------
			show(i);
			break;
		}
	}
	return i;
}

int Panel::add(int x=0, int y=0, int lx=16,int ly=8, char *txt ,block pic, void (*prog)(void), ...) {
	//?????????? ?????? ? ??????? ??? ⥪?⮬
	int i=-1;
	for ( i=0; i<MaxPanel; ++i ) {
		if (pan[i].typ==0) {
			++num;

			pan[i].x=x;
			pan[i].y=y;
			pan[i].txt=(txt==NULL?"":txt);
			pan[i].lx=max(lx,famestrlen((unsigned char *)pan[i].txt)+2);
			pan[i].ly=ly;
			pan[i].pic=pic;
			pan[i].prog=prog;

			pan[i].ico=(pic==NULL?0:1);

			pan[i].typ=1; //㦥 ???? ??????
			pan[i].status=0;
			//------------------------
			show(i);
			break;
		}
	}
	return i;
}

void Panel::del(int i)
{
	if (i>=0 && pan[i].typ!=0) {
		--num;
		pan[i].typ=0;
		m.off();
		WPut1(pan[i].x-1, pan[i].y-1, pan[i].lx+2, pan[i].ly+2);
		m.on();
	}
}

int Panel::press(void)
{
	int i,j=-1;
	char key=0;
	m.status();

	if ( m.left.press || m.right.press || m.center.press || kbhit()) {
		if ( kbhit() ) {
			key=/*getch();*/bioskey(0);
		}
		for ( i=0; i<MaxPanel; ++i ) {
			if (pan[i].typ!=0) {
				if ( key!=0 && pan[i].txt!=NULL && ( CharUp(pan[i].txt[0])==CharUp(key) ) ) {
					//???? ????᪠?? ?ணࠬ??!
					pan[i].status=1;
					show(i);
					delay(clic);
					pan[i].status=0;
					if (pan[i].prog!=NULL) {
						pan[i].prog();
					}
					show(i);
					return last=i;
				}
				if ( key==0 && m.x>=pan[i].x && m.y>=pan[i].y && m.x<pan[i].x+pan[i].lx && m.y<pan[i].y+pan[i].ly )	{
					//???? ?? ??????!
					if ( pan[i].status==0 ) {
						pan[i].status=1;
						show(i);
						delay(clic);
						current=i;
					}
					//j=i; //?????? ?? ?????? ? ????஬
				} else {
					//??誠 ?? ?⮨?
					if ( pan[i].status ) {
						pan[i].status=0;
						show(i);
					}
				}


			}
		}
	} else {
		for ( i=0; i<MaxPanel; ++i ) {
			if (pan[i].typ!=0 && m.x>=pan[i].x && m.y>=pan[i].y && m.x<pan[i].x+pan[i].lx && m.y<pan[i].y+pan[i].ly && pan[i].status)	{
				//???? ????᪠?? ?ணࠬ??!
				pan[i].status=0;
				if (pan[i].prog!=NULL) {
					pan[i].prog();
				}
				show();
				return last=i;
			}
		}
	}

	return last=j;
}
