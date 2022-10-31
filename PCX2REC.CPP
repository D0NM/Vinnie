// Дессинатор для ткацкого станка
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

block hidscr;

int yesno(char *);
void info(char *t);
void message(char *t);
unsigned long PckBlock(block src, unsigned long lenblk);

void f_help(void);
void f_quit(void); //выход в дос
void f_set(void);
void f_setup(void);
void send_rom(void);
void print_info(void);
void print_utok(void);
void print_osnova(void);
void print_fact(void);
void print_bigfact(void);
void print_smallfact(void);
void choose_utok(void);
void choose_osnova(void);
void choose_fact(void);
void do_fact(void);
void f_disk(void);
void print_hat(void);
unsigned char set_rgb(unsigned char n);
void read_shabl(void);
void save_shabl(void);
void save_col(void);
void save_npcx(void);
void f_clearfact(void);
void f_new(void);
void pcx_bin(void);
void bin_rom(void);
void show_bin(void);
void show_pcx(void);
void run_ani(void);
//это как cut insert в Windows
void savepocket(void);
void restorepocket(void);

//Mouse m;
//Panel p;

#define xsp 240
#define ysp 30

#define maxROM 1806336L

struct RGB {
	char R;
	char G;
	char B;
};

//описание Заголовок PCX:
struct {
     char manuf;	// Всегда =10 для Paintbrush
     char hard;		// Информация о версии
     char encod;	// Групповое кодирование(=1)
     char bitpx;	// Бит на точку
     int x1;		// Размеры картинки(включит.)
     int y1;
     int x2;
     int y2;
     int hres;		// Гориз.разрешение дисплея
     int vres;		// Вертик.разрешение дисплея
	 char clrma[48];	// Палитра
     char vmode;	// (игнорируется)
     char nplanes;	// Кол-во плоскостей (вер.2.5=0)
     int bplin;		// Байт на строку
     int palinfo;	// Информ.о палитре (1=цв.,2=сер)
     int shres;		// Разрешение сканнера
     int svres;
     char xtra[54];	// Доп.пустое место (фильтр)
} Pcx={
	10,
	5,
	1,
	8,
	0,
	0,
	1343,
	499,
	320,
	200,
	"",
	0,
	1,
	1344,
	1,
	0,
	0,
	""
};
#define Headersize 128L

//фиксирован. длина инф. слова в строке!!!!
#define fixlen 169

struct {
	unsigned int CRC;	//CRC-16bit контрольная сумма
	char pack;		//зархивир. или нет
	char typ;		//тип рисунка
	char nutok,nosnova;	//кол-во нитей осн. в факт и кол-во утков
	unsigned int lx,ly;	//размер картинки в точках
	unsigned int bplin;	//кол-во байт на строку
	unsigned int leftx,rightx; //пустой отступ слева и справа
} ROMhead={
	00,
	0,0,6,8,
	1344,500,
	168,
	00,00
};

const unsigned char mask[8]={128,64,32,16,8,4,2,1};
	//для AND
const unsigned char mask0[8]={127,191,223,239,247,251,253,254};
	//для OR

const unsigned char sizes[10]={7,8,9,10,11,12,13,14,15,16};

struct shablon {
	//толщина нити утка
	unsigned char siz_utok[6];
	//толщина нити основы
	unsigned char siz_osnova[8];
//фактур
//цвет утка
	RGB col_utok[6];
//цвет нити основы
	RGB col_osnova[8];
//"цвет" фактуры (каждой из 64)
	RGB col_fact[64];
	unsigned char fact[64][8][6];
};

shablon s;	//текущая обл для работы

//для сохр. при редактироании
unsigned char pocket[8][6];

unsigned char tek_utok;
unsigned char tek_osnova;
unsigned char tek_fact;

block menu;
block tfnt;
block tmp;

char stroka[64];
char stroka0[64];
char stroka1[64];

char name0[16]="noname";
char name1[16]="noname1";

void main(int argc, char *argv[]) {
	int x,y,i,j,t,u,old_butt;
	int fdz,fdz1;

	puts("PCX2REC by BMV \n	example: pcx2rec rec.pcx");

//	if( strlen(argv[1])==0 ) {
//		fatalerror("не задан файл");
//	}
	strcpy(stroka,argv[1]);

	hidscr=(block)famemalloc((unsigned long)320*200);
	tmp=(block)famemalloc((unsigned long)320*200);

	SetLib("graph");

	pcx_bin();

	if ( (fdz=open ("reclama.pic",O_BINARY | O_RDWR)) < 0) {
		fatalerror("Файл .pic не найден!");
	}
	if (read (fdz, hidscr,(unsigned long)320*200)==-1 ) {
		fatalerror("Не могу считать рисунок");
		return;
	}
	close(fdz);

	//запись
	PutLib("reclama.pic",hidscr,(unsigned long)320*200);
	remove("reclama.pic");
	PackLib();
	SetLib("");
	puts("все прошло ОК!");
}

