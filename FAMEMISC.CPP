/* ?????? ? ???? ०??? 256?? 320?200 */

#include	"famegraf.h"

#include	<conio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdarg.h>
#include	<malloc.h>

// PATCH: platform code
#ifndef __MSDOS__
#include "platform.h"
#endif

#ifdef EDITOR
#include	"mouse.h"
extern Mouse m;
#endif

int win_num=0; /*⥪?饥 ?????⮥ ????*/
struct {
	unsigned int x,y,lx,ly;
	block save;
} win[16];

int G_x=0, G_y=0; /*??? ?????? ???????*/
int G_lx=8, G_ly=8,G_interval=1;
char char_fgd=17, char_bkgd=0; /*梥? ᨬ???? ? 䮭?*/
block fonts;    /*????? ??? ????⮢*/
block palette;  /*????? ??? ⥪?饩*/
block palette1; /*????? ??? ⥪?饩 ???????*/

//char Mode_256;

/*=====================================================================*/

void InitGraph(void) {/*??砫쭠? ???樠??????? ?????? ? ??*/
	Clip(0,0,319,199);
	fonts=(block)famemalloc(2048+256);
	palette=(block)famemalloc(768);
	palette1=(block)famemalloc(768);
	return;
}

void CloseGraph(void) {
	if ( palette!=NULL )
		PaletteOff(palette);
	textmode(C80);
}

/*=== ???? ??? ?????প? Trident ? 4? ??࠭??

void SetSinchronReg(char New_v, char Allow, char Number) {
	int Value;
	asm cli;
	outportb(0x3c4,Number); Value=inportb(0x3c5);
	Value=( Value & ( ~ Allow ) ) | ( New_v & Allow );
	outportb(0x3c4,Number); outportb(0x3c5,Value);
	asm sti;
}

void SetEltControl(char New_v, char Allow, char Number) {
	int Value;
	asm cli;
	outportb(0x3d4,Number); Value=inportb(0x3d5);
	Value=( Value & ( ~ Allow ) ) | ( New_v & Allow );
	outportb(0x3d4,Number); outportb(0x3d5,Value);
	asm sti;
}

void SetGraphReg(char New_v, char Allow, char Number) {
	int Value;
	asm cli;
	outportb(0x3ce,Number); Value=inportb(0x3cf);
	Value=( Value & ( ~ Allow ) ) | ( New_v & Allow );
	outportb(0x3ce,Number); outportb(0x3cf,Value);
	asm sti;
}

void XOR_17(char Allow) {
	asm cli;
	outportb(0x3c2,(inportb(0x3cc) & 0x0df) | (( Allow & 1 )<<5) );
	asm sti;
}

void StartAddress(unsigned int Address) {
	SetEltControl(Address>>8,0xff,0x0c);
	SetEltControl(0x00ff&Address,0xff,0x0d);
}

void MapMask(char Mask) {
	SetSinchronReg(Mask, 0x0f, 2);
}

void SelectPlane(char Number) {
	SetGraphReg(Number, 3, 4);
}

void MemoryMap(char Map) {
	SetGraphReg(Map<<2, 0x0c, 6);
}

void Chain_4(char Allow) {
	SetSinchronReg(Allow<<3, 0x8, 4); //08
}

void DubleWordMode(char Allow) {
	SetEltControl(Allow<<6, 0x40, 0x14);
}

void ByteOrWordMode(char Allow) {
	SetEltControl(Allow<<6, 0x40, 0x17);
}

void AddressConvolution(char Allow) {
	SetEltControl(Allow<<5, 0x20, 0x17);
}

void SetDMode(char Mode_Number) {
	WVR();
	MapMask(0x0f);
	MemoryMap(0);
	asm cli;
	if (Mode_Number==Mode_D0) {
			Chain_4(0);
	} else {
			Chain_4(1);
	}
	asm sti;
}

void SetSMode(char Mode_Number) {
	WVR();
	asm cli;
	if (Mode_Number==Mode_S0) {
		ByteOrWordMode(1);
		DubleWordMode(0);
	} else {
		ByteOrWordMode(0);
		DubleWordMode(1);
	}
	asm sti;
}

void Set_Mode_256(char Mode_Number) {
	if (Mode_Number==Mode_R0) {
		SetSMode(Mode_S0);
		SetDMode(Mode_D0);
	} else {
		SetSMode(Mode_S4);
		SetDMode(Mode_D4);
	}
	Mode_256=Mode_Number;
}

void SetActivePage(char Page) {
	if (Mode_256==Mode_R0) {
		asm mov	[WORD PTR HIGH CurrentScreen],0A000h;
		asm mov	[WORD PTR LOW CurrentScreen],0;
	} else {
		XOR_17(~((Page>>1) & 1));
		if (Page&1) {
			asm mov	[WORD PTR HIGH CurrentScreen],0B000h;
			asm mov	[WORD PTR LOW CurrentScreen],0;
		} else {
			asm mov	[WORD PTR HIGH CurrentScreen],0A000h;
			asm mov	[WORD PTR LOW CurrentScreen],0;
		}
	}
}

void SetVisualPage(char Page) {
	register int Value;
	WVR();
	//StartAddress((unsigned int)Page* 0x4000);
	asm cli;
	outportb(0x3d4,0x0c); Value=inportb(0x3d5);
	Value=( Value & ( ~ 0xff ) ) | ( Page*0x40 );
	outportb(0x3d4,0x0c); outportb(0x3d5,Value);
	asm sti;
}

=======================================*/

