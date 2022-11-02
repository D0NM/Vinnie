#ifndef H_FAMEGRAPH
#define H_FAMEGRAPH

#include <stdio.h>

#ifndef __MSDOS__
#include "platform.h"
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

typedef /*unsigned*/ char far * block;
typedef void far * screen;

#ifdef __cplusplus
extern "C" {
#endif

extern void far moddevice( int *device );
extern void far modvolume( int vol1, int vol2,int vol3,int vol4);
extern void far modsetup( char *filenm, int looping, int prot,int mixspeed,
									 int device, int *status);
extern void far modstop(void);
extern void far modinit(void);

extern void far CopyBlock(int,int,int,int,screen dest,int,int,screen src);

#ifdef __MSDOS__
#ifdef __cplusplus
}
#endif
#endif

#define k_up	18432
#define k_right	19712
#define k_left	19200
#define k_down	20480
#define k_esc	283
#define k_enter	7181
#define k_f10	17408
#define	k_c	11875
#define k_f	8550
#define k_l	9836
#define k_s	8051
#define k_d	8292
#define k_v	12150

#define wfon (int)134 /*фон окна*/
#define wlight (int)128 /*блик окна*/
#define wshadow (int)146 /*тень окна*/



// Графические переменные
extern int far	MaxX;
extern int far	MaxY;
extern int far	MinX;
extern int far	MinY;
extern screen	CurrentScreen;
extern block	CurrentAdr;
extern block	CurrentAdr1;
extern screen	SpritesScreen;
extern screen	VScreen;
extern unsigned char far CurrentColor;
extern block fonts;
extern int G_x, G_y;
extern int G_lx, G_ly, G_interval;
extern char char_fgd, char_bkgd;
extern block palette;
extern block palette1;

// Графические функции
void far Vga256(void);
void far SetScreen(screen);
void far NormalScreen(void);
void far ScreenCopy(screen dest,screen src);
void far CopyToScreen(screen src);
void far Cls(char);
void far WVR(void);
void far SetColor(char);
void far PutRGB(char,char,char,char);
void far PutPalette(block);
void far GetPalette(block);
void far PutFont(block);
int  far GetCharWidth(int);
void far DisplayChar(int c, int x, int y, char fgd, char bkgd);
void far Clip(int, int, int, int);
void far PutPixel(int, int);
int  far GetPixel(int, int);
void far Bar(int, int, int, int);
void far Rectangle(int, int, int, int);
void far Line(int, int, int, int);
void far PutImg(int, int, int, int, block);
void far GetImg(int, int, int, int, block);
void far PutMas(int, int, int, int, block);
void far PutMasr(int, int, int, int, block);
void far PutBlink(int, int, int, int, block);
void far PutBlinkr(int, int, int, int, block);
void far PutImg16(int, int, block);
void far PutMas16(int, int, block);
void far CalcAdr(int, int);
void far SPutImg16();
void far SPutMas16();
void far PutCImg(int, int, int, int, block);
void far PutCMas(int, int, int, int, block);
void far PutCMasr(int, int, int, int, block);
void far PutCBlink(int, int, int, int, block);
void far PutCBlinkr(int, int, int, int, block);
void far PutSMas(int, int, int, int, int, int, int, int, block);
void far PutSMasr(int, int, int, int, int, int, int, int, block);
void far PutSImg(int, int, int, int, int, int, int, int, block);
void far PutSBlink(int, int, int, int, int, int, int, int, block);
void far PutSBlinkr(int, int, int, int, int, int, int, int, block);
//void far CopyBlock(int,int,int,int,screen dest,int,int,screen src);
void far CopyBlock0(screen src);
long far gettic(void);
void CBar(int, int, int, int);
void TileBar(int, int, int, int, int,int,block);
void CopyCBlock(int, int, int, int, screen dest, int, int, screen src);

// Другие Графические функции
void InitGraph(void);
void CloseGraph(void);
void PaletteOn(block);
void PaletteOff(block);
void MoveXY(int, int);
void PutCh(unsigned char);
unsigned int famestrlen(unsigned char *);
void vputs(char * f);
void vputsc(char * f);
int  vprint( char *fmt, ... );
char CharUp(unsigned char);
//вывод большого размера
void putBch(int x, int y, unsigned char c, block buf);
void vputBs(char * f,block buf);
int vprintB( block buf, char *fmt, ... );
void PutMtb(int x,int y,int lx,int ly,int mx,int my,block buffer);
int  GetString(char *str,int lstr);
void WPut(int x, int y, int lx, int ly);
void WPut1(int x, int y, int lx, int ly);
void _WOpen(int x, int y, int lx, int ly); //без отрисовки
void WOpen(int x, int y, int lx, int ly);
void WClose();

void Swap (int *,int *); //обменять местами переменные

//функции для Trident и поддержки нескольк страниц!
#define Mode_D0 0
#define Mode_D4 2
#define Mode_S0 0
#define Mode_S2 1
#define Mode_S4 2
#define Mode_R0 0
#define Mode_R4 2
extern char Mode_256;

void SetSinchronReg(char New_v, char Allow, char Number);
void SetEltControl(char New_v, char Allow, char Number);
void SetGraphReg(char New_v, char Allow, char Number);
void XOR_17(char Allow);
void StartAddress(unsigned int Address);

void MapMask(char Mask);
void SelectPlane(char Number);
void MemoryMap(char Map);
void Chain_4(char Allow);
void DubleWordMode(char Allow);
void ByteOrWordMode(char Allow);
void AddressConvolution(char Allow);
void SetDMode(char Mode_Number);
void SetSMode(char Mode_Number);
void Set_Mode_256(char Mode_Number);
void SetActivePage(char Page);
void SetVisualPage(char Page);

void ReadFont(char *font_name);
void ReadPalette(char *palette_name,block dest);
block ReadSpr(char *data_name,unsigned long n);
void ReadData(char *data_name,block dest, unsigned long n);

// Спрайты (c) BMV

#pragma pack(push, 1)
struct sprites {
  short x,y;          //текущие координаты
  short ox,oy;        //старые координаты
  short lx,ly;        //ширина и высота тек. спр.
  block map;        //ссылка на граф.образ
  unsigned char on; //1-вкл, 0-выкл
};
#pragma pack(pop)

void InitSpr(void); //начальная инициализация
int CreateSpr(int lx,int ly, block map); //создать спрайт
void ClearSpr(int num); //удалитьть спрайт
void SprOn(int); //показывать спрайт
void SprOff(int); //прятать спрайт
void MoveSpr(int num, int x, int y); //движение спрайта
void SetSpr(int num, block map); //движение спрайта

void ShowSprites(void); //вывести все активные спрайты на экран
void EraseSprites(void); //восстановить прежний фон под спрайтами

// функции для работы с Библиотекой
int SetLib(char *);
int PutLib(char *, block, unsigned long);
int GetLib(char *, block);
int GetLibs(char *, block);	//тоже но не грузит, если было это имя
unsigned long SizeLib(char *);
int DelLib(char *);
int SaveMap(void);
int PackLib(void);
unsigned long PackBlock(block src, unsigned long lenblk);
unsigned long UnpackBlock(block src, unsigned long lenpak, unsigned long lenblk);
extern block pakdest;
void far *famemalloc(unsigned long); //типа farmalloc но с диагностикой
void _fmeminv(block s, unsigned long n); //инвертирует кусок памяти
void fatalerror(char *t);

#ifndef __MSDOS__
#ifdef __cplusplus
}
#endif
#endif

#endif