void fatalerror(char *t) {
	//выход по фатальной ошибке
	SetLib("");
	puts(t);
	exit(1);
}

void pcx_bin(void) {
	int fdz,fdz1;
	unsigned char data,repeatcount,lencod;
	unsigned int Xl,Yl;

	if ( (fdz=open (stroka,O_BINARY | O_RDWR)) < 0) {
		fatalerror("Файл .PCX не найден!");
	}
	if (read (fdz, tmp,Headersize)==-1 ) {
		fatalerror("Не могу считать заголовок");
		return;
	}
	lseek (fdz,128,SEEK_SET);
	_fmemcpy((block)&Pcx,tmp,Headersize);
	//---------------- размеры картинки ---------------
	ROMhead.lx=Xl=Pcx.x2-Pcx.x1+1; ROMhead.ly=Yl=Pcx.y2-Pcx.y1+1;
	ROMhead.bplin=ROMhead.lx/8;
	if(ROMhead.lx%8) {	//если занимает еще неполн. байт
		++ROMhead.bplin;
	}
	ROMhead.leftx=(fixlen-ROMhead.bplin)/2;
	ROMhead.rightx=(fixlen-ROMhead.bplin-ROMhead.leftx);
	//-------------------------------------------------

	//if( ((long)Xl*(long)Yl)>320*200 ) {
	//	fatalerror("Размеры (объем) рисунка более 320*200");
	//	return;
	//}

	_creat("reclama.pic",FA_ARCH);
	if ( (fdz1=open ("reclama.pic",O_BINARY | O_RDWR)) < 0) {
		fatalerror("Не могу открыть для записи файл");
		return;
	}
	puts("Подождите! Идет распаковка рисунка...");
	while (1) {
		if (read (fdz, &data, 1) == -1) {
			fatalerror("Ошибка чтения");
			close(fdz);
			close(fdz1);
			return;
		}
		//if ( (Yl&7)==0 ) {
			//MoveXY(290,190);char_fgd=16; char_bkgd=31;vprint("%04u",Yl);
			//SetColor(12); Bar(220,191,(int)max(99-(99*(long)Yl/ROMhead.ly),1),8);
		//}
		if ( (data & 0xC0)==0xC0 ) { /*установлены ли первые 2 бита*/
			repeatcount=(data & 0x3F);
			if ( read (fdz, &data, 1) == -1 ) {
				fatalerror("Ошибка чтения");
				close(fdz);
				close(fdz1);
				return;
			}
		}
		else {
			repeatcount=1;
		}
		lencod=repeatcount;
		do {	//вывод раскодиров. данных
			tmp[--repeatcount]=data;
			/*проверка конца файла*/
			if(!(--Xl)) {
				Xl=Pcx.x2-Pcx.x1+1;
				if(!(--Yl)) {
					if ( write (fdz1, tmp, (int)lencod) == -1 ) {
						fatalerror("Ошибка записи");
					}
					close(fdz);
					close(fdz1);
					puts("Файл .BIN создан успешно");
					return;
				}
			}
		} while (repeatcount);
		if (write (fdz1, tmp, lencod) == -1 ) {
			fatalerror("Ошибка записи");
			close(fdz);
			close(fdz1);
			return;
		}
	}
}

unsigned long PckBlock(block src, unsigned long lenblk) {
	static unsigned char t,l,cl;

	static unsigned long dpos,spos;

	cl=dpos=spos=0;
	//pakdest=(block)famemalloc((unsigned long)(lenblk+3));

	t=src[spos];
	for ( spos=0; spos<lenblk; spos++ ) {
		l=t; //предыдущий символ
		t=src[spos]; //возмем новый симв
		if (l==t) { //новый символ равен предыдущему
			cl++;
			if (cl>63) { //кол-во одинаковых симв превысило 64
				//закодир заголовок 11хххххх & символ
				pakdest[dpos++]= (0xff); //63
				pakdest[dpos++]= l;
				cl=1;
			}
		} else { //новый символ не равен предыдущему
			if (cl>1) { //запись цепочки
				pakdest[dpos++]= (0xc0 | cl);
				pakdest[dpos++]= l;
			} else { //символ без 2х перв бит
				if (l>127) { //запись цепочки
					pakdest[dpos++]= 0xc1; //1
					pakdest[dpos++]= l;
				} else {
					pakdest[dpos++]= l;
				}
			}
			cl=1;
		}
		//if (dpos>=lenblk) {
			//ошибка-переполнение при архивации
		//	return (dpos);
		//}
	} //конец осн. цикла
	if (cl >1) {
		pakdest[dpos++]= (0xc0 | cl);
		pakdest[dpos++]= l;
	} else {
		if (t>127) { //запись цепочки
			pakdest[dpos++]= 0xc1; //63
			pakdest[dpos++]= t;
		} else {
			pakdest[dpos++]= t;
		}
	}
	return (dpos);
}