void MoveXY(int x, int y) {	//??६?????? ⥪?⮢ ?????? ?? ??࠭? 320?200*/
	G_x=(int)x; G_y=(int)y;
}

void PutCh(unsigned char c_) { //?뢮? ᨬ???? ?? ??࠭
	unsigned int t=c_;
	if(G_y>(MaxY-G_ly)) G_y=(MaxY-G_ly);
	if(G_x>(MaxX-G_lx)) G_x=(MaxX-G_lx);

	if (c_==10 || c_==13 ) {
		goto PutCh_;
	}
	DisplayChar (c_, G_x, G_y, char_fgd, char_bkgd);
	//G_x+=(fonts[2048+t]+G_interval);
	G_x+=(GetCharWidth(t)+G_interval);

	if(G_x>(MaxX-G_lx)) {
PutCh_:
		G_x=MinX; G_y+=G_ly;
		if(G_y>(MaxY-G_ly)) G_y=(MaxY-G_ly);
	}
	return;
}

unsigned int famestrlen(unsigned char *s) {//????? ??⮢?? ??ப? ? ???ᥫ??
	unsigned int l=0,t;
	while ((t=(*s++))!=0) {
		//l+=(fonts[2048+t]+G_interval);
		l+=(GetCharWidth(t)+G_interval);
	}
	return l;
}

//=======================================
void putBch(int x, int y, unsigned char c, block buf) {
	static int nx,ny;
	static unsigned char t;
	static unsigned char m[]={128,64,32,16,8,4,2,1};
	static int f;

	f=c*8;
	for (ny=0; ny<8; ++ny ) {
		//᫥? ??ப? ??????
		if ( fonts[f]&m[ny] )
			PutMas(x+ny*3,y-3,3,3,buf+9*4);
		if ( fonts[f+ny]&m[0] )
			PutMas(x-3,y+ny*3,3,3,buf+9*8);
		if ( fonts[f+7]&m[ny] )
			PutMas(x+ny*3,y+8*3,3,3,buf+9);

		for (nx=0; nx<8; ++nx ) {
			t=0;
			if ( (fonts[f+ny]&m[nx])==0 ) {

				if( ny && fonts[f+ny-1]&m[nx] ) {
					t=1; //????
				}
				if( nx && fonts[f+ny]&m[nx-1] ) {
					t=t|2; //????
				}
				if( ny<7 && fonts[f+ny+1]&m[nx] ) {
					t=t|4; //???
				}
				if( nx<7 && fonts[f+ny]&m[nx+1] ) {
					t=t|8; //?ࠢ?
				}
				if ( t )
					PutMas(x+nx*3,y+ny*3,3,3,buf+(int)(9*(int)t));
			} else {
				PutMas(x+nx*3,y+ny*3,3,3,buf);
			}
		}
	}
}

void vputBs(char * f,block buf) {
	//?뢮? ??ப? ????訬? ?㪢???
	while (*f) {
		putBch(G_x,G_y,*f,buf);
		//G_x+=(fonts[2048+(unsigned char)*f]+G_interval)*3;
		G_x+=(GetCharWidth((unsigned char)*f)+G_interval)*3;
		++f;
	}
	return;
}

int vprintB( block buf, char *fmt, ... ) {
	//?뢮? ??ப? ??ଠ?? ????訬? ?㪢???
	va_list  argptr;
	char str[25];
	int cnt;
	va_start( argptr, fmt );
	cnt = vsprintf( str, fmt, argptr );
	vputBs( str , buf);
	va_end( argptr );
	return( cnt );
}

//=======================================
void PutPalette(block f) {
	static int i,k;
	for ( i=k=0; k<256*3; k+=3,++i ) {
		if ( (i&63)==0 ) WVR();
		PutRGB(i,f[k],(f+1)[k],(f+2)[k]);
	}
}

#define fadestep 8

void PaletteOn(block f) { /*????*/
	static int j,i;
	for (j=64;j>=0;j-=fadestep) {
		for (i=0;i<256*3;++i) {
			palette1[i]=(f[i]-j<=0)?0:(f[i]-j);
		}
		PutPalette(palette1);
	}
	return;
}

void PaletteOff(block f) { /*?????*/
	static int j,i;
	for (j=0;j<=64;j+=fadestep) {
		for (i=0;i<256*3;++i) {
			palette1[i]=(f[i]-j<=0)?0:(f[i]-j);
		}
		PutPalette(palette1);
	}
}

void Rectangle(int x1, int y1, int lx, int ly) {
	//?뢮? ࠬ??
	Line(x1,y1,x1,y1+ly-1); Line(x1,y1,x1+lx-1,y1);
	Line(x1+lx-1,y1,x1+lx-1,y1+ly-1); Line(x1,y1+ly-1,x1+lx-1,y1+ly-1);
}

void Swap (int *pa,int *pb) {
	register int t;
	t=*pa; *pa=*pb; *pb=t;
}

void Line(int x1, int y1, int x2, int y2)
{
	static int d,y,x,dx,dy,Aincr,Bincr,yincr;

	if ( abs(y2-y1)>abs(x2-x1) ) goto Do_dr_lin;

	if (x1>x2) {
		Swap (&x1,&x2);
		Swap (&y1,&y2);
	}

	if (y2>y1)
		yincr=1;
	else
		yincr=-1;

	dx=x2-x1; dy=abs(y2-y1); d=2*dy-dx;
	Aincr=2*(dy-dx);
	Bincr=2*dy;
	x=x1;
	y=y1;

	for (x=x1/*+1*/;x<=x2;x++) {
		if (d>=0) {
			y+=yincr;
			d+=Aincr;
		}
		else
			d+=Bincr;
			PutPixel(x,y);
	}
	return;

Do_dr_lin:
	if (y1>y2) {
		Swap (&x1,&x2);
		Swap (&y1,&y2);
	}

	if (x2>x1)
		yincr=1;
	else
		yincr=-1;

	dy=y2-y1; dx=abs(x2-x1); d=2*dx-dy;
	Aincr=2*(dx-dy);
	Bincr=2*dx;
	x=x1;
	y=y1;

	for (y=y1/*+1*/;y<=y2;y++) {
		if (d>=0) {
			x+=yincr;
			d+=Aincr;
		}
		else
			d+=Bincr;
			PutPixel(x,y);
	}
}

void vputs(char * f) { /*?뢮? ⥪?⮢?? ??ப? ?? ??࠭*/
	while (*f) {
		PutCh(*f);
		++f;
	}
}

int vprint( char *fmt, ... ) {
	va_list  argptr;
	char str[64];
	int cnt;
	va_start( argptr, fmt );
	cnt = vsprintf( str, fmt, argptr );
	vputs( str );
	va_end( argptr );
	return( cnt );
}

void vputsc(char * f) { /*?뢮? ⥪?⮢?? ??ப? ?? ??࠭ ?? 業???*/
	MoveXY(159-famestrlen((unsigned char *)f)/2,G_y);
	vputs(f);
}

void PutMtb(int x,int y,int lx,int ly,int mx,int my,block buffer) {
	static int i,j;
	static int tx,ty;
	static int ny;
	for (j=0;j<ly;++j) {
		ny=y+(ty=(j*my/ly)); ty=((j+1)*my/ly-ty);
		ty=(ty ? ty : 1);
		for (i=0;i<lx;++i) {
			tx=( i*mx/lx );
			CurrentColor=( *(buffer+j*lx+i) );
			Bar( x+tx, ny, ((i+1)*mx/lx)-tx, ty );
		}
	}
	return;
}

void PutCMas(int x, int y, int lx, int ly, block buf) {
	//?뢮? ? ?஧?????? 梥⮬
	static int x1,y1,nx,ny,nlx,nly,rx,ry;
	x1=x+lx-1;y1=y+ly-1;

	if ( x1<MinX || y1<MinY || x>MaxX || y>MaxY ) {
		//?? ???㥬
		return;
	}
	if ( x1<=MaxX && y1<=MaxY && x>=MinX && y>=MinY ) {
		//???㥬 ?? ??१??
		PutMas(x,y,lx,ly,buf);
		return;
	}
	//??१?? ?? ??࠭?
	// X ???न????
	nlx=lx;	nx=x; rx=0;
	if (x<MinX) {
		nx=MinX;
		rx=(MinX-x);
		nlx=lx-rx;
	}
	if (x1>MaxX) {
		nlx-=(x1-MaxX);
	}

	// Y ???न????
	nly=ly; ny=y; ry=0;
	if (y<MinY) {
		ny=MinY;
		ry=(MinY-y);
		nly=ly-ry;
	}
	if (y1>MaxY) {
		nly-=(y1-MaxY);
	}

	PutSMas(nx,ny,lx,ly,rx,ry,nlx,nly,buf);
}

void PutCMasr(int x, int y, int lx, int ly, block buf) {
	//?뢮? ? ?஧?????? 梥⮬
	static int x1,y1,nx,ny,nlx,nly,rx,ry;
	x1=x+lx-1;y1=y+ly-1;

	if ( x1<MinX || y1<MinY || x>MaxX || y>MaxY ) {
		//?? ???㥬
		return;
	}
	if ( x1<=MaxX && y1<=MaxY && x>=MinX && y>=MinY ) {
		//???㥬 ?? ??१??
		PutMasr(x,y,lx,ly,buf);
		return;
	}

	//??१?? ?? ??࠭?
	// X ???न????
	nlx=lx;	nx=x; ry=rx=0;
	if (x<MinX) {
		nx=MinX-1;
		nlx-=(MinX-x);
	}
	if (x1>MaxX) {
		rx=(x1-MaxX);
		nlx-=(x1-MaxX);
	}

	// Y ???न????
	nly=ly; ny=y;
	if (y<MinY) {
		ny=MinY;
		ry=(MinY-y);
		nly=ly-ry;
	}
	if (y1>MaxY) {
		nly-=(y1-MaxY);
	}

	PutSMasr(nx,ny,lx,ly,rx,ry,nlx,nly,buf);
}

void PutCBlink(int x, int y, int lx, int ly, block buf) {
	//?뢮? ? ?஧?????? 梥⮬
	static int x1,y1,nx,ny,nlx,nly,rx,ry;
	x1=x+lx-1;y1=y+ly-1;

	if ( x1<MinX || y1<MinY || x>MaxX || y>MaxY ) {
		//?? ???㥬
		return;
	}
	if ( x1<=MaxX && y1<=MaxY && x>=MinX && y>=MinY ) {
		//???㥬 ?? ??१??
		PutBlink(x,y,lx,ly,buf);
		return;
	}
	//??१?? ?? ??࠭?
	// X ???न????
	nlx=lx;	nx=x; ry=rx=0;
	if (x<MinX) {
		nx=MinX;
		rx=(MinX-x);
		nlx=lx-rx;
	}
	if (x1>MaxX) {
		nlx-=(x1-MaxX);
	}

	// Y ???न????
	nly=ly; ny=y;
	if (y<MinY) {
		ny=MinY;
		ry=(MinY-y);
		nly=ly-ry;
	}
	if (y1>MaxY) {
		nly-=(y1-MaxY);
	}

	PutSBlink(nx,ny,lx,ly,rx,ry,nlx,nly,buf);
}

void PutCBlinkr(int x, int y, int lx, int ly, block buf) {
	//?뢮? ? ?஧?????? 梥⮬
	static int x1,y1,nx,ny,nlx,nly,rx,ry;
	x1=x+lx-1;y1=y+ly-1;

	if ( x1<MinX || y1<MinY || x>MaxX || y>MaxY ) {
		//?? ???㥬
		return;
	}
	if ( x1<=MaxX && y1<=MaxY && x>=MinX && y>=MinY ) {
		//???㥬 ?? ??१??
		PutBlinkr(x,y,lx,ly,buf);
		return;
	}

	//??१?? ?? ??࠭?
	// X ???न????
 	nlx=lx;	nx=x; ry=rx=0;
	if (x<MinX) {
		nx=MinX-1;
		nlx-=(MinX-x);
	}
	if (x1>MaxX) {
		rx=(x1-MaxX);
		nlx-=(x1-MaxX);
	}

	// Y ???न????
	nly=ly; ny=y;
	if (y<MinY) {
		ny=MinY;
		ry=(MinY-y);
		nly=ly-ry;
	}
	if (y1>MaxY) {
		nly-=(y1-MaxY);
	}

	PutSBlinkr(nx,ny,lx,ly,rx,ry,nlx,nly,buf);
}

void PutCImg(int x, int y, int lx, int ly, block buf) {
	//?뢮? ??? ?஧??筮???
	static int x1,y1,nx,ny,nlx,nly,rx,ry;
	x1=x+lx-1;y1=y+ly-1;

	if ( x1<MinX || y1<MinY || x>MaxX || y>MaxY ) {
		//?? ???㥬
		return;
	}
	if ( x1<=MaxX && y1<=MaxY && x>=MinX && y>=MinY ) {
		//???㥬 ?? ??१??
		PutImg(x,y,lx,ly,buf);
		return;
	}
	//??१?? ?? ??࠭?
	// X ???न????
	nlx=lx;	nx=x; ry=rx=0;
	if (x<MinX) {
		nx=MinX;
		rx=(MinX-x);
		nlx=lx-rx;
	}
	if (x1>MaxX) {
		nlx-=(x1-MaxX);
	}

	// Y ???न????
	nly=ly; ny=y;
	if (y<MinY) {
		ny=MinY;
		ry=(MinY-y);
		nly=ly-ry;
	}
	if (y1>MaxY) {
		nly-=(y1-MaxY);
	}

	PutSImg(nx,ny,lx,ly,rx,ry,nlx,nly,buf);
}


void CBar(int x, int y, int lx, int ly) {
	//?뢮? ?????????????? ? ??१???
	static int x1,y1,nx,ny,nlx,nly,rx,ry;
	x1=x+lx-1; y1=y+ly-1;

	if ( x1<MinX || y1<MinY || x>MaxX || y>MaxY ) {
		//?? ???㥬
		return;
	}
	if ( x1<=MaxX && y1<=MaxY && x>=MinX && y>=MinY ) {
		//???㥬 ?? ??१??
		Bar(x,y,lx,ly);
		return;
	}
	//??१?? ?? ??࠭?
	// X ???न????
	nlx=lx;	nx=x; ry=rx=0;
	if (x<MinX) {
		nx=MinX;
		rx=(MinX-x);
		nlx=lx-rx;
	}
	if (x1>MaxX) {
		nlx-=(x1-MaxX);
	}

	// Y ???न????
	nly=ly; ny=y;
	if (y<MinY) {
		ny=MinY;
		ry=(MinY-y);
		nly=ly-ry;
	}
	if (y1>MaxY) {
		nly-=(y1-MaxY);
	}

	Bar(nx,ny,nlx,nly);
}

void TileBar(int x, int y, int lx, int ly, int tx, int ty, block buf) {
	//?뢮? ?????????????? ????????
	int _MaxX=MaxX,_MaxY=MaxY,_MinX=MinX,_MinY=MinY,i,j;
	Clip(x,y,x+lx-1,y+ly-1);
	for ( i=y; i<=y+ly; i+=ty )
		for ( j=x; j<=x+lx; j+=tx ) {
			PutCImg(j,i,tx,ty,buf);
		}
	Clip((MinX=_MinX),(MinY=_MinY),(MaxX=_MaxX),(MaxY=_MaxY));
}

/*
void CopyCBlock(int x, int y, int lx, int ly, screen dest, int xs, int ys, screen src) {
	//????஢???? ????? ? ??㣮?? ??࠭?
	//? ??१???
	static int x1,y1,nx,ny,nlx,nly,rx,ry;
	x1=x+lx-1; y1=y+ly-1;

	if ( x1<MinX || y1<MinY || x>MaxX || y>MaxY ) {
		//?? ???㥬
		return;
	}
	if ( x1<=MaxX && y1<=MaxY && x>=MinX && y>=MinY ) {
		//???㥬 ?? ??१??
		CopyBlock(x,y,lx,ly,dest,xs,ys,src);
		return;
	}
	//??१?? ?? ??࠭?
	// X ???न????
	nlx=lx;	nx=x; ry=rx=0;
	if (x<MinX) {
		nx=MinX;
		rx=(MinX-x);
		nlx=lx-rx;
	}
	if (x1>MaxX) {
		nlx-=(x1-MaxX);
	}

	// Y ???न????
	nly=ly; ny=y;
	if (y<MinY) {
		ny=MinY;
		ry=(MinY-y);
		nly=ly-ry;
	}
	if (y1>MaxY) {
		nly-=(y1-MaxY);
	}

	CopyBlock(nx,ny,nlx,nly,dest,xs+rx,ys+ry,src);
}*/

char CharUp(unsigned char i) {
	if (i>=224 && i<=239) {
		i-=48;	//᪫???? ???᪨? ?㪢
	}
	if ( (i>=96 && i<=127) || (i>=160 && i<=191) ) {
		return (i-32);
	}
	return i;
}

int GetString(char *str,int lstr) {
  int /*i,j,*/lpos,nkey;
  int xs=G_x, ys=G_y; /*??? ?????? ???????*/
  char oldbk=char_bkgd;

  char_bkgd=30;
  str[lstr]=0; lpos=strlen(str);

  while (1) {
	MoveXY(xs,ys);
	vputs(str);
	CurrentColor=0;
	Bar(G_x,G_y,G_lx,G_ly);

	while (kbhit()==0);

	switch((nkey=getch())) {

	case 13: /*??室 ?? ENTER*/
		char_bkgd=oldbk;
		return 1;

	case 8: /*?????*/
Do_Zaboj:
		if (lpos>0) --lpos;
		str[lpos]=0;
		break;

	case 27:
		char_bkgd=oldbk;
		return -1; /*??室 ?? ESC*/

	case 0:
		switch(getch()) {

			case 75: /*left*/
			goto Do_Zaboj;
		}

	default:
		if (nkey>31 && lpos<lstr) {
			str[lpos++]=nkey;
			str[lpos]=0;
		}
		break;
	}
  }
}

void WPut(int x, int y, int lx, int ly) {
//??ᮢ???? ???㪫??? ???? ?? ᢥ⮢?? ?????? ? ⥭????
#ifdef EDITOR
	m.off();
#endif
	CurrentColor=wfon;
	Bar(x,y,lx,ly);
	CurrentColor=wlight;
	Bar(x,y,lx,1);
	Bar(x,y,1,ly);
	CurrentColor++;
	Bar(x,y,1,1);
	CurrentColor=wshadow;
	Bar(x,y+ly-1,lx,1);
	Bar(x+lx-1,y+1,1,ly-1);
	CurrentColor++;
	Bar(x+lx-1,y+ly-1,1,1);
#ifdef EDITOR
	m.on();
#endif
}

void WPut1(int x, int y, int lx, int ly) {
//??ᮢ???? ??????????? ???? ?? ᢥ⮢?? ?????? ? ⥭????
#ifdef EDITOR
	m.off();
#endif
	CurrentColor=wfon+1;
	Bar(x,y,lx,ly);
	CurrentColor=wshadow-1;
	Bar(x+1,y+1,lx-1,1);
	Bar(x+1,y+1,1,ly-1);
	CurrentColor=0;
	Bar(x,y,lx,1);
	Bar(x,y,1,ly);
#ifdef EDITOR
	m.on();
#endif
}

void _WOpen(int x, int y, int lx, int ly) {
//?????⨥ ???? ? ???????????? 䮭? ??? ???
#ifdef EDITOR
	m.off();
#endif
	win[win_num].x=x;
	win[win_num].y=y;
	win[win_num].lx=lx;
	win[win_num].ly=ly;
	win[win_num].save=(block)famemalloc(lx*ly);
	GetImg(x,y,lx,ly,win[win_num].save);
	win_num++;
#ifdef EDITOR
	m.on();
#endif
}

void WOpen(int x, int y, int lx, int ly) {
//?????⨥ ???? ? ???????????? 䮭? ??? ???
#ifdef EDITOR
	m.off();
#endif
	win[win_num].x=x;
	win[win_num].y=y;
	win[win_num].lx=lx;
	win[win_num].ly=ly;
	win[win_num].save=(block)famemalloc(lx*ly);
	GetImg(x,y,lx,ly,win[win_num].save);
	WPut(x,y,lx,ly);
	win_num++;
#ifdef EDITOR
	m.on();
#endif
}

void WClose() {
//?????⨥ ??᫥????? ???? ? ????⠭???????? 䮭?
#ifdef EDITOR
	m.off();
#endif
	--win_num;
	PutImg(win[win_num].x,win[win_num].y,win[win_num].lx,win[win_num].ly,win[win_num].save);
	farfree(win[win_num].save);
#ifdef EDITOR
	m.on();
#endif
}

/*
void ReadFont(char *font_name) {
	int fdz;
	//????㧪? ????⮢
	if ( (fdz=open (font_name,O_BINARY | O_RDONLY)) < 0) exit(1);
	if ((int)read (fdz, fonts, 2048) == -1 ) exit (1);
	close(fdz);
	PutFont(fonts);
}

void ReadPalette(char *palette_name,block dest) {
	int fdz;
	//????㧪? ???????
	if ( (fdz=open (palette_name,O_BINARY | O_RDONLY)) < 0) exit (1);
	if ((int)read (fdz,dest, 768) == -1) exit (1);
	close (fdz);
}

block ReadSpr(char *data_name,unsigned long n) {
	int fdz;
	block dest;
	dest=(block)famemalloc(n);
	if ( (fdz=open (data_name,O_BINARY | O_RDONLY)) < 0) exit (1);
	if ( read (fdz, dest, n) == -1 ) exit (1);
	close (fdz);
	return (dest);
}

void ReadData(char *data_name,block dest,unsigned long n) {
	int fdz;
	if ( (fdz=open (data_name,O_BINARY | O_RDONLY)) < 0) exit (1);
	if ((int)read (fdz, dest, n) == -1 ) exit (1);
	close (fdz);
}
*/
