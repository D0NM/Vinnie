#include	<stdlib.h>
#include	<dos.h>
#include	<malloc.h>
#include	<string.h>

#include "famegraf.h"
#include "joy.h"
#include "keys.h"
#include "all.h"

#ifdef EDITOR
#include "mouse.h"
Mouse m;
#include "dialog.h"
Panel p;
#endif

long tic; //для задержки по таймеру

void splitcopy(int x, int y, int lx, int ly,screen dest, int sx, int sy, screen src);

block demobuf; //буфер под демонстр нажатия клавишь
char demo=0; //демонстр 0выключена 1-включена
unsigned int pdemo;

#ifdef EDITOR
block menu;
#endif

block ind; //под разные значки и индикаторы

block trase;
block level;
block blevel;

block bfon;
block fon;
block manspr; //образы героя

block mnstrspr; //образы гадов
unsigned long pmnspr=0; //последний свободн байт

block items;
block additems;
block modelspr;
block lifts;
block tmp;
block tfnt;     //под большие шрифты

block obloka; //под взрывы
block enbull; //под вражеские снаряды

screen bckg; //задний фон
screen hidscr; //скрытая страничка

//char serial_pc=1; // 1- обычная писишка, 0 - Trident 512k
char speedraw; // 1,2 - для быстрой машины, 0 - медленная
char draw; //отрисовывать ли сейчас что-либо?

char str0[25];
/*const */ char far stroka[255];
char levnum[4]="01";
int curr_level=1;
// PATCH: save last valid password
char lastpass[6] = "";

char files[10][13]={ //файлы под уровни
	"01def.bin",
	"01level.bin",
	"01blevel.bin",
	"01monstr.bin",
	"01monstr.def",
	"01models.bin",
	"01demo.bin",
	"01fon.bin",
	"01bckg.bin",
	"01strip.def"
};

char f_rama[]="rama01.bin";

levdef leveldef;

//размер уровня
unsigned int siz_xlev=500;
unsigned int siz_ylev=60;
unsigned long siz_level=(unsigned long)500*60;//siz_xlev*siz_ylev;
unsigned long svel=9,cvel=8;

char poc[32][16]={
	{16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16}, //пустота 0
	{00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}, //цел прозрачный блок

	{15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0}, //скат /| 2

	{15,15,14,14,13,13,12,12,11,11,10,10,9,9,8,8}, //в друг стор 5
	{7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0}, //6


	{15,15,15,15,14,14,14,14,13,13,13,13,12,12,12,12}, //самый слаб скат
	{11,11,11,11,10,10,10,10,9,9,9,9,8,8,8,8},
	{07,07,07,07,06,06,06,06,05,05,05,05,04,04,04,04},
	{03,03,03,03,02,02,02,02,01,01,01,01,00,00,00,00},

	{00,00,00,00,01,01,01,01,02,02,02,02,03,03,03,03}, //самый слаб скат
	{04,04,04,04,05,05,05,05,06,06,06,06,07,07,07,07},
	{8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,11},
	{12,12,12,12,13,13,13,13,14,14,14,14,15,15,15,15},

	{0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7}, //скат слаб 3
	{8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15}, //продолжение 4

	{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}, //скат |\ 1


	{00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}, //цел блок
	{00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}, //кирпичная
	{00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}, // ?
	{00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}, //исчезающая

	{02,01,00,00,00,00,01,01,01,01,00,00,00,00,01,02}, //конвеер
	{02,01,00,00,00,00,01,01,01,01,00,00,00,00,01,02}, //конвеер

	{-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10}, //подпорка

	{16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16}, //вывод сверху
	{00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}, //резерв 2
	{00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}, //резерв 3
	{00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}, //резерв 4
	{00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00}, //резерв 5

	{11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11}, //еще выше
	{07,07,07,07,07,07,07,07,07,07,07,07,07,07,07,07}, //выше
	{03,03,03,03,03,03,03,03,03,03,03,03,03,03,03,03}, //низк

	{00,01,02,03,02,01,00,10,01,00,01,02,03,02,01,00} // шипы
};

char nappoc[32]={0,0,-3,-2,-2,-1,-1,-1,-1,1,1,1,1,2,2,3, 0,0,0,0,-3,3,0,0,0,0,0,0,0,0,0,0}; //папр и смещен
//char napypoc[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#define maxstrip 16
#define st_still 0
#define st_roll 1
#define st_go 2

int nstrip=1;
int striply=1;
#pragma pack(push, 1)
struct {
	short x,y; /*начальная позиция*/
	short ly;	/*высота полоски*/
	short sx,sy; /**/
	short st;
	short typ;
} strip[maxstrip];
#pragma pack(pop)
void show_strip(int y);
void do_strip(int sx);

int x,y,sx,sy; //тек положение точки
unsigned char far *poslab; //тек поз точки в массиве трассы
unsigned char far *poslal; //тек поз точки в массиве фона
unsigned char tekblock;

int coins;	//монетки
int hearts;	//сердечки
int keys;	//ключи
int beams;	//шишки
int shields;	//защита
int power;	//энергия
int times;	//время
unsigned long score,hiscore; //очки
int continues;	//продолжения
char maxjump;	//высота прыжка
char maxspeed;	//максимальная скорость
char glass;	//очки
char shboots;	//шиповки
char spboots;	//скоростные башмаки
char jetpack;	//ранец
char kolun;	//колун

char boss;	//признак активности БОССА

unsigned int nsecret;	//# секретов
unsigned int nitems;	//# вещей

//герой
hero man;

//враги
object obj[maxobj];
strkobj kobj[maxobj];

unsigned char waitmess=0;
int messy;
char messsy;
char * mess;
char waitboom=0;
char waitfire=0;
boom booms[maxboom];
strbullets bullets[maxbullet];

monstr monsters[maxmonstr];
int nmonstr=0; //текущ кол-во видов монстров

strmodels models[maxmodel];

static char *tmenu[]={
"Игра",
"Пароль",
"Демо",
"Помощь",
"Авторы",
"Выход"
};

sshop shop[12]={
	{05,"Шишка",16,16,&items,256*8*4,7},
	{10,"Мед",16,16,&items,256*8,7},
	{25,"Часы",16,16,&items,256*8*5,7},
	{99,"+1 Жизнь",16,16,&items,256*8*2,7},
	{100,"+1 Продолжение",16,16,&additems,0,1},
	{30,"Супер Сила",16,16,&items,256*8*6,7},
	{50,"Супер Щит",16,16,&additems,256*12,1},
	{110,"Супер Линза",16,16,&additems,256*8,1},
	{120,"Супер Прыгалки",16,16,&additems,256*2,1},
	{175,"Супер Шиповки",16,16,&additems,256*4,1},
	{145,"Супер Колун",16,16,&additems,256*10,1},
	{250,"Супер Ранец",16,16,&additems,256*6,1}
};

strup upput[maxnup];
int nup=-1; //кол-во элементов сверху

#ifdef EDITOR
unsigned char curr_fon=1;
unsigned char curr_trase=0;
unsigned char curr_items=0;
unsigned char curr_monstr=0;
char curr_model=0;
//режимы редактора
unsigned char st_fon=1;		 //редактировать фон
unsigned char st_trase=0;	//редактировать трассу
unsigned char st_items=0;	//редактировать вещи
unsigned char st_monstr=0;       //установка на поле монстров
unsigned char st_model=0;       //установка на моделей
#endif

unsigned int xw=0;
unsigned int yw=0;

int yesno(char *);
// PATCH: end => zend (conflict with some internal variable)
// PATCH: added volatile or got infinite stuck in pause mode
volatile int zend,pause,key_f,key_b;
volatile int left,right,up,down,jump,fire;
volatile int t_left,t_right,t_up,t_down,t_jump,t_fire;

unsigned int r_x,r_y; //прежние координаты лев. верхн угла окна

#ifdef DEMO
block lfonts;    /*буфер для тонких шрифтов*/
//fonts=(block)famemalloc(2048+256);
#endif


#pragma pack(push, 1)
struct {
	short dev,mix,vol;
	short j;	//исп джойстик или нет
	short f;	//включить сзади фон или нет
	short cheat; //кряк - или нет
} setup;
#pragma pack(pop)
int dev,mix,vol,state;

// PATCH: new SwapShort() for short type since type changed from int
void SwapShort (short *pa,short *pb) {
	register short t;
	t=*pa; *pa=*pb; *pb=t;
}

// PATCH: for non Russian keyboard
int GetLevelPass(int xs, int ys, char *str, int lstr) {
int lpos, i;
//int xs = G_x, ys = G_y; /*поз графич курсора*/
char oldbk;
  oldbk = char_bkgd;
  char_bkgd = 30;
  kb_on();
  zend = left = right = up = down = jump = fire = 0;
  // PATCH: save last valid password
  if (lastpass[0]) {
    lpos = lstr - 1;
    strcpy(stroka, lastpass);
  } else {
    lpos = 0;
    str[lpos] = '\x80';
  }
  str[lpos + 1] = 0;
  up = 1;
  str[lpos]++;
  i = 0;
  while (!i) {
    if (fire || zend || up || down || left || right) {
      do {
        if (left) {
          if (lpos) {
            str[lpos] = 0;
            lpos--;
          }
        }
        if (fire || right) {
          lpos++;
          if (lpos >= lstr) {
            i = 1;
          } else {
            str[lpos] = str[lpos - 1];
            str[lpos + 1] = 0;
          }
          break;
        }
        if (zend) {
          str[0] = 0;
          i = -1;
          break;
        }
        if (up) {
          if (str[lpos] > '\x80') {
            str[lpos]--;
          } else {
            str[lpos] = '\x9F';
          }
          break;
        }
        if (down) {
          if (str[lpos] < '\x9F') {
            str[lpos]++;
          } else {
            str[lpos] = '\x80';
          }
          break;
        }
      } while (0);
      if (!i) {
        MoveXY(xs, ys);
        for (i = 0; i <= (lstr + 1); i++) { PutCh(' '); }
        i = 0;
        MoveXY(xs, ys);
        vputs(str);
      }
      while (zend || left || right || up || down || jump || fire) {
        zend = left = right = up = down = jump = fire = 0;
      }
    }
  }
  kb_off();
  char_bkgd = oldbk;
  return(i);
}

// PATCH: joystick
void jupdate(unsigned short butt) {
  // allow joystick and keyboard to coexists
  left  = (butt & JOY1_L) ? 1 : left;
  right = (butt & JOY1_R) ? 1 : right;
  up    = (butt & JOY1_U) ? 1 : up;
  down  = (butt & JOY1_D) ? 1 : down;
  fire  = (butt & JOY1_A) ? 1 : fire;
  jump  = (butt & JOY1_B) ? 1 : jump;
}

int main(void) {
	int i,j;
	// PATCH: editor only
	#ifdef EDITOR
	int x,y,p_up,p_down,p_left,p_right;
	#endif
	static char f_help[13]="help0.pic";
	#ifdef __MSDOS__
	block mpcs; // PATCH: PC Speaker
	#endif

	kb_set(md);
	kb_on();

	//считаю конфиг из файла
	SetLib("graph");
	GetLib("pooh.cfg",(block)&stroka);
	_fmemcpy((block)&setup.dev,(block)&stroka,(unsigned long)sizeof(setup));
	vol=setup.vol; dev=setup.dev; mix=setup.mix;
	dev = ((dev == -1) ? 255 : dev); // PATCH: fix disabled state (-1 => 255)
	js_init();
	js_set(jupdate);
	if (js_state() && setup.j)	//если есть жустик
		js_on();
	//if (J.present)	//если есть жустик
		//J.present=setup.j;

#ifdef EDITOR
	dev=255;
	speedraw=setup.f=3;
	G_lx=G_ly=6;
#else
	speedraw=setup.f;	//фон надо?
	//k.off();
	if ( dev!=255 ) { // PATCH: disable if not specified
		#ifdef __MSDOS__
		// PATCH: PC Speaker constantly make noise in DOSBox with other sound devices
		if (dev != 0) {
			mpcs = (block) modinit;
			mpcs += 0x1852 - 0x26A5; // add instruction address, sub modinit() address
			// check for required code to patch: or al, 003h
			if ((mpcs[0] == 0x0C) && (mpcs[1] == 0x03)) {
				mpcs[1] = 0x00;
			}
		}
		#endif
		modinit();
	}
	//moddevice( &dev );

#endif

#define needmem (380000+(dev==255?64000:125000))

/*	if ( coreleft()<needmem ) {
		sprintf(stroka,"Надо еще %lu байт ОЗУ\n"
		,needmem-coreleft());
		fatalerror(stroka);
	}*/
	if ( dev!=255 ) // PATCH: disable if not specified
		modvolume(vol,vol,vol,vol);

#ifdef EDITOR
	js_off();
	//J.present=0;
#else
	//if ( J.present ) {
	if (js_state()) {
		printf("Калибровка 1го джойстика:\n"
			"Отцентрируйте джойстик и нажмите кнопку,\n"
			"Наклоните джойстик в левый верхний угол и нажмите кнопку,\n"
			"потом в правый нижний угол и нажмите кнопку.");
		if ( dev!=255 ) {
			//вкл музыку настройки джойстика
			modsetup("intro.fms", 4, 0 ,mix, dev, &state );
			modvolume(vol,vol,vol,vol);
		}
		js_test();
		//J.init();
		if ( dev!=255 ) {
			//выкл муз
			modstop();
		}

	}
#endif

	//PVOICE_INIT(); //иниц говорилку

	InitGraph();
	GetPalette(palette);
	PaletteOff(palette);
	Vga256();
	//Set_Mode_256( Mode_R4 );

#ifdef EDITOR
	GetLib("light.fit",fonts);
#else
	GetLib("normal.fit",fonts);
#endif
	PutFont(fonts);

#ifdef DEMO
	lfonts=(block)famemalloc(2048+256);
	GetLib("light.fit",lfonts);
#endif

	GetLib("normal.col",palette);

	for ( i=0; i<256*3; ++i )
		palette1[i]=0;
#ifdef EDITOR
	PutPalette(palette);
	menu=(block)famemalloc(SizeLib("menu.bin"));
	GetLib("menu.bin",(block)menu);
	trase=(block)famemalloc(SizeLib("trase.bin"));
	GetLib("trase.bin",(block)trase);
#else
	PutPalette(palette1);
#endif
	obloka=(block)famemalloc(SizeLib("obloka.bin"));
	GetLib("obloka.bin",(block)obloka);
	enbull=(block)famemalloc(SizeLib("enbull.bin"));
	GetLib("enbull.bin",(block)enbull);
	lifts=(block)famemalloc(SizeLib("lift.bin"));
	GetLib("lift.bin",(block)lifts);
	items=(block)famemalloc(SizeLib("items.bin"));
	GetLib("items.bin",(block)items);
	additems=(block)famemalloc(SizeLib("additm.bin"));
	GetLib("additm.bin",(block)additems);
	modelspr=(block)famemalloc(SizeLib("models.bin"));
	GetLib("models.bin",(block)modelspr);
	tfnt=(block)famemalloc(SizeLib("tfnt.bin")); //под большие шрифты
	GetLib("tfnt.bin",(block)tfnt);
	ind=(block)famemalloc(SizeLib("honey.bin"));
	GetLib("honey.bin",(block)ind);
	manspr=(block)famemalloc(SizeLib("men.bin"));
	GetLib("men.bin",(block)manspr);
	demobuf=(block)famemalloc(maxdemo);
	tmp=(block)famemalloc((unsigned long)2048);

	fon=(block)famemalloc((unsigned long)49152);
	_fmemset(fon,0,(unsigned long)49152);
	if( setup.f>1 ) {
		bckg=famemalloc((unsigned long)320*200);
		_fmemset(bckg,0,(unsigned long)320*200);
	}
	mnstrspr=(block)famemalloc((unsigned long)320*200);
	level=(block)famemalloc(maxsiz_level);
	blevel=(block)famemalloc(maxsiz_level);
	hidscr=famemalloc((unsigned long)320*200);

	//очистка начальная
	_fmemset(level,0,maxsiz_level);
	_fmemset(blevel,0,maxsiz_level);
	_fmemset(demobuf,0,maxdemo);
	for ( i=0; i<maxobj; ++i ) {
		//инициализируем массив с монстрами
		kobj[i].n=-1;
	}
        for ( i=0; i<maxmodel; ++i ) {
		//инициализируем массив с моделями
		models[i].typ=-1;
                models[i].on0=models[i].name0[0]=models[i].name[0]=0;
	}


#ifdef EDITOR
	Cls(28);
	p.init();
	p.add(xsp,ysp+18*0,16,16,"D",(block)menu+256*0,f_disk);
	p.add(xsp,ysp+18*1,16,16,"S",(block)menu+256*1,paramlev);
	p.add(xsp,ysp+18*2,16,16,"C",(block)menu+256*2,f_clear);
	p.add(xsp,ysp+18*3,16,16,"R",(block)menu+256*6,f_game);
	p.add(xsp,ysp+18*4,16,16,"M",(block)menu+256*7,f_map);
	p.add(xsp-2,ysp+18*7,16,10,"XY",NULL,f_poohxy);
	p.add(xsp-2,ysp+18*7+20,16,10,"Демо",NULL,f_gamedemo);
	p.add(xsp,ysp+18*10-4,16,16,"L",(block)menu+256*10,choose_model);
	p.add(xsp,ysp+18*5,16,16,"Q",(block)menu+256*5,f_quit);
	p.addr(0,0,16,16,"F",choose_fon);
	p.addr(0,176,16,16,"T",choose_trase);
	p.addr(16*17,0,16,16,"I",choose_items);
	p.addr(16*17,176,16,16,"O",choose_monstr);
	p_up=p.add(17,1,16*16-2,14,NULL,NULL,but_up);
	p_down=p.add(17,177,16*16-2,14,NULL,NULL,but_down);
	p_left=p.add(1,17,14,16*10-2,NULL,NULL,but_left);
	p_right=p.add(273,17,14,16*10-2,NULL,NULL,but_right);
	p.add(16,200-7,10,8,"Фон",NULL,switch_fon);
	p.add(16+30,200-7,18,8,"Трасса",NULL,switch_trase);
	p.add(16+30+47,200-7,16,8,"Вещи",NULL,switch_items);
	p.add(16+30+47+30,200-7,26,8,"Монстры",NULL,switch_monstr);
	p.add(16+30+47+30+50,200-7,26,8,"Модели",NULL,switch_model);

	m.on();
	k.off();
	ris_lab(xw,yw);
	put_curr();
	while ( 1 ) {
		p.press();

		if ( m.left.press && m.x>16 && m.x<16*17 && m.y>16 && m.y<16*11) {
                        //рисование мышой
			if ( st_model ) {
				if ( st_model==1 ) {
					//то присваиваем начальные координаты
					models[curr_model].x1=xw*16+(m.x-16);
					models[curr_model].y1=yw*16+(m.y-16);
				} else {
					//иначе конечные
					models[curr_model].x2=xw*16+(m.x-16);
					models[curr_model].y2=yw*16+(m.y-16);
				}
				st_model=st_monstr=st_fon=st_trase=st_items=0;
				put_curr();
			} else
			if ( st_monstr ) {
				for ( i=0; i<maxobj; ++i ) {
					//из массива достаем монстрят
					if ( kobj[i].n<0 ) {
						kobj[i].x=xw*16+(m.x-16);
						kobj[i].y=yw*16+(m.y-16);
						kobj[i].n=curr_monstr;
						st_monstr=st_fon=st_trase=st_items=0;
						put_curr();
						break;
					}
				}
			} else {
				if ( st_fon ) {
					*(level+(xw+(m.x-16)/16)+(yw+(m.y-16)/16)*siz_xlev)=curr_fon;
				}
				if ( st_trase ) {
					*(blevel+(xw+(m.x-16)/16)+(yw+(m.y-16)/16)*siz_xlev)=curr_trase;
				}
				if ( st_items ) {
					*(blevel+(xw+(m.x-16)/16)+(yw+(m.y-16)/16)*siz_xlev)=curr_items<<5|(31&(*(blevel+(xw+(m.x-16)/16)+(yw+(m.y-16)/16)*siz_xlev)));
				}
			}
			ris_lab(xw,yw);
		} else
		if ( m.right.press && m.x>16 && m.x<16*17 && m.y>16 && m.y<16*11) {
			//запомнить под курсором

			if ( st_monstr ) {
				for ( i=0; i<maxobj; ++i ) {
					//из массива достаем монстрят
					if ( kobj[i].n>=0 &&
					  ( kobj[i].x<=xw*16+(m.x-16) && kobj[i].x+monsters[kobj[i].n].lx>=xw*16+(m.x-16) &&
					  kobj[i].y>=yw*16+(m.y-16) && kobj[i].y-monsters[kobj[i].n].ly<=yw*16+(m.y-16) )	) {

						curr_monstr=kobj[i].n;
						kobj[i].n=-1;
						st_monstr=1;
						ris_lab(xw,yw);
						put_curr();
						break;
					}
				}
			}

			if ( st_fon ) {
				curr_fon=*(level+(xw+(m.x-16)/16)+(yw+(m.y-16)/16)*siz_xlev);
				put_curr();

			}
			if ( st_trase ) {
				curr_trase=(*(blevel+(xw+(m.x-16)/16)+(yw+(m.y-16)/16)*siz_xlev))&31;
				put_curr();
			}
			if ( st_items ) {
				curr_items=*(blevel+(xw+(m.x-16)/16)+(yw+(m.y-16)/16)*siz_xlev)>>5;
				put_curr();
			}
		}
		if ( m.right.press ) { //быстрый скроллинг
			if ( p.current==p_up )
				but_up();
			else if ( p.current==p_down )
				but_down();
			else if ( p.current==p_left )
				but_left();
			else if ( p.current==p_right )
				but_right();
		} else {
			p.current=-1;
		}
	}
#else
	//Keys k=Keys(md);
	curr_level=0; zend=0;

	while ( zend==0 ) {
		//пока не нажата какая-либо клавиша - засставка
		kb_set(md);
		str0[0] = 0; // PATCH: fix glitched menu
		SetLib("mult"); //мульт на начало уровня
		mult("zast.rec");
		SetLib("graph");
		if ( zend ) {
			goto mainmenu;
		}

		//заставка
		if ( dev!=255 ) {
			//вкл музыку заставки
			modsetup("intro.fms", 4, 0 ,mix, dev, &state );
			modvolume(vol,vol,vol,vol);
		}
		PaletteOn(palette);

		tic=gettic()+18*10;
		zend=0;
                while (gettic()<tic && zend==0 /*&& m.left.press==0*/) {
			delay(500);
		}
		//if ( m.left.press ) end=1;

		PaletteOff(palette);
mainmenu:

		if ( dev!=255 ) {
			//выкл муз
			modstop();
		}

		if ( zend ) {
			//создание фона для меню
			//m.off();
			SetLib("graph");
			GetLib("normal.col",palette);
			GetLib("tiles.bin",(block)hidscr);
			NormalScreen();
			Clip(0,0,319,199);
			TileBar(0,0,320,200,64,64,(block)hidscr+random(4)*64*64);
#define nmenx 126
#define nmeny 62
			WPut(nmenx-5,nmeny-5,78,14*6+6);
			char_fgd=17;
			//меню
			for ( i=0; i<6; ++i ) {
				MoveXY(nmenx,nmeny+1+14*i);
				WPut(nmenx-1,nmeny-1+14*i,70,12);
				vputsc(tmenu[i]);
			}

			MoveXY(5,10);
			vputBs("Супер Винни-Пух",tfnt+144*4);
			MoveXY(22,170);
			vputBs("(C) 2023, v2.06", tfnt + 144 * 1); // PATCH: show version and year
			//vputBs("(C) 1996 FaMe",tfnt+144*1);
#ifdef DEMO
			MoveXY(10,192);
			vputsc("Демонстрационная версия");
#endif
			MoveXY(0,154);
			//vprint("Св.пам: %lu байт",(unsigned long) coreleft());
			if ( coreleft()<130000 && dev!=255 ) {
				vputsc("Для музыки надо 620k свободной памяти");
			}

			PaletteOn(palette);
			//m.on();
			kb_set(mz);

			if ( dev!=255 ) {
				//вкл музыку
				modsetup("intro.fms", 4, 0 ,mix, dev, &state );
				modvolume(vol,vol,vol,vol);
			}
			j=zend=left=right=up=down=jump=fire=i=0;

			while ( fire==0 && jump==0 ) {
				//цикл меню
				//p.press();
				//пауза

				CurrentColor=32;
				//m.off();
				Rectangle(nmenx-2,nmeny-2+14*i,72,14);
				//m.on();
				tic=gettic()+2;
        			while (gettic()<tic) {
					delay(100);
				}
				CurrentColor=wfon;
				//m.off();
				Rectangle(nmenx-2,nmeny-2+14*i,72,14);
				//m.on();
				tic=gettic()+2;
        			while (gettic()<tic) {
					delay(100);
				}

				if ( up || left ) {
					j=up=left=0;
					if (--i<0 ) {
						i=5;
					}
				}
				if ( down || right ) {
					j=down=right=0;
					if (++i>5 ) {
						i=0;
					}
				}
				if ( zend==1 ) {	//выход по еску
					fire=i=5;
				}
				if ( ++j>100 ) {	//выход на ДЕМО
					fire=i=2;
				}
			}
			switch ( i ) {
				case 0:
					//начать Игру
					if ( dev!=255 ) {
						modstop();//выкл муз
					}
					SetScreen(hidscr);
					coins=demo=0;
					curr_level=1;
					f_game();
					curr_level=0;
					zend=0;
					break;
				case 1:
					//m.off();
					demo=0;
					MoveXY(nmenx+60,nmeny+14*1);
					kb_off();
					stroka[0]=0;
					WPut(nmenx+55,nmeny+14*1-5,60,19);
					MoveXY(nmenx+60,nmeny+14*1);
					// PATCH: for non Russian keyboard
					GetLevelPass(nmenx + 60, nmeny + 14 * 1, stroka, 5);
					//GetString(stroka,5);
					kb_on();
					//m.on();
					if ( dev!=255 ) {
						modstop();//выкл муз
					}
					curr_level=testpass();
#ifndef DEMO
					if( curr_level>0 && curr_level<=9 ) {
#else
					if( curr_level>0 && curr_level<=2 ) {
#endif
						// PATCH: save last valid password
						strcpy(lastpass, stroka);
						SetScreen(hidscr);
						f_game();
					} else {
						// PATCH: save last valid password
						lastpass[0] = 0;
					}
					curr_level=0;
					zend=0;
					delay(1000); // PATCH: wait for pressed keys to release
					break;
				case 2:	//демо
					PaletteOff(palette);
					if ( dev!=255 ) {
						modstop();//выкл муз
					}
					goto do_demo;
					//break;

				case 3:	//инструкция
					//m.off();
					if ( dev!=255 ) {
						//выкл муз
						modstop();
					}
					str0[0]=i=left=right=zend=0;
					do {
						PaletteOff(palette);
						f_help[4]='0'+(char)i;
						GetLibs(f_help,(block)hidscr);
						PutImg(0,0,320,200,(block)hidscr);
						PaletteOn(palette);
						while( left==0 && right==0 && zend==0 );
						if( left && i>0) {
							i--;
						} else
						if( right && i<5) {
							i++;
						}
						left=right=0;
					} while( zend==0 );
					PaletteOff(palette);
					zend=1;
					goto mainmenu;

				case 4:	//об авторе
					//m.off();
					if ( dev!=255 ) {
						//выкл муз
						modstop();
					}
					zend=0;
					mult("author.rec");
					zend=1;
					goto mainmenu;

				case 5:
					//выход в дос
					zend=1;
					//break;

			}


		} else if ( zend==0 ) {
			//демонстрация
do_demo:
			//m.off();
			demo=1;

			//SetLib("graph");
			//GetLib("normal.col",palette);
			SetLib("mult");

			if( setup.f>1 ) {
				//bckg=famemalloc((unsigned long)320*200);
				farfree(bckg);
			}
			farfree(mnstrspr);
			farfree(level); farfree(blevel);

			if ( dev!=255 ) {
				//вкл музыку заставки
				modsetup("intro.fms", 4, 0 ,mix, dev, &state );
				modvolume(vol,vol,vol,vol);
			}

			mult("intro.rec");


			if ( dev!=255 ) {
				//выкл муз
				modstop();
			}
			if( setup.f>1 ) {
				bckg=famemalloc((unsigned long)320*200);
				//farfree(bckg);
			}
			mnstrspr=(block)famemalloc((unsigned long)320*200);
			level=(block)famemalloc(maxsiz_level);
			blevel=(block)famemalloc(maxsiz_level);

#ifndef DEMO
			if ( ++curr_level>6 ) {
#else
			if ( ++curr_level>2 ) {
#endif
				curr_level=1;
			}
			//m.on();
			f_game();
			zend=0;
		}

	}
#endif
	fatalerror("Спасибо за игру!");
	return(0);
}

void readlev(void) {
	int i;
	SetLib("levels");
	sprintf(levnum,"%02d",curr_level);
	for (i=0; i<=9; ++i) {
		files[i][0]=levnum[0];
		files[i][1]=levnum[1];
	}
	GetLib(f_def,(block)hidscr);
	_fmemcpy(&leveldef,(block)hidscr,(unsigned long)sizeof(leveldef));
	//из общих парам уровня узнаем имена фона и т.п.
	strcpy(f_fon,leveldef.fonname);
	strcpy(f_bckg,leveldef.bckgname);
	siz_xlev=leveldef.lx;
	siz_ylev=leveldef.ly;
	siz_level=(unsigned long)siz_xlev*siz_ylev;

	GetLib(f_bmonstr,(block)hidscr);
	_fmemcpy(&kobj,(block)hidscr,(unsigned long)sizeof(kobj));
	GetLib(f_models,(block)hidscr);
	_fmemcpy(&models,(block)hidscr,(unsigned long)sizeof(models));
	GetLib(f_level,(block)level);
	GetLib(f_blevel,(block)blevel);
	GetLib(f_demo,(block)demobuf);
	GetLib(f_fon,(block)fon);
	if( setup.f>1 ) {
		GetLib(f_bckg,(block)bckg);
	}
	GetLib(f_dstrip,(block)tmp);
	readstrip();
	GetLib(f_dmonstr,(block)tmp);
	readmonstr();
	//подсчет кол-ва секретов
	nsecret=0;
	for ( i=0; i<maxmodel; ++i ) {
		if ( models[i].typ>=m_copy ) ++nsecret;
	}
	//подсчет кол-ва вещей
	nitems=0;
	for ( i=0; i<siz_level; ++i ) {
		if ( blevel[i]&224 ) ++nitems;
	}
}

void savelev(void) {
	int i;
	SetLib("levels");
	sprintf(levnum,"%02d",curr_level);
	for (i=0; i<=9; ++i) {
		files[i][0]=levnum[0];
		files[i][1]=levnum[1];
	}
	PutLib(f_def,(block)&leveldef,(unsigned long)sizeof(leveldef));
	GetLib(f_def,(block)hidscr);
	_fmemcpy(&leveldef,(block)hidscr,(unsigned long)sizeof(leveldef));
	strcpy(f_fon,leveldef.fonname);
	strcpy(f_bckg,leveldef.bckgname);

	PutLib(f_level,(block)level,siz_level);
	GetLib(f_level,(block)level);
	PutLib(f_blevel,(block)blevel,siz_level);
	GetLib(f_blevel,(block)blevel);
	PutLib(f_demo,(block)demobuf,maxdemo);
	GetLib(f_demo,(block)demobuf);

	PutLib(f_bmonstr,(block)&kobj,(unsigned long)sizeof(kobj));
	GetLib(f_bmonstr,(block)hidscr);
	_fmemcpy(&kobj,(block)hidscr,(unsigned long)sizeof(kobj));
	PutLib(f_models,(block)&models,(unsigned long)sizeof(models));
	GetLib(f_models,(block)hidscr);
	_fmemcpy(&models,(block)hidscr,(unsigned long)sizeof(models));
}

void readmonstr(void) {
	//считывание описания из файла и создание массива
	int n,i,j,f;
	block pos=tmp;

	nmonstr=0;
	pmnspr=0; //последний свободн байт в буфере образов монстров

	SetLib("graph");
	//считаем очередн строку
	memcpy(&stroka,pos,250);

	//worm 16 16  0 8 ?  16 8 ?  8 16 ?  8 0 ?  0  worm.bin  1 2 3 4 5 6
	n=sscanf(stroka,"%s  %u %u  %d %d %d  %d %d %d  %d %d %d  %d %d %d  %u  %s  %u %u %u %u",
	&man.name,&man.lx,&man.ly,
	&man.lfx,&man.lfy1,&man.lfy2,&man.rtx,&man.rty1,&man.rty2,
	&man.upx1,&man.upx2,&man.upy,&man.dwx1,&man.dwx2,&man.dwy,
	&man.typ,&man.namespr,&man.maxgo,&man.maxjm,&man.maxst,&man.maxspec
	);

	man.dsx1=man.dsx2=man.dsy=man.f=man.sy=man.sx=0;

	//farfree(manspr);
	//manspr=(block)famemalloc(SizeLib(man.namespr));
	GetLib(man.namespr,(block)manspr);

	#define m_s man.lx*man.ly
	//рассчет смещений спрайтов
	man.gospr=manspr;
	man.jmspr=manspr+m_s*(man.maxgo);
	man.stspr=manspr+m_s*(man.maxgo+man.maxjm);
	man.specspr=manspr+m_s*(man.maxgo+man.maxjm+man.maxst);

	//идем на след строку
	while ( *pos>27 || *pos==9 ) ++pos; while ( *pos<27 ) ++pos;

		for (i=0; i<maxmonstr; ++i ) {
			//считаем очередн строку
			memcpy(&stroka,pos,250);

			//worm 16 16  0 8  16 8  8 16  8 0  0  worm.bin  4
			n=sscanf(stroka,"%s  %u %u  %d %d  %d %d  %d %d  %d %d  %u %u %u  %s  %u %u %u %u",
			&monsters[i].name,&monsters[i].lx,&monsters[i].ly,
			&monsters[i].lfx,&monsters[i].lfy,&monsters[i].rtx,&monsters[i].rty,
			&monsters[i].upx,&monsters[i].upy,&monsters[i].dwx,&monsters[i].dwy,
			&monsters[i].typ,&monsters[i].speed,&monsters[i].lives,&monsters[i].namespr,
			&monsters[i].maxgo,&monsters[i].maxjm,&monsters[i].maxst,&monsters[i].maxspec
			);

			if ( n<10 ) {
				//когда описание кончилось - выход
				break;
			}

			//это монстр
			//вычислим начала спрайтов
			f=0; //флаг, что таких спрайтов нет...
			for ( j=0; j<nmonstr; ++j ) {
				if (j<i && stricmp(monsters[i].namespr,monsters[j].namespr)==0) {
					//если уже есть данный спрайт, то
					//сошлемся на него
					monsters[i].gospr=monsters[j].gospr;
					monsters[i].jmspr=monsters[j].jmspr;
					monsters[i].stspr=monsters[j].stspr;
					monsters[i].specspr=monsters[j].specspr;
					f=1; //флаг, что есть такой спрайт
					++nmonstr;
					break;
				}
			}

			//если такого спр не было, то загрузим
			if (f==0 && GetLib(monsters[i].namespr,mnstrspr+pmnspr)) {

				#define m_o monsters[i].lx*monsters[i].ly

				monsters[i].gospr=mnstrspr+pmnspr;
				monsters[i].jmspr=mnstrspr+pmnspr+m_o*(monsters[i].maxgo);
				monsters[i].stspr=mnstrspr+pmnspr+m_o*(monsters[i].maxgo+monsters[i].maxjm);
				monsters[i].specspr=mnstrspr+pmnspr+m_o*(monsters[i].maxgo+monsters[i].maxjm+monsters[i].maxst);

				if (monsters[i].maxst==0)
					monsters[i].stspr=monsters[i].gospr;

				pmnspr+=SizeLib(monsters[i].namespr);
				++nmonstr;
			}

			//идем на след строку
			while ( *pos>27 || *pos==9 ) ++pos; while ( *pos<27 ) ++pos;
	}
}

void doshop(int maxit) {
	//магазин
	screen old=CurrentScreen;
	int i,j,l;
	if ( maxit<=0 ) maxit=1;

	kb_set(mz);

	CurrentColor=draw=char_bkgd=31;
	l=i=j=zend=fire=jump=down=left=right=up=
	t_fire=t_jump=t_down=t_left=t_right=t_up=0;

	NormalScreen();
	TileBar(16,16,16*16,16*10,45,63,lifts);
	MoveXY(80,16+7);
	vputBs("МАГАЗИН",tfnt+144*2);
	PutMas(208,114,32,32,(block)modelspr+256*6+32*32*2);
	Bar(19,165,16*16-6,9);
	MoveXY(20,166);
	vputs("/ , ENTER - Купить, CTRL - Выход");

	for ( i=0; i<maxit; ++i ) {
		Bar(16+32*(i&7),32+16+32*(i/8),32,32);
		PutMtb(16+8+32*(i&7),32+16+8+32*(i/8),shop[i].lx,shop[i].ly,16,16,*shop[i].buf+shop[i].sm);
	}

	//инициализируем задержку по таймеру
	tic=gettic()+wclock;
	while ( jump==0 ) {

		if ( l!=i ) {
			WVR();
			CurrentColor=0;
			Bar(16+32*(l&7),32+16+32*(l/8),32,32);
			PutMtb(16+8+32*(l&7),32+16+8+32*(l/8),shop[l].lx,shop[l].ly,16,16,*shop[l].buf+shop[l].sm);
			if ( power ) {
				PutMas(24,24,16,16,items+256*8*6+((j)*256));
			}
			l=i;
			CurrentColor=char_bkgd=31;
			Bar(31,137,6*26+2,9);
			MoveXY(32,138);
			vprint("$%03u - %-15s",shop[i].price,shop[i].name);
		}

		if ( ++j>shop[i].mf ) j=0;
		PutMtb(16+1+32*(i&7),32+16+1+32*(i/8),shop[i].lx,shop[i].ly,30,30,*shop[i].buf+shop[i].sm+j*256);
		CurrentColor=15+j;
		Rectangle(16+32*(i&7),32+16+32*(i/8),32,32);


		while (gettic()<tic);
		tic=gettic()+2;

		//считаем с жустика, если он есть
		if (js_state()) {
			js_read();
		}
		/*if ( J.present ) {
			J.status();
			up=J.a.up;
			down=J.a.down;
			left=J.a.left;
			right=J.a.right;
			fire=J.a.a;
			jump=J.a.b;
		}*/

		if ( up || left ) {
			if (--i<0)
			i=maxit;
			left=up=0;
			tic=gettic()+2;
		}
		if ( down || right ) {
			if (++i>maxit) i=0;
			down=right=0;
			tic=gettic()+2;
		}
		if ( fire ) {
			fire=0;
			if ( coins>=shop[i].price )
			switch (i) {
			case 0:
				//ШИШКА
				if ( beams<99) {
					coins-=shop[i].price;
					++beams;
				}
				break;
			case 1:
				//МЕД
				if ( man.on<5) {
					coins-=shop[i].price;
					++man.on;
				}
				break;
			case 2:
				//ЧАСЫ
				if (times<5000) {
					coins-=shop[i].price;
					times=9999;
				}
				break;
			case 3:
				//ЖИЗНИ
				if (hearts<9) {
					coins-=shop[i].price;
					++hearts;
				}
				break;
			case 4:
				//continues
				if (continues<9) {
					coins-=shop[i].price;
					++continues;
				}
				break;
			case 5: //энергия
				if (power<400) {
					coins-=shop[i].price;
					power=500;
				}
				break;
			case 6: //ЗАЩИТА
				if (shields==0) {
					coins-=shop[i].price;
					shields=1;
				}
				break;
			case 7:	//очки
				if (glass==0) {
					coins-=shop[i].price;
					glass=1;
				}
				break;
			case 8:
				//пружинка
				if (maxjump>-9) {
					coins-=shop[i].price;
					--maxjump;
				}
				break;
			case 9:
				//шиповки
				if (shboots==0) {
					coins-=shop[i].price;
					shboots=1;
				}
				break;
			case 10:
				//колун
				if (kolun==0) {
					coins-=shop[i].price;
					kolun=1;
				}
				break;
			case 11:
				//ранец
				if (jetpack==0) {
					coins-=shop[i].price;
					jetpack=1;
				}
				//break;
			}
			put_score();put_additems();
		}

	}
	//if ( J.present==0 )
	//	while ( jump!=0); //ждем пока не отпустят ESC
	char_bkgd=up=down=left=right=fire=zend=jump=0;
	SetScreen(old);
	kb_set(mm);
}
#ifdef EDITOR

void put_curr(void) {
	m.off();
	if ( st_model ) {
		CurrentColor=4;
		Bar(0,0,16,16);Bar(0,176,16,16);
		Bar(16*17,0,16,16);Bar(16*17,176,16,16);
	} else
	if ( st_monstr ) {
		CurrentColor=3;
		Bar(0,0,16,16);Bar(0,176,16,16);
		Bar(16*17,0,16,16);
		CurrentColor=0;
		Bar(16*17,176,16,16);
		PutMtb(16*17,176,monsters[curr_monstr].lx,monsters[curr_monstr].ly,min(monsters[curr_monstr].lx,16),min(monsters[curr_monstr].ly,16),monsters[curr_monstr].stspr);
	} else {
		CurrentColor=3;
		Bar(16*17,176,16,16);

		//отрисуем выбранные элементы
		CurrentColor=15;
		if ( st_fon ) {
			PutImg16(0,0,fon+(curr_fon)*256);
		} else {
		  Bar(0,0,16,16);
		}
		if ( st_trase ) {
		   PutImg16(0,176,trase+(curr_trase)*256);
		} else {
		  Bar(0,176,16,16);
		}

		if ( st_items ) {
			if ( curr_items==0 ) {
				CurrentColor=0;
				Bar(16*17,0,16,16);
			} else {
				PutImg16(16*17,0,items+((curr_items-1)*8)*256);
			}
		} else {
			Bar(16*17,0,16,16);
		}
	}
	m.on();
}

int yesno(char *t) {
	Panel mp;
	int x,y,lx,ly,i;
	ly=30;
	lx=famestrlen((unsigned char *)t)+12;
	x=(MaxX-MinX-lx)/2+MinX;
	y=(MaxY-MinY-ly)/2+MinY;
	m.off();
	WOpen(x,y,lx,ly);

	mp.init();
	mp.add(x+(lx-36)/2+18,y+18,10,8,"Нет");
	mp.add(x+(lx-36)/2,y+18,10,8,"Да");
	MoveXY(x+6,y+6);
	vputs(t);
	m.on();
	while (1) {
		i=mp.press();

		if ( i>=0 ) {
			WClose();
			return i;
		}

	}
}

void f_quit(void) {
	if (yesno("ВЫ ХОТИТЕ ВЫЙТИ В DOS ?")) {
		//выход в ДОС
		fatalerror("Всего хорошего!");
	}
}

void f_gamedemo(void) {
	demo=1;
	f_game();
	demo=0;
}

void f_disk(void) {
	Panel p;
	WOpen(235,20,65,80);
	p.add(243,23,20,8,"N УРОВНЯ");
	p.add(243,43,20,8,"Считать");
	p.add(243,63,20,8,"Записать");
	p.add(243,83,20,8,"Выход");
	while ( p.press()<0 );
	if ( p.last==0 ) { //изменить номер уровня
		m.off();
		WOpen(220,40,80,15);
		MoveXY(225,45);
		vputs("N уровня:");
		sprintf(levnum,"%02d",curr_level);
		GetString(levnum,2);
		curr_level=atoi(levnum);
		WClose();
		m.on();
                for (int i=0; i<=8; ++i) {
			files[i][0]=levnum[0];
			files[i][1]=levnum[1];
		}
	} else
	if ( p.last==1 ) { //читать
		if (yesno("ВЫ ХОТИТЕ ЗАГРУЗИТЬ УРОВЕНЬ ?")) {
			readlev();
		}
	} else
	if ( p.last==2 ) { //записать
		if (yesno("ВЫ ХОТИТЕ ЗАПИСАТЬ УРОВЕНЬ ?")) {
			savelev();
		}
	}
	WClose();
	ris_lab(xw,yw);
}

void paramlev(void) {
	m.off();
	#define psx 135
	#define psy 20
	WOpen(psx,psy,160,110);
	MoveXY(psx+2,psy+2);
	vputs("ИМЯ:");
	sprintf(str0,"%-s",leveldef.name);
	GetString(str0,15);
	strcpy(leveldef.name,str0);

	MoveXY(psx+2,psy+12);
	vputs("ТИП:");
	sprintf(str0,"%01d",leveldef.typ);
	GetString(str0,1);
	leveldef.typ=atoi(str0);

	MoveXY(psx+2,psy+22);
	vputs("LX:");
	sprintf(str0,"%4d",leveldef.lx=siz_xlev);
	GetString(str0,4);
	siz_xlev=leveldef.lx=atoi(str0);

	MoveXY(psx+60,psy+22);
	vputs("LY:");
	sprintf(str0,"%4d",leveldef.ly=siz_ylev);
	GetString(str0,4);
	siz_ylev=leveldef.ly=atoi(str0);

	//размер уровня
	siz_level=(unsigned long)siz_xlev*siz_ylev;

	MoveXY(psx+2,psy+32);
	vputs("R:");
	sprintf(str0,"%3d",leveldef.fonR);
	GetString(str0,3);
	leveldef.fonR=atoi(str0);
	MoveXY(psx+42,psy+32);
	vputs("G:");
	sprintf(str0,"%3d",leveldef.fonG);
	GetString(str0,3);
	leveldef.fonG=atoi(str0);
	MoveXY(psx+82,psy+32);
	vputs("B:");
	sprintf(str0,"%3d",leveldef.fonB);
	GetString(str0,3);
	leveldef.fonB=atoi(str0);

	MoveXY(psx+2,psy+42);
	vputs("ПОСЛЕДНИЙ?:");
	sprintf(str0,"%01d",leveldef.end);
	GetString(str0,1);
	leveldef.end=atoi(str0);

	MoveXY(psx+2,psy+52);
	vputs("ИМЯ ФОНА:");
	sprintf(str0,"%-s",leveldef.fonname);
	GetString(str0,12);
	strcpy(leveldef.fonname,str0);
	MoveXY(psx+2,psy+62);
	vputs("ЗДН.ПЛН:");
	sprintf(str0,"%-s",leveldef.bckgname);
	GetString(str0,12);
	strcpy(leveldef.bckgname,str0);

	MoveXY(psx+2,psy+72);
	vputs("МУЗЫКА:");
	sprintf(str0,"%-s",leveldef.musicname);
	GetString(str0,12);
	strcpy(leveldef.musicname,str0);

	MoveXY(psx+2,psy+82);
	vputs("МУЛЬТ НАЧ:");
	sprintf(str0,"%-s",leveldef.mname0);
	GetString(str0,12);
	strcpy(leveldef.mname0,str0);

	MoveXY(psx+2,psy+92);
	vputs("МУЛЬТ КОН:");
	sprintf(str0,"%-s",leveldef.mname1);
	GetString(str0,12);
	strcpy(leveldef.mname1,str0);

	WClose();
	ris_lab(xw,yw);
	m.on();
}

void f_clear(void) {
	if ( yesno("ОЧИСТИТЬ ТЕКУЩИЙ УРОВЕНЬ ?") ) {
		_fmemset(level,0,siz_level);
		_fmemset(blevel,0,siz_level);
	}
	if ( yesno("УДАЛИТЬ МОНСТРОВ ?") ) {
		for (int i=0; i<maxobj; ++i ) {
			kobj[i].x=kobj[i].y=0;
			kobj[i].n=-1;
		}
	}
	if ( yesno("УДАЛИТЬ МОДЕЛИ ?") ) {
		for (int i=0; i<maxmodel; ++i ) {
			models[i].typ=-1;
		}

	}
	ris_lab(xw,yw);
}

void but_up(void) {
	if (yw>0) --yw;
	ris_lab(xw,yw);
}
void but_down(void) {
	if ( yw<siz_ylev-10 ) ++yw;
	ris_lab(xw,yw);
}
void but_left(void) {
	if (xw>0) --xw;
	ris_lab(xw,yw);
}
void but_right(void) {
	if ( xw<siz_xlev-16 ) ++xw;
	ris_lab(xw,yw);
}

void f_model(void) {
	//установка лифтов и др. механизмов
}

void ris_lab(int x, int y) {
	static int xww,yww,i;
	static unsigned int teki;
	static unsigned int poslab;
	poslab=y*siz_xlev+x;
	m.off();
	for ( yww=16; yww<=16*10; yww+=16 ) {
		for ( xww=16; xww<=16*16; xww+=16 ) {
			PutImg16(xww,yww,fon+*(level+poslab)*256);
			if ( st_trase && *(blevel+poslab) ) {
				PutMas16(xww,yww,trase+(*(blevel+poslab)&31)*256);
			}
			if ( st_items && (teki=*(blevel+poslab))>31 ) { //вывод вещи
				PutMas16(xww,yww,items+(unsigned long)(2048L*((teki>>5)-1)) );
			}
			++poslab;
		}
		poslab=poslab+siz_xlev-16;
	}
	Clip(16,16,16*17-1,16*11-1);

	//вывод Пуха
	PutCMas(leveldef.nx-(x*16)+16,leveldef.ny-(y*16)+16-man.ly,man.lx,man.ly,man.stspr);

	for (i=0; i<maxmodel; ++i) {
		if (models[i].typ>=0) {
			//если модель включена
			if ( i==curr_model ) {  //покажем текущую модель
					CurrentColor=7;
					CBar(models[i].x1-(x*16)+15,models[i].y1-(y*16)+15,models[i].lx+2,models[i].ly+2);
					CBar(models[i].x2-(x*16)+15,models[i].y2-(y*16)+15,models[i].lx+2,models[i].ly+2);
			}
			switch ( models[i].typ )
			{
				case m_lift0:
				case m_lift:
					CurrentColor=20;
					CBar(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16,models[i].lx,models[i].ly);
					CurrentColor=25;
					CBar(models[i].x2-(x*16)+16,models[i].y2-(y*16)+16,models[i].lx,models[i].ly);
					break;

				case m_hole:
				case m_brick:
				case m_copy:
					CurrentColor=33;
					CBar(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16,models[i].lx,1);
					CBar(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16+models[i].ly,models[i].lx,1);
					CurrentColor=37;
					CBar(models[i].x2-(x*16)+16,models[i].y2-(y*16)+16,models[i].lx,1);
					CBar(models[i].x2-(x*16)+16,models[i].y2-(y*16)+16+models[i].ly,models[i].lx,1);
					break;

				case m_warp:
					CurrentColor=2;
					CBar(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16,models[i].lx,1);
					CBar(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16+models[i].ly,models[i].lx,1);
					break;

				case m_door:
					PutCImg(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16,32,32,(block)modelspr+256*6+32*32*models[i].on0);
					break;

				case m_button:
					PutCMas(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16,16,16,(block)modelspr+256*2+256*models[i].on0);
					break;

				case m_switch:
					PutCMas(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16,16,16,(block)modelspr+256*4+256*models[i].on0);
					break;

				case m_keyhole:
					PutCImg(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16,16,16,(block)modelspr+256*models[i].on0);
					break;

				case m_shop:
					PutCMas(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16,32,32,(block)modelspr+256*6+32*32*2);
					break;

				case m_flag:
					PutCMas(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16,16,32,(block)modelspr+256*6+32*32*3+16*32*models[i].on0);
					break;

				case m_touch:
					CurrentColor=2;
					CBar(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16,models[i].lx,1);
					CBar(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16+models[i].ly,models[i].lx,1);
					break;

				case m_item:
					PutCMas(models[i].x1-(x*16)+16,models[i].y1-(y*16)+16,16,16,(block)additems+512*models[i].lx);
					break;
			}
		}
	}
	for (i=0; i<maxobj; ++i) {
		if (kobj[i].n>=0) {
			PutCMas(kobj[i].x-(x*16)+16,kobj[i].y-monsters[kobj[i].n].ly-(y*16)+16,monsters[kobj[i].n].lx,monsters[kobj[i].n].ly,monsters[kobj[i].n].stspr);
		}
	}

	Clip(0,0,319,199);
	m.on();
}

void switch_fon(void) {
	//вкл/выкл рисование фона
	st_fon=(st_fon?0:1);
	ris_lab(xw,yw);
	put_curr();
}

void switch_trase(void) {
	//вкл/выкл рисование трассы
	st_trase=(st_trase?0:1);
	ris_lab(xw,yw);
	put_curr();
}

void switch_items(void) {
	//вкл/выкл рисование вещей
	st_items=(st_items?0:1);
	ris_lab(xw,yw);
	put_curr();
}

void switch_monstr(void) {
	//вкл/выкл рисование монстров
	st_monstr=(st_monstr?0:1);
	ris_lab(xw,yw);
	put_curr();
}

void switch_model(void) {
	//вкл/выкл рисование монстров
	st_model=(st_model?0:1);
	ris_lab(xw,yw);
	put_curr();
}

void choose_fon(void) {
	int xww,yww,i=0;
	m.off();
	CurrentColor=1;
	Bar(16,0,16*16,16*12);
	for ( yww=0; yww<=16*11; yww+=16 ) {
		for ( xww=16; xww<=16*16; xww+=16 ) {
			PutImg16(xww,yww,fon+(i++)*256);
		}
	}
	m.on();
	do { m.status();} while ( m.left.press==0 && m.right.press==0 );
	if ( m.right.press==0 ) {
		curr_fon=(m.x-16)/16+((m.y/*-16*/)/16)*16;
		st_fon=1;
		put_curr();
	}
	do { m.status();} while ( m.left.press!=0 || m.right.press!=0 );
	ris_lab(xw,yw);
}

void choose_trase(void) {
	int xww,yww,i=0;
	m.off();
	CurrentColor=1;
	Bar(16,16,16*16,16*10);
	for ( yww=16; yww<=16*2; yww+=16 ) {
		for ( xww=16; xww<=16*16; xww+=16 ) {
			PutImg16(xww,yww,trase+(i++)*256);
		}
	}
	m.on();
	do { m.status();} while ( m.left.press==0 && m.right.press==0 );
	if ( m.right.press==0 ) {
		curr_trase=(m.x-16)/16+((m.y-16)/16)*16;
		st_trase=1;
		put_curr();
	}
	do { m.status();} while ( m.left.press!=0 || m.right.press!=0 );
	ris_lab(xw,yw);
}

void choose_items(void) {
	int xww,yww,i=0;
	m.off();
	CurrentColor=1;
	Bar(16,16,16*16,16*10);
	for ( yww=16; yww<=16*1; yww+=16 ) {
		for ( xww=16; xww<=16*8; xww+=16 ) {
			PutImg16(xww,yww,i==0?(fon):(items+(i-8)*256));
			i+=8;
		}
	}
	m.on();
	do { m.status();} while ( m.left.press==0 && m.right.press==0 );
	if ( m.right.press==0 ) {
		curr_items=(m.x-16)/16+((m.y-16)/16)*16;
		st_items=1;
		put_curr();
	}
	do { m.status();} while ( m.left.press!=0 || m.right.press!=0 );
	ris_lab(xw,yw);
}

void choose_monstr(void) {
	int xww,yww,i=0;
	m.off();
	CurrentColor=0;
	Bar(16,16,16*16,16*10);

	for ( yww=16; yww<=16*10-32; yww+=32 ) {
		for ( xww=16; xww<=16*16-48; xww+=48 ) {
			if ( i>=nmonstr ) {
				break;
			}
                        if (i&1) {
				CurrentColor=1;
				Bar(xww,yww,48,32);
			}
			PutMtb(xww+1,yww+1,monsters[i].lx,monsters[i].ly,min(monsters[i].lx,46),min(monsters[i].ly,30),monsters[i].gospr);
			MoveXY(xww,yww+32-8);
			vprint("%s",monsters[i++].name);
		}
	}
	//подсчет монстров в лабиринте
	xww=0;
	for ( i=0; i<maxobj; ++i ) {
		if ( kobj[i].n!=-1 ) ++xww;
	}
	MoveXY(20,16*10);
	vprint("Св.пам: %u, Всего %u типов из %u",(unsigned int)((320*200)-pmnspr),(unsigned int)nmonstr,(unsigned int)maxmonstr);
	MoveXY(20,16*10+8);
	vprint("Активно %u монстров из %u возможных",(unsigned int)xww,(unsigned int)maxobj);

	m.on();
	do { m.status();} while ( m.left.press==0 && m.right.press==0 );
	if ( m.right.press==0 ) {
		curr_monstr=(m.x-16)/48+((m.y-16)/32)*5;
		if ( curr_monstr<nmonstr ) {
			st_monstr=1;
		} else {
			curr_monstr=0;
		}
		put_curr();
	}
	do { m.status();} while ( m.left.press!=0 || m.right.press!=0 );
	ris_lab(xw,yw);
}

void choose_model(void) {
	//установка и редактирование моделей
	Panel p;
	int i,j=0;

	CurrentColor=0;
	WPut(16,16,16*16,16*10);
	show_model(curr_model);
	//----
	p.add(19,86,20,8,"<---",NULL,NULL);
	p.add(19+26,86,20,8,"РЕДАКТ",NULL,NULL);
	p.add(19+26+40,86,20,8,"--->",NULL,NULL);
	p.add(19,96+10,20,8,"УДАЛИТЬ",NULL,NULL);
	p.add(19,116+10,72,8,"НАЧАЛО координат",NULL,NULL);
	p.add(19+100,116+10,66,8,"КОНЕЦ координат",NULL,NULL);
	p.add(220,156,20,8,"Выход",NULL,NULL);

	//подсчет моделей в лабиринте
	for ( i=0; i<maxmodel; ++i ) {
		if ( models[i].typ!=-1 ) ++j;
	}
	MoveXY(20,16*10);
	vprint("Св.пам: %lu байт",(unsigned long) coreleft());
	MoveXY(20,16*10+8);
	vprint("Активно %u моделей из %u возможных",j,maxmodel);

	p.last=-1;
	while ( p.last!=6 && st_model<=0) {
		p.press();
		switch ( p.last )
		{
			case 0:
				if ( --curr_model<0 ) {
					curr_model=maxmodel-1;
				}
				show_model(curr_model);
				break;
			case 1:
				edmodel(curr_model);
				show_model(curr_model);
				break;
			case 2:
				if ( ++curr_model>=maxmodel ) {
					curr_model=0;
				}
				show_model(curr_model);
				break;
			case 3:
				//models[curr_model].name[0]=
				//models[curr_model].name0[0]=0;
				_fmemset(&models[curr_model],0,(unsigned long)sizeof(models[0]));
				models[curr_model].typ=-1;
				show_model(curr_model);
				break;
			case 4:
				st_model=1;
				break;
			case 5:
				st_model=2;
				break;
		}

        }
	ris_lab(xw,yw);
}

void show_model(int n) {
	#define omx 32
	#define omy 32
	m.off();
	char_bkgd=218;
	models[n].name0[7]=models[n].name[7]=0;
	MoveXY(omx,omy);
	vprint("#%02d '%8s' [%4s] ТИП: ",n,models[n].name,models[n].on0?"ВКЛ":"ВЫКЛ");
	switch ( models[n].typ )
	{
		case -1:
			vprint("УДАЛЕН ");
			break;

		case m_lift0:
			vprint("ЛИФТ ->");
			models[n].lx=max(models[n].lx,8);
			models[n].ly=max(models[n].ly,1);
			break;

		case m_lift:
			vprint("ЛИФТ <>");
			models[n].lx=max(models[n].lx,8);
			models[n].ly=max(models[n].ly,1);
			break;

		case m_door:
			vprint("ДВЕРЬ  ");
			models[n].lx=models[n].ly=32;
			break;

		case m_button:
			vprint("КНОПКА ");
			models[n].lx=models[n].ly=16;
			break;

		case m_switch:
			vprint("ПЕРЕКЛ.");
			models[n].lx=models[n].ly=16;
			break;

		case m_keyhole:
			vprint("ЗАМОК  ");
			models[n].lx=models[n].ly=16;
			break;

		case m_hole:
			vprint("ДЫРЫ   ");
			models[n].lx=max(models[n].lx,1);
			models[n].ly=max(models[n].ly,1);
			break;

		case m_brick:
			vprint("СТЕНКА ");
			models[n].lx=max(models[n].lx,1);
			models[n].ly=max(models[n].ly,1);
			break;

		case m_warp:
			vprint("ЛАЗ    ");
			models[n].lx=models[n].ly=32;
			break;

		case m_shop:
			vprint("МАГАЗИН");
			models[n].lx=models[n].ly=32;
			break;

		case m_touch:
			vprint("ЛОВУШКА");
			models[n].lx=max(models[n].lx,8);
			models[n].ly=max(models[n].ly,8);
			break;

		case m_flag:
			vprint("ФЛАГ   ");
			models[n].lx=16; models[n].ly=32;
			break;

		case m_copy:
			vprint("КОПИРОВ");
			models[n].lx=max(models[n].lx,1);
			models[n].ly=max(models[n].ly,1);
			break;

		case m_item:
			vprint("Суп.ВЕЩЬ");
			break;

	}
	MoveXY(omx,omy+10);
	vprint("[LX %04d, LY %04d] ->'%8s'",models[n].lx,models[n].ly,models[n].name0);
	MoveXY(omx,omy+20);
	vprint("[X1 %04d, Y1 %04d] [X2 %04d, Y2 %04d]",models[n].x1,models[n].y1,models[n].x2,models[n].y2);
	MoveXY(omx-12,omy+30);
	vprint("0,1:Лифт 2,8:Дверь 3:Кнопка 4:Рубильник 5:Замок");
	MoveXY(omx-12,omy+36);
	vprint("6:Дыры 7:Стены 9:Магазин 10:Ловушка 11:Флаг");
	MoveXY(omx-12,omy+42);
	vprint("12:Копирование 13:Супер-Вещь");
	char_bkgd=0;
	m.on();

}

void edmodel(int n) {
	m.off();
	#define ssx 45
	#define ssy 90
	WOpen(ssx,ssy,120,40);
	MoveXY(ssx+2,ssy+2);
	vputs("ИМЯ:");
	sprintf(str0,"%-s",models[n].name);
	GetString(str0,7);
	strcpy(models[n].name,str0);

	MoveXY(ssx+2,ssy+12);
	vputs("ВКЛ:");
	sprintf(str0,"%1d",models[n].on0);
	GetString(str0,1);
	models[n].on0=atoi(str0);

	MoveXY(ssx+50,ssy+12);
	vputs("ТИП:");
	sprintf(str0,"%02d",models[n].typ);
	GetString(str0,2);
	models[n].typ=atoi(str0);

	MoveXY(ssx+2,ssy+22);
	vputs("LX:");
	sprintf(str0,"%3d",models[n].lx);
	GetString(str0,3);
	models[n].lx=atoi(str0);

	MoveXY(ssx+60,ssy+22);
	vputs("LY:");
	sprintf(str0,"%3d",models[n].ly);
	GetString(str0,3);
	models[n].ly=atoi(str0);

	MoveXY(ssx+2,ssy+32);
	vputs("ССЫЛКА НА:");
	sprintf(str0,"%-s",models[n].name0);
	GetString(str0,7);
	strcpy(models[n].name0,str0);

	WClose();
	m.on();
}


void f_map(void) {
	static int x,y;

	CurrentColor=1;
	Bar(16,16,16*16,16*10);
	for ( y=0; y<siz_ylev; ++y )
	for ( x=0; x<((siz_xlev<16*16)?siz_xlev:16*16); ++x ) {
		CurrentColor=(*(blevel+x+y*siz_xlev));
		PutPixel(x+16,y+16);
	}
	if ( xw<16*16 && yw<16*10 ) {
		CurrentColor=7;
		Rectangle(xw+16,yw+16,16,10);
	}
	do { m.status();} while ( m.left.press==0 && m.right.press==0 );
	if ( m.right.press==0 ) {
		xw=min(siz_xlev-16,max(m.x-16,0));
		yw=min(siz_ylev-10,max(m.y-16,0));
	}
	do { m.status();} while ( m.left.press!=0 || m.right.press!=0 );
	ris_lab(xw,yw);
}

void f_poohxy(void) {
	do { m.status();} while ( m.left.press==0 && m.right.press==0 );
	if ( m.right.press==0 ) {
        	leveldef.nx=min(xw*16+m.x-16,siz_xlev*16); leveldef.ny=min(yw*16+m.y-16,siz_ylev*16);
	}
	do { m.status();} while ( m.left.press!=0 || m.right.press!=0 );
	ris_lab(xw,yw);
}

#else
#endif

#define asdvig 128	//для англ букв 65!
void makepass(int n) {
	//создать пароль по N уровня + деньги
	unsigned int i,j=0,k,r=random(10);
	k=(unsigned int)min(coins+(continues+glass+kolun+shboots*2+jetpack*2)*100+beams*5+shields*50,1020)>>2;
	i=stroka[0]=r+asdvig;	//смещение
	stroka[1]=i+(k&15);	//младш знач для денег
	stroka[2]=i+((k>>4)&15);//старш знач для денег
	stroka[3]=i+n;	//уровень
	for ( i=0; i<4; ++i ) {
		j+=(unsigned char)(stroka[i]);
	}
	stroka[4]=(j&15)+asdvig;	//контрольная сумма
	stroka[5]=0;
}

int testpass(void) {
	//проверка верности пароля
	unsigned int i,j=0;
	for ( i=0; i<4; ++i ) {
		j+=(unsigned char)(stroka[i]=CharUp(stroka[i]));
	}
	if ( ((j&15)+asdvig)==(unsigned char)CharUp(stroka[4]) ) {
		coins=((unsigned char)stroka[1]-(unsigned char)stroka[0])|(((unsigned char)stroka[2]-(unsigned char)stroka[0])<<4)<<2;
		return ((unsigned char)stroka[3]-(unsigned char)stroka[0]);
	}
	//пароль не верен
	coins=0;
	return -1;
}

void fatalerror(char *t) {
	//выход по фатальной ошибке
	if ( dev!=255 ) {
		//выкл муз
		modstop();
	}
	SetLib("");
	CloseGraph();
	puts(t);
	kb_off();
	exit(1);
}

void readstrip(void) {
	//считывание описания для scroll-strip
	// 01strip.def
	int i,n;
	block pos=tmp;

	striply=nstrip=0;

	for (i=0; i<maxstrip; ++i ) {
		//считаем очередн строку
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
	striply-=168;
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

//void do_strip(int sx=0) {
void do_strip(int sx) {
	int i;
	sx=min(sx,319);
	for ( i=0; i<nstrip; ++i ) {
		switch ( strip[i].typ ) {
			case st_roll:
				strip[i].x-=(strip[i].sx-sx/4);
			case st_still:
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
				break;
		}
		if ( strip[i].x>319 ) {
			strip[i].x-=320;
		} else if ( strip[i].x<0 ) {
			strip[i].x+=320;
		}
	}
}

void addmess(char *m,unsigned char w) {
	waitmess=w; mess=m;
	messy=176; messsy=-5;
}

//модуль собственно game
void actmodel(void) {
	static int i,j;
	if (man.myrg==0) //если мужик не в шоке, то
	for ( i=0; i<maxmodel; ++i ) {
		//проверка на соприкосновение с моделью
		if ( models[i].typ>=0 ) {
			if ( man.x+(man.upx1+man.upx2)/2>=models[i].x && man.x+(man.upx1+man.upx2)/2<=models[i].x+models[i].lx
				&&  man.y-man.upy<models[i].y+models[i].ly && man.y-man.upy>models[i].y ) {
				if( models[i].typ>=0 && models[i].on ) {
					if ( stricmp("EXIT",models[i].name0)==0 ) {
						//конец уровня!!!!
						man.s=s_end;
						zend=1;
						//т.к. выод то пока!
						break;
					} else
					if ( stricmp("BOSS",models[i].name0)==0 ) {
						//BOSS уровня!!!!
						boss=1;
						if ( dev!=255 ) {
							modstop();
							modsetup("boss.fms", 4, 0 ,mix, dev, &state );
							modvolume(vol,vol,vol,vol);
						}
						addmess("БОСС УРОВНЯ",40);
						man.myrg=wmyrg;
					}
				}
				switch ( models[i].typ ) {
				case m_door:
				case m_warp:
					//ДВЕРЬ
					if ( models[i].on ) {
						//телепортация
						man.x=models[i].x2;
						man.y=models[i].y2;
						man.sx=man.sy=man.dsx1=man.dsx2=0;
						man.s=s_ouch;
						addboom(man.x,man.y-man.ly,32,32,0,0,b_boom,obloka+(10*256+32*32*5));
                                	        man.myrg=wmyrg;
					}
					break;

				case m_button:
					//КНОПКА
					if ( models[i].on ) {
						for ( j=0; j<maxmodel; ++j ) {
							if ( stricmp(models[i].name0,models[j].name)==0 ) {
								models[j].on=1;
								models[i].on=0;
								man.myrg=wmyrg;
								addmess("Чпок",30);
							}
						}
					}
					break;

				case m_switch:
					//ПЕРЕКЛ
					if ( models[i].on ) {
						models[i].on=0;
					} else {
						models[i].on=1;
					}
					for ( j=0; j<maxmodel; ++j ) {
						if ( stricmp(models[i].name0,models[j].name)==0 ) {
							models[j].on=models[i].on;
							man.myrg=wmyrg;
							addmess("Щелк",30);
						}
					}
					break;

				case m_keyhole:
					//ЗАМОК
					if ( models[i].on && keys>0 ) {
						--keys;
						for ( j=0; j<maxmodel; ++j ) {
							if ( stricmp(models[i].name0,models[j].name)==0 ) {
								models[j].on=1;
								models[i].on=0;
								man.myrg=wmyrg;
								addmess("Клик",30);
							}
						}
					}
					break;

				case m_shop:
					//магазин
					doshop(atoi(models[i].name0));
					man.myrg=wmyrg;
					break;
				}
			}
		}
	}
}

void open_exit(void) {
	static int j;
	for ( j=0; j<maxmodel; ++j ) {
		if ( stricmp("EXIT",models[j].name)==0 ) {
			models[j].on=1;
			man.myrg=wmyrg;
			addmess("ВЫХОД ОТКРЫТ",60);
		}
	}
}

/*
void splitcopy0(screen dest, int sx, int sy, screen src) {
	//вывод блока со сдвигом
	static int lx1,lx2;
	if ( sx+16*16>=320 ) {
		lx2=16*16-(lx1=320-sx);
	} else {
		lx2=16*16-(lx1=16*16);
	}

	CopyBlock(16, 16, lx1, 10*16, dest, sx, sy, src);
	if ( lx2 )
		CopyBlock(16+lx1, 16, lx2, 10*16, dest, 0, sy, src);
}*/

void far mm(unsigned char butt) {
	//обработка прерывания по клавиатуре
	register char c;
	if ( butt&128 ) {
		c=0;
		butt=butt&127;
	} else {
		c=1;
	}

	if ( butt==1 ) //Esc
		zend=c;
	if ( butt==77 )
		t_right=c;
	if ( butt==75 )
		t_left=c;
	if ( butt==29 ) //Ctrl
		t_jump=c;
	if ( butt==72 )
		t_up=c;
	if ( butt==80 )
		t_down=c;
	if ( butt==56 || butt==57) //alt spc - 57
		t_fire=c;
	if ( butt==25 )
		pause=c;
	if ( butt==33 )
		key_f=c; //фон
	if ( butt==48 )
		key_b=c; //BOSS-key
}

void far mz(unsigned char butt) {
	//обработка прерывания по клавиатуре
	if ( butt==1 ) //Esc
		zend=1;
	if ( butt==77 )
		right=1;
	if ( butt==75 )
		left=1;
	if ( butt==72 )
		up=1;
	if ( butt==80 )
		down=1;
	if ( butt==56 || butt==57 || butt==28) //alt spc entr
		fire=1;
	if ( butt==29 ) //ctrl
		jump=1;
}

void far md(unsigned char butt) {
	//обработка прерывания по клавиатуре
	//для демонстрации
	zend=1;
}

//=============================================================

void f_game(void) { //проба скролла и объектов
	static int x,y,sx,sy,i,scrsdvig,scrsdvig_;
	kb_set(mm);

#ifdef EDITOR
	m.off();
#endif

	if ( demo ) {
		//для демонструхи другой обработч клавишь
		kb_set(md);
	} else {
		PaletteOff(palette);
	}

	SetLib("graph");
	GetLib("normal.col",palette);

	shields=kolun=glass=shboots=jetpack=zend=score=0; //монетки,продолжения
	continues=1; //продолжения
	maxjump=-8; //высота прыжка
	maxspeed=4; //макс. скорость

	while ( continues-- ) {
		man.s=s_end;
		hearts=3; //сердечки
		beams=5; //ШИШКИ
	while ( hearts>0 ) {
		//пока не кончились жизни
		//считаем уровень
		if ( man.s==s_end ) {
			//если грузим уровень, то...
			//--- считаем заголовок уровня
			SetLib("levels");
			sprintf(levnum,"%02d",curr_level);
			for (i=0; i<=8; ++i) {
				files[i][0]=levnum[0];
				files[i][1]=levnum[1];
			}
			GetLib(f_def,(block)hidscr);
			_fmemcpy(&leveldef,(block)hidscr,(unsigned long)sizeof(leveldef));
			//------------------
			//readlev();
			SetLib("graph");
			GetLib("normal.col",palette);
			GetLib("tiles.bin",(block)hidscr);
			NormalScreen();
			Clip(0,0,319,199);
			TileBar(0,0,320,200,64,64,(block)hidscr+random(4)*64*64);
			MoveXY(74,20);
			vprintB(tfnt+144*1,"УРОВЕНЬ %u",curr_level);
			MoveXY((320-famestrlen((unsigned char *)leveldef.name)*3)/2,90);
			vputBs(leveldef.name,tfnt+144*2);
			tic=gettic()+18*3;

			if( setup.f>1 ) {
				//bckg=famemalloc((unsigned long)320*200);
				farfree(bckg);
			}
			farfree(mnstrspr);
			farfree(level); farfree(blevel);
			if ( dev!=255 ) {
				//вкл музыку
				modsetup(leveldef.musicname, 4, 0 ,mix, dev, &state );
				modvolume(vol,vol,vol,vol);
			}
			PaletteOn(palette);

			zend=0;while (gettic()<tic && zend==0);

			PaletteOff(palette);

			if ( demo==0 ) {
				SetLib("mult"); //мульт на начало уровня
				mult(leveldef.mname0);
			}
			if( setup.f>1 ) {
				bckg=famemalloc((unsigned long)320*200);
				//farfree(bckg);
			}
			mnstrspr=(block)famemalloc((unsigned long)320*200);
			level=(block)famemalloc(maxsiz_level);
			blevel=(block)famemalloc(maxsiz_level);

			if ( dev!=255 ) {
				//выкл муз
				modstop();
			}

			readlev();
			SetLib("graph");
			SetScreen(hidscr);
			f_rama[4]=levnum[0];
			f_rama[5]=levnum[1];
			GetLib(f_rama,(block)hidscr);
			CopyToScreen(hidscr);

			for ( i=0; i<maxmodel; ++i ) {
				if ( models[i].typ>=0 ) {
					models[i].on=models[i].on0;
					models[i].x=models[i].x1;
					models[i].y=models[i].y1;
				} else {
					models[i].on=0;
				}
			}
			//иниц в начале уровня
			for ( i=0; i<maxobj; ++i ) {
				//из массива достаем монстрят
				if ( kobj[i].n>=0 )
					makemonstr(i,kobj[i].n,kobj[i].x,kobj[i].y);
				else
					obj[i].on=0;
			}
			boss=keys=0; //ключи
		}

		waitboom=1;
		srand(20); //иниц ДСЧ
		waitmess=scrsdvig=fire=jump=down=left=right=up=0;
		t_fire=t_jump=t_down=t_left=t_right=t_up=0;
		beams=max(5,beams); //ШИШКИ

		man.x=leveldef.nx; //инициализация героя
		man.y=leveldef.ny;

		if (man.x-16*16/2+man.lx/2<=0) r_x=0;
		else if (man.x-16*16/2+man.lx/2>siz_xlev*16-16*16) r_x=siz_xlev*16-16*16;
		else r_x=man.x-16*16/2+man.lx/2;
		//центровка по Y
		if (man.y-16*10/2-16*2<=0) r_y=0;
		else if (man.y-16*10/2-16*2>siz_ylev*16-16*10) r_y=siz_ylev*16-10*16;
		else r_y=man.y-16*10/2-16*2;

		x=y=sx=sy=0;

                for ( i=0; i<maxbullet; ++i ) {
		    bullets[i].typ=-1;
                }
		for ( i=0; i<maxboom; ++i ) {
			booms[i].typ=-1;
		}

		man.on=5; //5 медок энергия
		man.buf=man.stspr;
        	man.myrg=wmyrg; //неуязвимость
		draw=man.sy=1;
		man.s=s_down;
		power=man.f=man.f0=man.fw=0;
		times=9999;
		put_score();put_additems();

		rislab(r_x,r_y); domodel(); doobj(); putup();
		CopyBlock0(hidscr);

		if ( speedraw<2 ) {
			//если отключен фон, то...
			palette[0]=leveldef.fonR;
			palette[1]=leveldef.fonG;
			palette[2]=leveldef.fonB;
		} else {
			palette[0]=palette[1]=palette[2]=0;
		}
		//инициализируем задержку по таймеру
		tic=gettic()+wclock;
		pdemo=0; //инициализируем демонстрацию

		if ( dev!=255 ) {
			modsetup(boss?"boss.fms":leveldef.musicname, 4, 0 ,mix, dev, &state );
			modvolume(vol,vol,vol,vol);
		}
		PaletteOn(palette);

		while ( zend==0 ) {

			draw=(speedraw==3 || speedraw==1 || waitboom&1)?1:0;

			//центровка по X
			if (man.x-16*16/2+man.lx/2<=0) x=0;
			else if (man.x-16*16/2+man.lx/2>siz_xlev*16-16*16) x=siz_xlev*16-16*16;
			else x=man.x-16*16/2+man.lx/2;

			//центровка по Y
			if (man.y-16*10/2-16*2<=0) y=0;
			else if (man.y-16*10/2-16*2>siz_ylev*16-16*10) y=siz_ylev*16-10*16;
			else y=man.y-16*10/2-16*2;

			//asm cli;
			left=t_left; right=t_right;
			up=t_up; down=t_down;
			jump=t_jump; fire=t_fire;
			//asm sti;

			//считаем с жустика, если он есть
			if (js_state()) {
				js_read();
			}
			/*if ( J.present ) {
				J.status();
				up=J.a.up; down=J.a.down;
				left=J.a.left; right=J.a.right;
				fire=J.a.a; jump=J.a.b;
			}*/

			if ( demo ) {
				//если демонстрация, то расшифруем
				i=demobuf[pdemo++];
				left=i&1;	right=i&2;
				up=i&4;         down=i&8;
				jump=i&16;      fire=i&32;
				zend=i&64;
				if ( pdemo>=maxdemo ) {
					zend=1;
				}
			} else {
				//иначе пишем демонстрацию
				if ( pdemo<maxdemo ) {
					demobuf[pdemo++]=left|(right<<1)|(up<<2)|(down<<3)|(jump<<4)|(fire<<5)|(zend<<6);
				}
			}

			SetScreen(hidscr);
			Clip(16,16,16*17-1,16*11-1);
			rislab(x,y);
			domodel();
			if ( demo ) {
				MoveXY(105,24);
				vputs("Демонстрация");//,tfnt+144*(waitboom&3));
			}
			doobj(); doman();
			dobullet(); putup(); doboom();
			if( waitmess>0 ) {
				waitmess--; MaxY=190;
				MoveXY(24,messy);
				if( messsy<1 || waitmess<6) {
					messy+=(++messsy);
				}
				char_fgd=12+(waitboom&3);
				vputs(mess);
			}
			if ( draw )
				CopyBlock0(hidscr);

			if (times<=0) {
				man.s=s_crash;
				times=1000;
			     	addmess("ВРЕМЯ ИСТЕКЛО",60);
			} else {
				--times;
			}
			if ( waitfire>0 ) {
				--waitfire;
			}
			put_score();

			if ( left ) { //кнопка влево
				if (man.sx>=-maxspeed) {
					//в воде
					if( leveldef.typ!=t_water || (waitboom&3)==0 ) {
						--man.sx;
					}
				}
				if ( man.sx<0 )
					man.napr=0;
			} else if (right==0 && man.sx<0) {
				//скольжение
				if( leveldef.typ!=t_ice || (waitboom&3)==0 ) {
					++man.sx;
				}
			}
			if ( right ) { //кнопка вправо
				if (man.sx<=maxspeed) {
					//в воде
					if( leveldef.typ!=t_water || (waitboom&3)==0 ) {
						++man.sx;
					}
				}
				if ( man.sx>0 )
					man.napr=1;
			} else if (left==0 && man.sx>0) {
				//скольжение
				if( leveldef.typ!=t_ice || (waitboom&3)==0 ) {
					--man.sx;
				}
			}

			if ( jump ) { //прыжок
				if ( man.s==s_go ) {
					man.sy=maxjump;
					man.s=s_jump;
				} else
				if( jetpack && power && up && man.s==s_down ) {
					//в полет
					man.s=s_fly; man.dsx1=man.dsx2=0;
				} else
				if ( man.s==s_fly && power) {
					//в добавим газку
					if( man.sy>-3 )
						--man.sy;
				}
			} else if ( man.s==s_jump ) {
				man.sy=man.sy/2;
				man.s=s_down;
			}

			if ( fire && man.s!=s_fly ) { //стрельба

				if ( waitfire==0 ) {
					waitfire=8;

					if (beams>0) {
						if ( addbullet(man.x+16,man.y-20,man.sx+(man.napr!=0?4:-4),up?-2:(down?2:0),1,items+256*4*8) ) {
							//фаза броска
							man.buf=man.stspr+4*man.lx*man.ly;
							man.fw=3;
							--beams;
						}
					}
				}
			}

			if ( up  && man.s==s_go && man.sx==0 ) { //сдвиг для просмотра вверх экрана
				actmodel(); //использ вещь

				if ( scrsdvig<16*5 ) {
					scrsdvig+=2;
					scrsdvig_=0;
				}
				if (man.y-scrsdvig-16*10/2-16*2<=0) r_y=0;
				else if (man.y-scrsdvig-16*10/2-16*2>siz_ylev*16-16*10) r_y=siz_ylev*16-10*16;
				else r_y=man.y-scrsdvig-16*10/2-16*2;
			} else {
				scrsdvig=0;
			}

			if ( down && man.s==s_go && man.sx==0 ) { //сдв вниз
				if ( scrsdvig_<16*7) {
					scrsdvig_+=2;
					scrsdvig=0;
				}
				if (man.y+scrsdvig_-16*10/2-16*2<=0) r_y=0;
				else if (man.y+scrsdvig_-16*10/2-16*2>siz_ylev*16-16*10) r_y=siz_ylev*16-10*16;
				else r_y=man.y+scrsdvig_-16*10/2-16*2;
			} else {
				scrsdvig_=0;
			}

			if ( pause ) {//пауза в игре
				NormalScreen();	i=0;
				while ( pause );
				if ( dev!=255 )
					modvolume(vol/2,vol/2,vol/2,vol/2);
				while ( pause==0 ) {
					MoveXY(109,92);
					WVR();
					vputBs("ПАУЗА",tfnt+144*i++);
					if (i>4) i=0;
					delay(100);
				}
				if ( dev!=255 )
					modvolume(vol,vol,vol,vol);
				while ( pause );
				SetScreen(hidscr);
			}

			if ( key_b ) {//BOSS-key
				if ( dev!=255 )
					modvolume(0,0,0,0);
				while ( key_b ) {
					if ( key_f ) {	//экстренный выход
						makepass(curr_level);
						fatalerror(curr_level>1?stroka:"");
					}
				};
				PaletteOff(palette);
				while ( key_b==0 );
				if ( dev!=255 )
					modvolume(vol,vol,vol,vol);
				PaletteOn(palette);
			}

			if ( key_f ) {
				//переключение скорости вывода
				if ( ++speedraw>setup.f ) {
					speedraw=0;
				}
				if ( speedraw<2 ) {
					palette[0]=leveldef.fonR;
					palette[1]=leveldef.fonG;
					palette[2]=leveldef.fonB;
				} else {
					palette[0]=palette[1]=palette[2]=0;
				}

				NormalScreen();
				putBch(136,92,'1'+speedraw,tfnt+144*speedraw);
				while ( key_f ) {
#ifndef DEMO
					if( setup.cheat ) {
						//cheat-key посмотреть смерть
						if ( pause ) {
							hearts=man.on=1;
							times=100;
						} //Жизни+$
						if ( t_left && t_right ) {
							coins=9000;
							hearts=man.on=5;
						}
#ifndef EDITOR					//на след. урвнь
						if ( t_up && t_down ) {
							man.s=s_end;
							zend=1;
						}
#endif
					}
#endif
				}
				SetScreen(hidscr);
				PutPalette(palette);
			}

			if ( man.s==s_go && man.sy<1 ) {
				//для отсутствия остановов при движении с горы
				man.sy=2;
			}

			//задержка по таймеру
			while (gettic()<tic);
			tic=gettic()+wclock;

#ifdef DEMO
			if ( times==9000 || times==7000 || times==5000) {
				NormalScreen();
				WPut(24,24,16*15,16*9);
				for( i=0;i<16; ++i) {
					SetColor(47-i);
					PutCBlink(40+i,40+i,32,32,man.stspr);
				}
				PutFont(lfonts);
				PutMas(56,56,32,32,man.stspr);
				MoveXY(130,50);
				vputs("ПОДОЖДИТЕ 15 СЕКУНД!");
				MoveXY(96,80);
				vputs("Это демонстрационная версия!");
				MoveXY(30,110);
				vputs("Автор: Михаил Братусь, тел.(351-35) 2-14-60");
				MoveXY(40,120);
				vputs("456321 г. Миасс ул. Вернадского д.4, кв.52");
				MoveXY(40,130);
				vputs("E-mail: migel@intech.telecom.chel.su");
				MoveXY(40,140);
				vputs("(c) 1996 FaMe SoftWare by BMV");
				PutFont(fonts);
				tic=gettic()+18*15;
				while (gettic()<tic);
			}
#endif

		}
		PaletteOff(palette);
		if ( dev!=255 ) {
			modstop();
		}
		//обнуляем цвет фона
		palette[0]=palette[1]=palette[2]=0;
		//если мужика пришили... то
		if ( demo ) {
			//прерываение игры
			zend=hearts=0;
		} else if ( man.s==s_over ) {
			//подох
			zend=0;

			//убавляем жизнь
			if (--hearts<=0) {
				//OVER?
				NormalScreen(); Cls(0);
				Clip(0,0,319,199);
				MoveXY(23,170);
				//k.set(md);

				SetLib("mult");
				if( setup.f>1 ) {
					//bckg=famemalloc((unsigned long)320*200);
					farfree(bckg);
				}
				farfree(mnstrspr);
				farfree(level); farfree(blevel);

				if ( dev!=255 ) {
					modsetup("boss.fms", 4, 0 ,mix, dev, &state );
					modvolume(vol,vol,vol,vol);
					zend=0;
				}

				if ( continues ) {
					tic=gettic()+18*8;
					mult("continue.rec");
					vprintB(tfnt+144*3,"Продолжение %1u",continues);
				} else {
					tic=gettic()+18*3;
					mult("theend.rec");
				}

				while (gettic()<tic && zend==0);
				if ( dev!=255 ) {
					modstop();
				}
				if( setup.f>1 ) {
					bckg=famemalloc((unsigned long)320*200);
				}
				mnstrspr=(block)famemalloc((unsigned long)320*200);
				level=(block)famemalloc(maxsiz_level);
				blevel=(block)famemalloc(maxsiz_level);

				PaletteOff(palette);
				zend=0;
			}

		} else if ( man.s==s_end ) {
			// PATCH: secrets and things counters moved here before memory will be freed
			//считаем оставш число секретов
			x=0;
			for ( i=0; i<maxmodel; ++i ) {
				if ( models[i].typ>=m_copy ) ++x;
			}
			//подсчет кол-ва вещей
			y=0;
			for ( i=0; i<siz_level; ++i ) {
				if ( blevel[i]&224 ) ++y;
			}
			SetLib("mult"); //мульт на конец уровня
			if( setup.f>1 ) {
				//bckg=famemalloc((unsigned long)320*200);
				farfree(bckg);
			}
			farfree(mnstrspr);
			farfree(level); farfree(blevel);
			if ( dev!=255 ) {
				//вкл музыку
				modsetup(leveldef.musicname, 4, 0 ,mix, dev, &state );
				modvolume(vol,vol,vol,vol);
			}
			mult(leveldef.mname1);
//-------------------------------
			SetLib("graph");
			GetLib("normal.col",palette);
			GetLib("tiles.bin",(block)hidscr);
			NormalScreen();
			Clip(0,0,319,199);
			TileBar(0,0,320,200,64,64,(block)hidscr+random(4)*64*64);
			MoveXY((320-famestrlen("ИТОГИ УРОВНЯ")*3)/2,20);
			vputBs("ИТОГИ УРОВНЯ",tfnt);
			MoveXY((320-famestrlen("СЕКРЕТЫ: %u%%")*3)/2,70);
			vprintB(tfnt+144*2,"СЕКРЕТЫ: %u%%",(int)((nsecret-x)*100/nsecret));
			MoveXY((320-famestrlen("ВЕЩИ: %u%%")*3)/2,110);
			vprintB(tfnt+144*4,"ВЕЩИ: %u%%",(int)((nitems-y)*100/nitems));
			if ( leveldef.end==0 ) {
				MoveXY(26,160);	makepass(curr_level+1);
				vprintB(tfnt+144*3,"ПАРОЛЬ: %s",stroka);
			}
			PaletteOn(palette);
			tic=gettic()+18*20;
			zend=0;
			while (gettic()<tic && zend==0);
			PaletteOff(palette);
//-----------------------------
			if ( dev!=255 ) {
				//выкл муз
				modstop();
			}
			if( setup.f>1 ) {
				bckg=famemalloc((unsigned long)320*200);
				//farfree(bckg);
			}
			mnstrspr=(block)famemalloc((unsigned long)320*200);
			level=(block)famemalloc(maxsiz_level);
			blevel=(block)famemalloc(maxsiz_level);
			//на следующь уровень

			if ( leveldef.end==0 ) {
				//инкремент уровня
				++curr_level;
				zend=shields=0; //убираем щит, если есть
#ifdef DEMO
				if( curr_level>2 ) {
					SetLib("graph");
					mult("author.rec");
					continues=end=hearts=0;
				}
#endif
				//проверка на конец
			} else {
				//конец игры
				SetLib("mult");
				if( setup.f>1 ) {
					//bckg=famemalloc((unsigned long)320*200);
					farfree(bckg);
				}
				farfree(mnstrspr);
				farfree(level); farfree(blevel);
				if ( dev!=255 ) {
					modsetup("coming.fms", 4, 0 ,mix, dev, &state );
					modvolume(vol,vol,vol,vol);
				}
				mult("youwin.rec");
				SetLib("graph");
				mult("author.rec");
				if ( dev!=255 ) {
					//выкл муз
					modstop();
				}
				if( setup.f>1 ) {
					bckg=famemalloc((unsigned long)320*200);
					//farfree(bckg);
				}
				mnstrspr=(block)famemalloc((unsigned long)320*200);
				level=(block)famemalloc(maxsiz_level);
				blevel=(block)famemalloc(maxsiz_level);
#ifdef EDITOR
				//GetLib("normal.col",palette);
				SetScreen(hidscr);
#endif
				continues=zend=hearts=0;
			}
		} else {
			//прерываение игры
			continues=zend=hearts=0;
		}
	} // для hearts
	} //это для contin
	NormalScreen();

#ifdef EDITOR
	if ( demo==0 ) { //допишем EOF в демо
		demobuf[pdemo-1]=1<<6; //клавиша ESC
		SetLib("levels");
		PutLib(f_demo,(block)demobuf,maxdemo);
		GetLib(f_demo,(block)demobuf);
	}
	readlev(); Cls(28); ris_lab(xw,yw); PaletteOn(palette);
    m.on();
#endif
}

char check(register int x_,register int y_) { //выод кода блока по координатам
	register int sdv;
	// PATCH: crashed here on level1 (just in case, patch too)
	x_ = (x_ < 0) ? 0 : x_;
	y_ = (y_ < 0) ? 0 : y_;
	//разделим координаты
	sx=(x_&0x0f); x=x_>>4; sy=(y_&0x0f); y=y_>>4;
	//позицию первую вычислим
	tekblock=*(poslab=(unsigned char far *)blevel+(sdv=x+y*siz_xlev));
	poslal=(unsigned char far *)level+sdv;

	return (tekblock&31);
}

char checkdw(register int x_,register int y_) {
	//вывод кода блока по координатам с коррекцией на пол
	register int sdv;
	// PATCH: crashed here on level1 (just in case, patch too)
	x_ = (x_ < 0) ? 0 : x_;
	y_ = (y_ < 0) ? 0 : y_;
	//разделим координаты
	sx=(x_&0x0f); x=x_>>4; sy=(y_&0x0f); y=y_>>4;
	//позицию первую вычислим
	tekblock=*(poslab=(unsigned char far *)blevel+(sdv=x+y*siz_xlev));
	poslal=(unsigned char far *)level+sdv;

	if ( sy>=poc[(tekblock&31)][sx] )
		return (tekblock&31);
	return 0;
}

int addbullet(int x, int y, int sx, int sy, char typ, block buf) {
	//добавление пули в очередь
	static int i;
	for ( i=0; i<maxbullet; ++i ) {
		if ( bullets[i].typ<0 ) {
			bullets[i].x=x;
			bullets[i].y=y;
			bullets[i].sx=sx;
			bullets[i].sy=sy;
			bullets[i].typ=typ;
			bullets[i].buf=buf;
			bullets[i].f=0;
			return 1;
		}

	}
	return 0;
}

void dobullet(void) {
	//вывод и обработка патронов
	static int i,t;
	static strbullets *bul;
	for ( i=0; i<maxbullet; ++i ) {
		if ( bullets[i].typ>=0 ) {
			bul=&bullets[i];
			//сторкнулся ли патрон с чем?
			t=check((bul->x+=bul->sx),(bul->y+=bul->sy));
			if ( (t!=0 && t!=23) || bul->y>=siz_ylev*16) {
				check((bul->x-bul->sx),(bul->y-bul->sy));
				//если шишка упала, то пусть лежит
				if ( *poslab==0 && bul->typ==1) {
					if ( bul->sx ) {
						bul->typ=-1;
						addbullet(bul->x-bul->sx,bul->y-bul->sy,0,bul->sy+1,1,items+256*4*8);
					} else {
						*poslab=5<<5;
					}
				} else {
					if( bul->typ<8 || bul->sy>=0 ) {
						bul->typ=-1;
					}
				}
				if ( bul->typ==-1 ) {
					addboom(bul->x-8,bul->y-8,16,16,0,0,b_boom,obloka+256*15);
				} else {
					addboom(bul->x-8,bul->y-8,16,16,0,0,b_boom,obloka);
				}
			}
			if( bul->typ>=8 ) {	//снаряды врага
				if ( draw )
					PutCImg(bul->x-r_x+8,bul->y-r_y+8,16,16,bul->buf);
                        	if (waitboom&1) {
					if ( ++bul->sy>16 || bul->y>siz_ylev*16 ) {
						bul->typ=-1;
					}
        	                }
			} else {	//призы или шишки
				if ( draw )
					PutCMas(bul->x-r_x+8,bul->y-r_y+8,16,16,bul->buf+256*bul->f);
				if ( ++bul->f>7 ) {
					bul->f=0;

					if ( bul->sx>0 )
						--bul->sx;
					else if ( bul->sx<0 )
						++bul->sx;

					if (++bul->sy>6) {
						bul->sy=6;
					}
				}
			}

		}

	}
}

void domodel(void) {
	//вывод и обработка патронов
	static int i,j,t,t0,x1,y1;
	static strmodels *md;
	for ( i=0; i<maxmodel; ++i ) {
		if ( models[i].typ>=0) {
			//если модель включена
			md=&models[i];
			switch ( md->typ ) {
				case m_lift0:
				case m_lift:
					md->sx=md->sy=0;
					if ( md->on ) {
						if ( md->x<md->x2 ) {
							md->sx=1;
						} else if ( md->x>md->x2 ) {
							md->sx=-1;
						} else {
							md->sx=0;
						}
						if ( md->y<md->y2 ) {
							md->sy=1;
						} else if ( md->y>md->y2 ) {
							md->sy=-1;
						} else {
							md->sy=0;
						}
						md->x+=md->sx;
						md->y+=md->sy;
						if ( md->typ==m_lift && md->sy==0 && md->sx==0) {
							SwapShort(&md->x2,&md->x1);
							SwapShort(&md->y2,&md->y1);
						}
                                        }
					PutCImg(md->x-r_x+16,md->y-r_y+16,md->lx,md->ly,lifts);
					break;

				case m_door:
					PutCImg(md->x-r_x+16,md->y-r_y+16,32,32,(block)modelspr+256*6+32*32*md->on);
					break;

				case m_warp:
					if( glass && (waitboom&31)==3 ) {
						addboom(md->x+16,md->y+16,8,8,0,-5,b_buh,obloka+256*10+32*32*10+128*5+64*2);
					}
					break;

				case m_shop:
					PutCMas(md->x-r_x+16,md->y-r_y+16,32,32,(block)modelspr+256*6+32*32*2);
					break;

				case m_flag:
        				if ( man.x+(man.dwx1+man.dwx2)/2>=md->x && man.x+(man.dwx1+man.dwx2)/2<=md->x+16
						&&  man.y<=md->y+48 && man.y>=md->y ) {
						if ( md->on ) {
								md->on=0;
								leveldef.nx=man.x; leveldef.ny=man.y-8;
								addboom(md->x,md->y,16,16,0,0,b_boom,obloka+256*5*4);
								addmess("Запомню место...",40);

						}
					}
					PutCMas(md->x-r_x+16,md->y-r_y+16,16,32,(block)modelspr+256*6+32*32*3+32*16*md->on);
					break;

				case m_touch:
        				if ( man.x+(man.dwx1+man.dwx2)/2>=md->x && man.x+(man.dwx1+man.dwx2)/2<=md->x+md->lx
		        			&&  man.y<=md->y+md->ly && man.y>=md->y ) {
						if ( md->on ) {
							//PVOICE_START(voc07,lv07,0,0,0,0L);
							for ( j=0; j<maxmodel; ++j ) {
								if ( stricmp(md->name0,models[j].name)==0 ) {
									models[j].on=1;
									md->on=0;
                                                                }
							}
							if ( stricmp("EXIT",md->name0)==0 ) {
								//конец уровня!!!!
								man.s=s_end;
								zend=1;
								//т.к. выод то пока!
								break;
							} else
							if ( stricmp("BOSS",md->name0)==0 ) {
								//BOSS уровня!!!!
								boss=1;
								if ( dev!=255 ) {
									modstop();
									modsetup("boss.fms", 4, 0 ,mix, dev, &state );
									modvolume(vol,vol,vol,vol);
								}
								addmess("БОСС УРОВНЯ",40);
								man.myrg=wmyrg;
							}
						}
					}
					break;

				case m_button:
					PutCMas(md->x-r_x+16,md->y-r_y+16,16,16,(block)modelspr+256*2+256*md->on);
					break;

				case m_switch:
					PutCMas(md->x-r_x+16,md->y-r_y+16,16,16,(block)modelspr+256*4+256*md->on);
					break;

				case m_keyhole:
					PutCImg(md->x-r_x+16,md->y-r_y+16,16,16,(block)modelspr+256*md->on);
					break;

				case m_hole:
				case m_brick:
					if ( md->on ) {
						if ( md->x<md->x2 ) {
							md->sx=1;
						} else if ( md->x>md->x2 ) {
							md->sx=-1;
						} else {
							md->sx=0;
						}
						if ( md->y<md->y2 ) {
							md->sy=1;
						} else if ( md->y>md->y2 ) {
							md->sy=-1;
						} else {
							md->sy=0;
						}
						md->x+=md->sx;md->y+=md->sy;
						for( y1=0; y1<=md->ly;y1+=16 )
						for( x1=0; x1<=md->lx;x1+=16 ) {
							t=check(md->x+x1,md->y+y1);
							if ( md->typ==m_hole ) {
								//взрываем
								if (t) {
									addboom(x*16/*+x1*/,y*16/*+y1*/,16,16,5-random(11),1+random(2),b_fall,fon+(*poslal)*256);
									addboom(x*16/*+x1*/,y*16/*+y1*/,16,16,0,0,b_boom,obloka+256*5*4);
									*poslab=*poslal=0;
								}
							} else {
								//ставим стенку
								if (t!=16) {
									addboom(x*16/*+x1*/,y*16/*+y1*/,16,16,0,0,b_boom,obloka+256*5*4);
									*poslab=*poslal=16;
								}
							}
						}

						if ( md->sy==0 && md->sx==0) {
							//конец работы
							for ( j=0; j<maxmodel; ++j ) {
								if ( stricmp(md->name0,models[j].name)==0 ) {
									models[j].on=1;
                                                                }
							}
							md->typ=-1;
						}
                                        }
					break;

				case m_copy:
					if ( md->on ) {
						md->typ=-1;
						for( y1=0; y1<=md->ly;y1+=16 )
						for( x1=0; x1<=md->lx;x1+=16 ) {
								check(md->x+x1,md->y+y1);
								t=*poslab; t0=*poslal;
								check(md->x2+x1,md->y2+y1);
								*poslab=t; *poslal=t0;

						}
						addboom(md->x2+md->lx/2-16,md->y2+md->ly/2-16,32,32,0,1,b_boom,obloka+(10*256+32*32*5));
                                        }
					break;

				case m_item:
					if ( md->on ) {
						PutCMas(md->x-r_x+16,md->y-r_y+16,16,16,(block)additems+512*md->lx+((waitboom&2)?256:0));
        					if ( man.x+(man.dwx1+man.dwx2)/2>=md->x && man.x+(man.dwx1+man.dwx2)/2<=md->x+16
		        				&&  man.y<=md->y+32 && man.y>=md->y ) {
							switch ( md->lx ) {
								case 0:
									++continues;
									break;
								case 1:
									--maxjump;
									break;
								case 2:
									shboots=1;
									break;
								case 3:
									jetpack=1;
									break;
								case 4:
									glass=1;
									break;
								case 5:
									kolun=1;
									break;
								case 6:
									shields=1;
									break;
							}
							addboom(md->x,md->y+4,16,8,2,-2,b_buh,obloka+256*10+32*32*10+128*1);
							addboom(md->x,md->y+4,16,8,0,-3,b_buh,obloka+256*10+32*32*10+128*1);
							addboom(md->x,md->y+4,16,8,-2,-2,b_buh,obloka+256*10+32*32*10+128*1);
							score+=600;
							man.myrg=wmyrg;
							md->typ=-1;
							put_additems();
						}
					}
					break;
			}
		}
	}
}

void addboom(int x, int y, int lx, int ly, char sx, char sy, char typ, block buf) {
	//добавление объекта взрыва в очередь
	static int i;
	for ( i=0; i<maxboom; ++i ) {
		if ( booms[i].typ<0 ) {
			booms[i].x=x;
			booms[i].y=y;
			booms[i].lx=lx;
			booms[i].ly=ly;
			booms[i].sx=sx;
			booms[i].sy=sy;
			booms[i].typ=typ;
			booms[i].buf=buf;
			switch ( typ ) {
				case b_boom:
					booms[i].f=4;
					break;
				case b_buh:
					booms[i].f=8;
					break;
				default:
					booms[i].f=1;

			}
			return;
		}

	}
}

void doboom(void) {
	//вывод взрывов и других краткосрочн процессов
	static int i;
	static boom *bm;
	for ( i=0; i<maxboom; ++i ) {
		bm=&booms[i];
		switch ( bm->typ ) {
			case b_boom:	//взрыв на месте
				PutCMas(bm->x-r_x+16,bm->y-r_y+16,bm->lx,bm->ly,bm->buf+bm->f*bm->lx*bm->ly);
                        	if (waitboom&1) {
	                        	if (--bm->f<0)
						bm->typ=-1;
        	                }
				bm->x+=bm->sx;
				bm->y+=bm->sy;
				break;
			case b_buh:	//нет смены фаз
				PutCMas(bm->x-r_x+16,bm->y-r_y+16,bm->lx,bm->ly,bm->buf);
                        	if (waitboom&1) {
	                        	if (--bm->f<0)
						bm->typ=-1;
        	                }
				bm->x+=bm->sx;
				bm->y+=bm->sy;
				break;
			case b_crash:	//разлет кусков
			case b_fall:	//опадение кусков
				PutCMas(bm->x-r_x+16,bm->y-r_y+16,bm->lx,bm->ly,bm->buf);
                        	if (waitboom&1) {
					if ( ++bm->sy>16 || bm->y>siz_ylev*16 ) {
						bm->typ=-1;
					}
        	                }
				bm->x+=bm->sx;
				bm->y+=bm->sy;
				break;

		}

	}
	waitboom++;
}

void rislab(unsigned int x_, unsigned int y_) {
	static unsigned int x,y,sx,sy,i,j;
	static unsigned char far *poslab,*poslabt;
	static int sdvig0,sdvig,sdvig1;
	static unsigned int teki;
	static int teksmn,teksw;

	//для скроллинга
	//для плавного сдвига окна
	r_x=((teksw=r_x)+x_)/2; r_y=(r_y+y_)/2;
	teksw=r_x-teksw;

	//разделим координаты
	sx=(r_x&0x0f); x=r_x>>4;
	sy=(r_y&0x0f); y=r_y>>4;
	//позицию первую вычислим
	poslab=(unsigned char far *)level+(teksmn=x+y*siz_xlev);
	poslabt=(unsigned char far *)blevel+teksmn;

	//для анимации фона
	if ( ++sdvig0>=4 ) {
		sdvig0=0;
		if ( ++sdvig>=4 ) sdvig=0;
	}
	//для анимации вещей
	sdvig1=(++sdvig1)&7;

	nup=-1; //кол-во элементов сверху
	CalcAdr(16-sx,16-sy);
	do_strip(teksw/*man.sx*/);
	if ( draw )
	if (speedraw>1) { //вывод для быстрых и медл машин
		//вывод заднего плана
		show_strip( (unsigned int)(((long)r_y*striply)/((long)(siz_ylev-10)*16)) );
		//splitcopy0(CurrentScreen, r_x>320*8?(r_x-320*8)/8:r_x/8, r_y*40/((siz_ylev-10)*16), bckg);
		for ( i=0; i<=10; ++i ) {
			for ( j=0; j<=16; ++j ) {
				if ( *poslab ) { //вывод фона
					if ( (*poslabt&31)==23 ) { //вывод снаружи
						upput[++nup].adr=CurrentAdr;
						upput[nup].buf=fon+(*poslab+( (*poslab<16*8)?0:sdvig))*256;
					} else {
						CurrentAdr1=fon+(*poslab+( (*poslab<16*8)?0:sdvig))*256;
						SPutMas16();
					}
				}
				if ( (teki=*poslabt)>31 ) { //вывод вещи
					CurrentAdr1=items+(((teki>>5)-1)*8+sdvig1)*256;
					SPutMas16();
				}
				CurrentAdr+=16; ++poslab; ++poslabt;
			}
			poslab=poslab+(teksw=siz_xlev-17);
			poslabt=poslabt+teksw;
			CurrentAdr+=(320*16-16*17);
		}
	}
	else {
		for ( i=0; i<=10; ++i ) {
			for ( j=0; j<=16; ++j ) {
				CurrentAdr1=fon+(*poslab+( (*poslab<16*8)?0:sdvig))*256;
				if ( (*poslabt&31)==23 ) { //вывод снаружи
					upput[++nup].adr=CurrentAdr;
					upput[nup].buf=fon+(*poslab+( (*poslab<16*8)?0:sdvig))*256;
				}
				SPutImg16();
				if ( (teki=*poslabt)>31 ) { //вывод вещи
					CurrentAdr1=items+(((teki>>5)-1)*8+sdvig1)*256;
					SPutMas16();
				}
				CurrentAdr+=16; ++poslab; ++poslabt;
			}
			poslab=poslab+(teksw=siz_xlev-17);
			poslabt=poslabt+teksw;
			CurrentAdr+=(320*16-16*17);
		}
	}
}

void putup(void) {	//вывод верхнего рельева (сверху героя)
	while ( nup>=0 ) {
		CurrentAdr=upput[nup].adr; CurrentAdr1=upput[nup--].buf;
		SPutMas16();
	}
}

void doman() { //управление Героем
	static int oldx,oldy,i; //сохр стар коорд
	static int dw1,dw2,lf1,lf2,rt1,rt2,up1,up2; //коды трассы
	static int dwi1,dwi2,lfi1,lfi2,rti1,rti2,upi1,upi2; //коды вещей
	static int sx1,sx2;

	oldx=man.x; oldy=man.y;

	//прибавим ускорения по осям
	man.x+=(man.sx+(man.dsx2+man.dsx1)/2); //max!!!!
	man.y+=(man.sy+man.dsy);

	//пр-ка на выход за границу лабиринта
	if (man.x<0) man.x=0;
	if (man.x>siz_xlev*16-man.lx) man.x=siz_xlev*16-man.lx-1;
	if (man.y<man.ly) man.y=man.ly;

	//что справа, слева и вверху?
	lf1=check(man.x+man.lfx,man.y-man.lfy1);
	lfi1=tekblock;
	lf2=check(man.x+man.lfx,man.y-man.lfy2);
	lfi2=tekblock;
	rt1=check(man.x+man.rtx,man.y-man.rty1);
	rti1=tekblock;
	rt2=check(man.x+man.rtx,man.y-man.rty2);
	rti2=tekblock;
	up1=check(man.x+man.upx1,man.y-man.upy);
	upi1=tekblock;
	up2=check(man.x+man.upx2,man.y-man.upy);
	upi2=tekblock;
	//узнаем что под нами
	dw1=checkdw(man.x+man.dwx1,man.y-man.dwy);
	dwi1=tekblock; sx1=sx;
	dw2=checkdw(man.x+man.dwx2,man.y-man.dwy);
	dwi2=tekblock; sx2=sx;

	if (++man.f0>6-abs(man.sx+max(man.dsx1,man.dsx2))) {
		man.f0=0; man.f++;
	}

	if ( man.fw>0 )	{
		//если есть задержка в смене фаз
		--man.fw;
	} else
	switch ( man.s ) {
		case s_go:
			//идет
			if ( abs(man.sx+(man.dsx1+man.dsx2)/2)>1 ) {
				if ( man.f>=man.maxgo ) man.f=0;
				man.buf=man.gospr+man.f*man.lx*man.ly;
				man.fw=1;
			} else {
				//трясется
				if ( dw1==0 && sx1<8 && ((dw2>15 && dw2<20) || dw2==1) ) {
					//дыра слева
					man.napr=0;
					man.buf=man.specspr+man.lx*man.ly*(1+(waitboom&1));
					man.fw=2;
					if( (waitboom&15)==0 ) {
						addboom(man.x+12,man.y-man.ly,8,8,0,-1,b_buh,obloka+256*10+32*32*10+128*5+64*2);
					}

				} else if ( dw2==0 && sx2>8 && ((dw1>15 && dw1<20) || dw1==1) ) {
					//дыра справа
					man.napr=1;
					man.buf=man.specspr+man.lx*man.ly*(1+(waitboom&1));
					man.fw=2;
					if( (waitboom&15)==0 ) {
						addboom(man.x+12,man.y-man.ly,8,8,0,-1,b_buh,obloka+256*10+32*32*10+128*5+64*2);
					}
				} else
				//стоит
				if ( up ) {
				//смотрит вверх
					man.buf=man.stspr+man.lx*man.ly;
				} else if ( down ) {
				//вниз
					man.buf=man.stspr+2*man.lx*man.ly;
				} else {
					//стоит
					if ((waitboom&31)!=0)
						man.buf=man.stspr;
					else {
						//моргание Пуха
						man.buf=man.specspr;
						man.fw=2;
					}
				}
				man.f=0;
			}
			break;

		case s_jump:
		case s_down:
			//прыжок
			if ( man.sy<-2 ) {
				man.f=0;
			} else if ( man.sy<2 ) {
				man.f=1; man.fw=4;
			} else {
				man.f=2;
			}
			if ( man.sy>7 ) {	//при сильном падении
				man.buf=man.stspr+3*man.lx*man.ly;
				man.fw=2;
			} else {
				//фазы для прыжка
				man.buf=man.jmspr+man.f*man.lx*man.ly;
			}
			break;

		case s_ouch:
		case s_crash:
			//кто-то укусил
			man.buf=man.stspr+3*man.lx*man.ly;
			man.fw=10;
			//man.napr=2-random(2);
			break;

		case s_fly:
			if ( abs(man.sx)<=1 ) {
				man.buf=man.specspr+man.lx*man.ly*3;
				man.fw=1;
			} else {
				man.buf=man.specspr+man.lx*man.ly*(4+(waitboom&1));
				man.fw=2;
			}
			if ( power==0 && man.sy<=0) {	//если кончилось горючее
				++man.sy;
			}
			if( man.sy<0 ) {
				//дым из топки
				addboom(man.napr?(man.x):(man.x+16),man.y-8,16,16,-man.sx,1,b_boom,obloka+256*10);
			}
			break;

		default:
			//????
			man.buf=man.stspr;
			break;
	}

	if ( man.s!=s_crash ) { //для мертвеца не надо таких проверок!

		//втискивание куда не надо
		if ( (lf1==16 || rt1==16) && dw2==16 && dw1==16 && up1==16 && up2==16 ) {
			man.x=oldx;
			man.sx=man.dsx1=man.dsx2=0;
			man.y=oldy;
			man.sy/=2;
		}

		//столкновение со стенами
		if ( ((lf1==16 || lf2==16) && man.sx<0 ) || ((rt1==16 || rt2==16) && man.sx>0)) {
			man.x=oldx;
			man.sx=man.dsx1=man.dsx2=0;
		}

		//проверка на соприкосновение с монстрами
		if ( shields && power ) {
		} else
		if (man.myrg==0 ) {//если мужик не мыргает
			for ( i=0; i<maxobj; ++i ) {
				if ( obj[i].on!=0 && obj[i].myrg==0) {
					//если монстр жив и не моргает
					if ( man.x+man.lx>obj[i].x && man.x<obj[i].x+obj[i].lx
						&&  man.y-man.ly<obj[i].y && man.y>obj[i].y-obj[i].ly ) {
						//если есть шипы-убийцы
						if( shboots && man.s==s_down && man.y<obj[i].y) {
							//если падаем сверху на гада
							man.sy=-8;
							man.s=s_jump;
							//то убъем его
							if ( obj[i].typ<16 && --obj[i].on<=0 ) {
								obj[i].on=0;
								addboom(obj[i].x,obj[i].y-obj[i].ly,obj[i].lx,obj[i].ly,man.sx,random(5),b_fall,obj[i].buf);
								addboom(obj[i].x+obj[i].lx/2-16,obj[i].y-obj[i].ly/2-16,32,32,0,0,b_boom,obloka+(10*256+32*32*5));
								addboom(obj[i].x,obj[i].y-obj[i].ly,16,8,0,-1,b_buh,obloka+256*10+32*32*10);
								score+=100;
							} else {
								obj[i].myrg=8;
							}
							break;
						} else {
							//если мужик не в состоянии аффекта,
							//то в шок его!
							obj[i].myrg=9;

							man.sy=-8; man.s=s_ouch;
							man.myrg=wmyrg;
							addmess("Ой!",30);

							if ( --man.on<=0 ) {
								//мед кончился - капут
								man.s=s_crash;
			     					addmess("КОНЧИЛСЯ МЕДОК",60);
							}
							break;
						}
					}
				}
			}

			//проверка на столкновение с патронами врага
			//if (man.myrg==0 && shields==0) //если мужик не мыргает
			for ( i=0; i<maxbullet; ++i ) {
				if ( bullets[i].x>=man.x && bullets[i].x<=man.x+man.lx &&
					bullets[i].y<=man.y && bullets[i].y>=man.y-man.ly ) {
						switch ( bullets[i].typ ) {
						case 1:	//шишка
							break;
						case 2:
							bullets[i].typ=-1;
							addboom(bullets[i].x-8,bullets[i].y-8,16,16,0,0,b_boom,obloka);
							obj[i].myrg=9;

							man.sy=-4;
							man.s=s_ouch;
							man.myrg=wmyrg;
							addmess("Ай!",30);

							if ( --man.on<=0 ) {
								//мед кончился - капут
								man.s=s_crash;
								addmess("КОНЧИЛСЯ МЕДОК",60);
							}
							break;
						case 3:	//монетка
							bullets[i].typ=-1;
							addboom(bullets[i].x-8,bullets[i].y-8,16,16,0,0,b_boom,obloka);
							++score;
							if ( coins<9999 ) {
								++coins;
							}
							break;
						case 4:	//бочка
							bullets[i].typ=-1;
							addboom(bullets[i].x-8,bullets[i].y-8,16,16,0,0,b_boom,obloka);
							score+=200;
							if ( man.on<5 ) {
							++man.on;
							} else {
								coins+=5;
							}
							break;
						}

				}
			}
		}
		//просмотр лифтов и пр.
		for ( i=0; i<maxmodel; ++i ) {
			if ( models[i].typ>=0 )	{
				if ( man.x+(man.dwx1+man.dwx2)/2>=models[i].x && man.x+(man.dwx1+man.dwx2)/2<=models[i].x+models[i].lx
				&&  man.y<=models[i].y+models[i].ly && man.y>=models[i].y ) {
					if ( models[i].typ==m_lift || models[i].typ==m_lift0 ) {
						//лифты
						man.y=models[i].y;
						man.dsx1=man.dsx2=models[i].sx;
						man.dsy=models[i].sy;
						if (man.s<s_lift)
							man.s=s_lift;
						break;
					}
				}
			}
		}

	}

	if ( man.myrg>0 ) { //прекращаем моргание
		--man.myrg;
	}
	if ( power>0 ) { //защита-то кончается
		--power;
	}

	if (  (dw1==31 || dw2==31 || up1==31 || up2==31) && man.s!=s_crash) {
		man.sy=-8;	//шипы - в шок его!
		man.s=s_crash;
		addmess("ВЕРНАЯ ГИБЕЛЬ",60);
	}

	//проверка на пропасть
	//падение
	if ( man.s==s_down ) {
		if( man.sy<8 && (leveldef.typ!=t_water || (waitboom&1)==0) ) {
			++man.sy;
		}
		if (dw1!=0 || dw2!=0) {
			//земля под ногами
			man.s=s_go;

			man.sy=0;
			if ( min(poc[dw1][sx1],poc[dw2][sx2])<0 ) {
				//выравнивает на скатах
				man.y-=16;
				dw1=check(man.x+man.dwx1,man.y-man.dwy);
				dwi1=tekblock; sx1=sx;
				dw2=check(man.x+man.dwx2,man.y-man.dwy);
				dwi2=tekblock; sx2=sx;
			}
			if ( dw1 && dw2 ) {
				man.y=(man.y&0xfff0)+min(poc[dw1][sx1],poc[dw2][sx2]);
				man.dsx1=nappoc[dw1];
				man.dsx2=nappoc[dw2];
				man.sy=0;
			} else if ( dw1 ) {
				man.y=(man.y&0xfff0)+poc[dw1][sx1];
				man.dsx1=nappoc[dw1];
				man.dsx2=man.sy=0;
			} else if ( dw2 ) {
				man.y=(man.y&0xfff0)+poc[dw2][sx2];
				man.dsx2=nappoc[dw2];
				man.dsx1=man.sy=0;
			}
			if( leveldef.typ==t_ice ) {
				//скользко
				man.dsx1*=2;
				man.dsx2*=2;
			}
			man.buf=man.jmspr+3*man.lx*man.ly;
			man.fw=2;
			addboom(man.x+8,man.y-4,16,16,0,0,b_boom,obloka+16*16*5);

			if ( down && kolun) { //если есть колун
				if ( dw1>16 && dw1<=19 ) { //долбим колуном
					check(man.x+man.dwx1,man.y-man.dwy);
					if ( (--*poslal)<=15 ) {
						addbullet(x*16+8,y*16+8,-random(2),0,8,fon+(*poslal+1)*256);
						*poslab=*poslal=0;
					} else {
						addboom(x*16+2-random(4),y*16,16,16,0,0,b_boom,obloka+16*16*5);
					}
				} else
				if ( dw2>16 && dw2<=19 ) { //долбим колуном
					check(man.x+man.dwx2,man.y-man.dwy);
					if ( (--*poslal)<=15 ) {
						addbullet(x*16+8,y*16+8,random(2),0,8,fon+(*poslal+1)*256);
						*poslab=*poslal=0;
					} else {
						addboom(x*16+2-random(4),y*16,16,16,0,0,b_boom,obloka+16*16*5);
					}
				}
			}
		}
	} else
	//прыжок
	if ( man.s==s_jump || man.s==s_ouch ) {
		if ( dw1 || dw2 ) {
		   //прыжок в сторону наклона
		   man.dsx1=nappoc[dw1];
		   man.dsx2=nappoc[dw2];
		} //для up1
		if ( up1>=16 && up1<=19 ) { //удар о кирпичный или исчезающий потолок
				man.s=s_down;
				man.sy=1;
				check(man.x+man.upx1,man.y-man.upy);

				if ( up1!=16 && (--*poslal)<=15 ) { //если [?] то-приз
					addbullet(x*16+8,y*16+8,-1,-1,8,fon+(*poslal+1)*256);
					*poslal=*poslab=0;
					if ( up1==18 ) {
						if ( random(2) ) {
							addbullet(x*16+8,y*16,0,-1,3,items);
						} else {
							addbullet(x*16+8,y*16,0,-1,4,items+256*8);
						}
					}
					if( (waitboom&1)==0 ) {
						addboom(man.x+12,man.y-man.ly,8,8,0,-1,b_buh,obloka+256*10+32*32*10+128*5+64*3);
					}
				} else {
					addboom(x*16,y*16,16,16,0,0,b_boom,obloka+16*16*5);
				}
		} else //для up2
		if ( up2>=16 && up2<=19 ) { //удар о кирпичный или исчезающий потолок
				man.s=s_down;
				man.sy=1;
				check(man.x+man.upx2,man.y-man.upy);

				if ( up2!=16 && (--*poslal)<=15 ) { //если [?] то-приз
					addbullet(x*16+8,y*16+8,1,-1,8,fon+(*poslal+1)*256);
					*poslal=*poslab=0;
					if ( up2==18 ) {
						if ( random(2) ) {
							addbullet(x*16+8,y*16,0,-1,3,items);
						} else {
							addbullet(x*16+8,y*16,0,-1,4,items+256*8);
						}
					}
					if( (waitboom&1)==0 ) {
						addboom(man.x+12,man.y-man.ly,8,8,0,-1,b_buh,obloka+256*10+32*32*10+128*5+64*3);
					}
				} else {
					addboom(x*16,y*16,16,16,0,0,b_boom,obloka+16*16*5);
				}
		} else
		if ( man.y<=man.ly ) { //выход за экран
			man.s=s_down;
			man.y=man.ly;
			man.sy=2;
			upi1=upi2=0;
		} else
		if (++man.sy>=0) {
			man.s=s_down;
			man.sy=man.dsx1=man.dsx2=0;
		}
	} else
	if ( man.s==s_fly ) {
		//полет
		if( jump==0 && man.sy<5 ) {
			++man.sy;
		}
		if ( dw1 || dw2 ) {
			//опустился
			man.s=down;
			man.y=oldy;
		}
		if ( up1>1 || up2>1 ) { //удар о потолок
			man.s=s_down;
			man.sy=1;
		}
	} else
	if ( man.s==s_crash ) {
		//это совсем капут
		if(man.sy<32) ++man.sy;
		man.sx=man.dsx1=man.dsx2=0;
		if (man.y>siz_ylev*16) {
			//выпадение ниже лабиринта
			man.s=s_over;
			zend=1;
		}
	} else //норма
	if ( man.s==s_go ) {
		man.dsy=0;
		if ( dw1==0 && dw2==0 ) {
				man.s=s_down;
				man.sy=1; //2
		} else
		if ( dw1==19 ) { //исчезающая стенка
			check(man.x+man.dwx1,man.y-man.dwy);
			if ( man.x==oldx ) {
				if ( (--*poslal)<=15 ) {
					addbullet(x*16+8,y*16+8,-random(2),0,8,fon+(*poslal+1)*256);
					*poslab=*poslal=0;
				} else {
					addboom(x*16+2-random(4),y*16,16,16,0,0,b_boom,obloka+16*16*5);
				}
			}
		} else
		if ( dw2==19 ) { //исчезающая стенка
			check(man.x+man.dwx2,man.y-man.dwy);
			if ( man.x==oldx ) {
				if ( (--*poslal)<=15 ) {
					addbullet(x*16+8,y*16+8,random(2),0,8,fon+(*poslal+1)*256);
					*poslab=*poslal=0;
				} else {
					addboom(x*16+2-random(4),y*16,16,16,0,0,b_boom,obloka+16*16*5);
				}
			}
		}

		if ( min(poc[dw1][sx1],poc[dw2][sx2])<0 ) {
			//если подпорка ... наверх!
			man.y-=16;
			dw1=check(man.x+man.dwx1,man.y-man.dwy);
			dwi1=tekblock; sx1=sx;
			dw2=check(man.x+man.dwx2,man.y-man.dwy);
			dwi2=tekblock; sx2=sx;
		}
		//если что-то под ногами
		if ( dw1 && dw2 ) {
			man.y=(man.y&0xfff0)+min(poc[dw1][sx1],poc[dw2][sx2]);
			man.dsx1=nappoc[dw1];
			man.dsx2=nappoc[dw2];
			man.sy=0;
		} else if ( dw1 ) {
			man.y=(man.y&0xfff0)+poc[dw1][sx1];
			man.dsx1=nappoc[dw1];
			man.dsx2=man.sy=0;
		} else if ( dw2 ) {
			man.y=(man.y&0xfff0)+poc[dw2][sx2];
			man.dsx2=nappoc[dw2];
			man.dsx1=man.sy=0;
		}
		if( leveldef.typ==t_ice ) {
			man.dsx1*=2;
			man.dsx2*=2;
		}

	} else
	if ( man.s==s_lift ) {
		man.s=s_go;
	}

	if( man.s!=s_crash ) {
		//проверка на сбор вещей
		if ( lfi1 )
			getitem(man.x+man.lfx,man.y-man.lfy1);
		else if ( rti1 )
			getitem(man.x+man.rtx,man.y-man.rty1);
		else if ( upi1 )
			getitem(man.x+man.upx1,man.y-man.upy);
		else if ( upi2 )
			getitem(man.x+man.upx2,man.y-man.upy);
		else if ( dwi1 )
			getitem(man.x+man.dwx1,man.y-man.dwy);
		else if ( lfi2 )
			getitem(man.x+man.lfx,man.y-man.lfy2);
		else if ( rti2 )
			getitem(man.x+man.rtx,man.y-man.rty2);
		else if ( dwi2 )
			getitem(man.x+man.dwx2,man.y-man.dwy);
	}

	if ( draw==0 ) {
		return ;
	}

	if ( (power>32 || power&1) ) {
		CurrentColor=20;
		//полоса энергии
		Bar(24,20, max((int)power>>2,1) ,8);
		PutMas16(24+max((int)power>>2,1),16,items+256*8*6+((waitboom&7)*256));
	}
	if ( (power>32 || power&1) && shields ) {
			//вывод тени за ним
			if ( man.napr ) {
				PutCBlinkr(man.x-r_x+16-man.sx-man.sx,man.y-r_y+16-man.ly-man.sy-man.sy,man.lx,man.ly,man.buf);
				CurrentColor--;
				PutCBlinkr(man.x-r_x+16-man.sx,man.y-r_y+16-man.ly-man.sy,man.lx,man.ly,man.buf);
				CurrentColor--;
				PutCBlinkr(man.x-r_x+16,man.y-r_y+16-man.ly,man.lx,man.ly,man.buf);
			} else {
				PutCBlink(man.x-r_x+16-man.sx-man.sx,man.y-r_y+16-man.ly-man.sy-man.sy,man.lx,man.ly,man.buf);
				CurrentColor--;
				PutCBlink(man.x-r_x+16-man.sx,man.y-r_y+16-man.ly-man.sy,man.lx,man.ly,man.buf);
				CurrentColor--;
				PutCBlink(man.x-r_x+16,man.y-r_y+16-man.ly,man.lx,man.ly,man.buf);
			}
	} else {
		if ((man.myrg&2)==0) {
			if ( man.napr ) { //если мужик повернут, то выводим задом на перед
				PutCMasr(man.x-r_x+16,man.y-r_y+16-man.ly,man.lx,man.ly,man.buf);
			} else {
				PutCMas(man.x-r_x+16,man.y-r_y+16-man.ly,man.lx,man.ly,man.buf);
			}
		} else {
			CurrentColor=15; //он ранен и мигает
			if ( man.napr ) { //если мужик повернут, то выводим задом на перед
				PutCBlinkr(man.x-r_x+16,man.y-r_y+16-man.ly,man.lx,man.ly,man.buf);
			} else {
				PutCBlink(man.x-r_x+16,man.y-r_y+16-man.ly,man.lx,man.ly,man.buf);
			}
		}
	}
}

void getitem(int x_, int y_) { //взятие вещи по координатам

	check(x_,y_); //посмотрим на то, что у нас по этим коорд.
	switch ( tekblock&224 ) {
		case 1<<5: //монета
			*poslab=*poslab&31;
			addboom(x*16+4,y*16+4,8,8,0,-1,b_buh,obloka+256*10+32*32*10+128*5);
			++score;
			if ( coins<9999 ) {
				++coins;
			}
			break;

		case 2<<5: //мед - энергия
			*poslab=*poslab&31;
			addboom(x*16,y*16,16,8,0,-1,b_buh,obloka+256*10+32*32*10+128*1);
			score+=200;
			if ( man.on<5 ) {
				++man.on;
			} else {
				coins+=5;
			}
			break;

		case 3<<5: //сердце - жизнь
			*poslab=*poslab&31;
			addboom(x*16,y*16,16,8,0,-1,b_buh,obloka+256*10+32*32*10+128*3);
			score+=1000;
			if ( hearts<9 ) {
				++hearts;
				put_additems();
			}
			break;

		case 4<<5: //ключи
			*poslab=*poslab&31;
			addboom(x*16,y*16,16,16,0,0,b_boom,obloka+256*5*4);
			addboom(x*16,y*16,16,8,0,-1,b_buh,obloka+256*10+32*32*10+128*1);
			score+=200;
			if ( keys<9 ) {
				++keys;
			} else {
				coins+=25;
			}
			break;

		case 5<<5: //шишки
			*poslab=*poslab&31;
			addboom(x*16+4,y*16+4,8,8,0,-1,b_buh,obloka+256*10+32*32*10+128*5+64);
			if ( beams<99 ) {
				++beams;
			}
			break;

		case 6<<5: //время
			*poslab=*poslab&31;
			addboom(x*16,y*16,16,8,0,-1,b_buh,obloka+256*10+32*32*10+128*2);
			score+=500;
			times=9999;
			break;

		case 7<<5: //энергия
        		*poslab=*poslab&31;
			addboom(x*16,y*16,16,16,0,0,b_boom,obloka+256*5*4);
			addboom(x*16,y*16,16,8,0,-1,b_buh,obloka+256*10+32*32*10+128*2);
			score+=500;
			power=500;
			//break;
		}
}

void put_score(void) {
	//вывод очков
	static int i;
	screen old;
	if ( draw==0 ) {
		return;	//не всегда выводить очки!
	}

	old=CurrentScreen;
	char_fgd=16;
	NormalScreen();
	Clip(0,0,319,199);
	for ( i=0; i<5; i++ ) {
		PutMas(16+i*9,200-15,8,8,ind+((i<man.on)?64:0));
	}

	char_bkgd=218;

	PutMas(80,200-15,8,8,ind+8*8*2);
	MoveXY(90,200-14);
	vprint("%04d",coins);

	PutMas(140,200-15,8,8,ind+8*8*5);
	MoveXY(150,200-14);
	vprint("%02d",beams);

	PutMas(170,200-15,8,8,ind+8*8*4);
	MoveXY(180,200-14);
	vprint("%1d",keys);

	PutMas(210,200-15,8,8,ind+8*8*6);
	MoveXY(220,200-14);
	vprint("%04d",times);

	MoveXY(258,200-14);
	vprint("%06lu",score);

	char_bkgd=0;

	Clip(16,16,16*17-1,16*11-1);
	SetScreen(old);
}

void put_additems(void) {
     //вывод дополнительны вещей
	screen old=CurrentScreen;
	NormalScreen();
	Clip(0,0,319,199);

	char_bkgd=218;
	if ( shields ) { //щит
		PutMas16(16*18,20+20*0,additems+256*12);
	}
	if ( glass ) { //ранцев. двиг
		PutMas16(16*18,20+20*1,additems+256*8);
	}
	if ( maxjump<-8 ) { //пружинки
		PutMas16(16*18,20+20*2,additems+256*2);
	}
	if ( shboots ) { //шиповки
		PutMas16(16*18,20+20*3,additems+256*4);
	}
	if ( kolun ) { //колун
		PutMas16(16*18,20+20*4,additems+256*10);
	}
	if ( jetpack ) { //ранцев. двиг
		PutMas16(16*18,20+20*5,additems+256*6);
	}

	if ( continues>0 ) { //продолжения
		PutMas16(16*18,20+20*6,additems);
		if ( continues>1 ) {
			MoveXY(16*18,20+20*6);
			vprint("%1d",continues);
		}
	}
	//жизни
	PutMas16(16*18,20+20*7,items+256*8*2);
	MoveXY(16*18,20+20*7); vprint("%1d",hearts);

	char_bkgd=0;

	Clip(16,16,16*17-1,16*11-1);
	SetScreen(old);
}

void makemonstr(int i, int n, unsigned int x,unsigned int y) {
	object *ob;
	//номер монстра, тип монстра, нач коорд монстра
	if ( i<maxobj && n<maxmonstr && n<nmonstr) {
		ob=&obj[i];
		ob->x=x;
		ob->y=y;
		ob->typ=monsters[n].typ;
		ob->n=n;

		ob->dsx=ob->dsy=
		ob->myrg=ob->napr=ob->f=ob->sy=ob->sx=0;

		ob->lx=monsters[n].lx;
		ob->ly=monsters[n].ly;
		ob->lfx=monsters[n].lfx; ob->rtx=monsters[n].rtx;
		ob->lfy=monsters[n].lfy; ob->rty=monsters[n].rty;
		ob->upx=monsters[n].upx; ob->upy=monsters[n].upy;
		ob->dwx=monsters[n].dwx; ob->dwy=monsters[n].dwy;
		ob->on=monsters[n].lives;
		ob->s=s_down;
	}
}

void doobj(void) {
	//поведение врагов
	static int oldx,oldy,i,j;
	static int dw,lf,rt,up_;
	static object *ob;

	for ( i=0; i<maxobj; ++i ) {
		if ( obj[i].on!=0 ) {
			//если активен, то изучаем
			ob=&obj[i];
			oldx=ob->x; oldy=ob->y;

			//пр-ка на выход за границу лабиринта
			if (ob->x<0) {
				ob->x=0;
				ob->sx=monsters[ob->n].speed;
			}
			if (ob->x>siz_xlev*16-ob->lx) {
				ob->x=siz_xlev*16-ob->lx-1;
				ob->sx=-monsters[ob->n].speed;
			}
			if (ob->y<=ob->ly) {
				ob->sy=monsters[ob->n].speed;
				ob->y=ob->ly;
			}
			if (ob->y>siz_ylev*16) {
				ob->sy=-monsters[ob->n].speed;
				ob->y=siz_ylev*16;
			}

			//прибавим ускорения по осям
			ob->x+=(ob->sx+ob->dsx);
			ob->y+=(ob->sy+ob->dsy);

			//что справа, слева и вверху?
			lf=check(ob->x+ob->lfx,ob->y-ob->lfy);
			rt=check(ob->x+ob->rtx,ob->y-ob->rty);
			up_=check(ob->x+ob->upx,ob->y-ob->upy);
			//узнаем что под нами
			dw=check(ob->x+ob->dwx,ob->y-ob->dwy);
			if ( dw==23 ) {
				dw=0;
			}
			if ( ob->typ==3 || ob->typ==6 || ob->typ==19 ) //летун
			{
				ob->s=s_fly;
			}

			if (++ob->f0>6-abs(ob->sx+ob->dsx)) {
				ob->f0=0;
				ob->f++;
			}

			if ( (ob->s==s_jump || ob->s==s_down) && monsters[ob->n].maxjm && abs(ob->sy)>2) {
				//прыгает
				ob->buf=monsters[ob->n].jmspr;
			} else
			if ( ob->sx+ob->dsx!=0 && ob->x!=oldx) {
				//идет
				if ( ob->f>=monsters[ob->n].maxgo ) ob->f=0;
				ob->buf=monsters[ob->n].gospr+ob->f*ob->lx*ob->ly;
			} else {
				//стоит
				ob->buf=monsters[ob->n].stspr;
				ob->f=0;
			}
			if (ob->sx>0) { //поворачиваем спрайт в зависимости от направления
				ob->napr=1;
			} else if ( ob->sx<0 ){
				ob->napr=0;
			}

			//столкновение со стенами
			if ( (lf==16 && ob->sx<0 )|| (rt==16 && ob->sx>0)) {
				if ( lf ) {
					ob->sx=0; //monsters[ob->n].speed;
					ob->x=oldx;
				}
				if ( rt ) {
					ob->sx=0; //-monsters[ob->n].speed;
					ob->x=oldx;
				}
			}

			if ( ob->myrg<=0 ) //если не моргает
			for ( j=0; j<maxbullet; ++j ) {	//столкновение с пулями и блоками
				if ( (bullets[j].typ==1 || bullets[j].typ>=8) && bullets[j].x>=ob->x && bullets[j].x<=ob->x+ob->lx &&
					bullets[j].y<=ob->y && bullets[j].y>=ob->y-ob->ly
				) {
					bullets[j].typ=-1;
					addboom(bullets[j].x-8,bullets[j].y-8,16,16,0,0,b_boom,obloka);
					ob->myrg=16;
					if ( --ob->on<=0 ) {
						if ( ob->typ>=16 ) {
							//если убит БОСС!!!!!!!
							boss=0;
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,-2,-2,3,items);	//монетка
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,2,-2,3,items);	//монетка
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,-1,-3,3,items);	//монетка
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,1,-3,3,items);	//монетка
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,0,-4,3,items);	//монетка
							score+=5000;
							coins+=30;
							open_exit();
							addboom(ob->x,ob->y-ob->ly,ob->lx,ob->ly,bullets[j].sx,-random(5),b_fall,ob->buf);
							addboom(ob->x,ob->y-ob->ly,16,8,0,-2,b_buh,obloka+256*10+32*32*10+128*4);
							if ( dev!=255 ) {
								//включаем нормальную музыку
								modstop();
								modsetup(leveldef.musicname, 4, 0 ,mix, dev, &state );
								modvolume(vol,vol,vol,vol);
							}
							for ( i=0; i<maxobj; ++i ) {
								ob->on=0;
							}
							return;

						} else {
							addboom(ob->x,ob->y-ob->ly,16,8,0,-1,b_buh,obloka+256*10+32*32*10);
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,0,-3,3,items);	//монетка
							score+=100;
						}
						ob->on=0;
						addboom(ob->x,ob->y-ob->ly,16,16,0,-8,b_fall,obloka+256*5*5+random(5)*256);
						addboom(ob->x,ob->y-ob->ly,ob->lx,ob->ly,bullets[j].sx,-random(5),b_fall,ob->buf);
						addboom(ob->x+ob->lx/2-16,ob->y-ob->ly/2-16,32,32,0,0,b_boom,obloka+(10*256+32*32*5));
					}
				}
			}
			if ( ob->s==s_fly ) {
				//ПОЛЕТ
				if ( up_ && up_!=23) {
					ob->sy=max(monsters[ob->n].speed,1);
				}
				if ( dw && dw!=23) {
					ob->sy=-max(monsters[ob->n].speed,1);
				}
				if ( lf && lf!=23) {
					ob->sx=monsters[ob->n].speed;
				}
				if ( rt && rt!=23 ) {
					ob->sx=-monsters[ob->n].speed;
				}
			} else
			//проверка на пропасть
			//падение
			if ( ob->s==s_down ) {
				if(ob->sy<7) ++ob->sy;
				//if (dw!=0 && (dw1!=0 || dw2!=0)) {
				if (dw!=0) {
					if ( dw==31 ) {
						//на шипах помрем!
						if ( ob->myrg==0 ) {
							--ob->on;
							ob->myrg=16;
						}
					}
					//земля под ногами
					ob->s=s_go;
					ob->sy=0;
					if ( poc[dw][sx]>=0 ) {
						ob->y=(ob->y&0xfff0)+poc[dw][sx];
					} else {
						ob->y-=16;
						dw=check(ob->x+ob->dwx,ob->y-ob->dwy);
						ob->y=(ob->y&0xfff0)+poc[dw][sx];
					}
				}
			} else
			//прыжок
			if ( ob->s==s_jump) {
				if ( up_==16 ) { //удар о потолок
						ob->s=s_down;
						ob->sy=0;
				} else
				if ( ob->y<=ob->ly || ob->y<=0) { //выход за экран
					ob->s=s_down;
					ob->y=ob->ly;
					ob->sy=4;
				} else
				if (++ob->sy>=0) {
					ob->s=s_down;
					ob->sy=0;
					ob->dsx=0;
				}
			} else {
				//норма
				if ( ob->s==s_go ) {
					if ( dw==0 ) {
						ob->s=s_down;
						ob->sy=2;
						//ob->dsx=ob->dsx/2;
						ob->dsy=0;
					} else if ( dw==31 ) {
						//на шипах помрем!
						if ( ob->myrg==0 ) {
							ob->myrg=16;
							if (--ob->on<=0) {
								//монстр возрождается после шипов
								addboom(ob->x,ob->y-ob->ly,32,32,0,0,b_boom,obloka+(10*256+32*32*5));
								addboom(kobj[i].x,kobj[i].y-ob->ly,32,32,0,0,b_boom,obloka+(10*256+32*32*5));
								makemonstr(i,kobj[i].n,kobj[i].x,kobj[i].y);
								break;
							}
						}
					} else {
						ob->sy=0;
						//прибавим от рельефа скорость движения

						//выравнивание по рельефу
						if ( poc[dw][sx]>=0 ) {
							ob->y=(ob->y&0xfff0)+poc[dw][sx];
						} else {
							ob->y-=16;
							dw=check(ob->x+ob->dwx,ob->y-ob->dwy);
							ob->y=(ob->y&0xfff0)+poc[dw][sx];
						}
						ob->dsx=nappoc[dw]/2;
					}
				}
			}

			if ( ob->typ>=16 && !boss) {
				continue;
			}

			if (ob->s==s_go || ob->s==s_fly) {
				switch ( ob->typ ) {
					case 0: //бродяга от стены до стены
						if( lf==16 && ob->sx<0 ) {
							ob->f=ob->sx=0;
							ob->buf=monsters[ob->n].stspr;
							break;
						}
						if( rt==16 && ob->sx>0 ) {
							ob->f=ob->sx=0;
							ob->buf=monsters[ob->n].stspr;
							break;
						}
						if (ob->sx==0 || (ob->sx+ob->dsx)==0) {
							if (lf==16) {
								ob->sx=monsters[ob->n].speed;
							} else if (rt==16) {
								ob->sx=-monsters[ob->n].speed;
							} else {
								ob->sx=monsters[ob->n].speed-random(monsters[ob->n].speed*2);
							}
						}
						break;

					case 1: //умный преследователь
						if ( dw==0 ) {	//внизу пропасть!
							ob->x=oldx;
							ob->y=oldy;
							ob->f=ob->sy=ob->sx=0;
							ob->buf=monsters[ob->n].stspr;
							break;
						}
						if (ob->x>man.x) {
							if ( ob->sx>-monsters[ob->n].speed )
								--ob->sx;
						}
						else if (ob->x<man.x) {
							if ( ob->sx<monsters[ob->n].speed )
								++ob->sx;
						}
						else ob->sx/=2;
						break;

					case 2: //прыгун
						if ( dw ) {
							ob->sy=-random(10);
							ob->s=s_jump;
							ob->napr=(man.x<ob->x)?0:1;
						}
						break;

					case 3: //летун от стены до стены
						if ( ob->sx==0 ) {
							if (lf) ob->sx=monsters[ob->n].speed;
							else if (rt) ob->sx=-monsters[ob->n].speed;
							else ob->sx=monsters[ob->n].speed-random(monsters[ob->n].speed*2);
						}
						break;

					case 4: //падун от потолка до пола СНАРЯД и т.п.
						if ( dw ) {
							ob->on=0;
							addboom(ob->x+ob->lx/2-8,ob->y-ob->ly/2-8,16,16,0,0,b_boom,obloka);
						}
						break;

					case 5: //прыгун от стены до стены
						if ((lf==16 || rt==16) && ob->sx)
							ob->sx=0;
						else if ( ob->sx+ob->dsx==0) {
							if (lf==16) {
								ob->sx=monsters[ob->n].speed;
							} else if (rt==16) {
								ob->sx=-monsters[ob->n].speed;
							} else ob->sx=monsters[ob->n].speed-random(monsters[ob->n].speed*2);
						}
						if ( dw ) {
							ob->sy=-3-random(3);
							ob->s=s_jump;
						}
						break;

					case 6: //летающий умный преследователь
						if (ob->x>man.x) {
							if ( ob->sx>-monsters[ob->n].speed )
								--ob->sx;
						} else if (ob->x<man.x) {
							if ( ob->sx<monsters[ob->n].speed )
								++ob->sx;
						} else ob->sx=0;

						if (ob->y>man.y-20) {
							if ( ob->sy>-max(monsters[ob->n].speed,1) )
								--ob->sy;
						} else if (ob->y<man.y-20) {
							if ( ob->sy<max(monsters[ob->n].speed,1) )
								++ob->sy;
						} else ob->sy=0;
						break;

					case 16: //босс N2
						//if( boss==0 )
						//	break;
						if (ob->x>man.x-16) {
							if ( ob->sx>-monsters[ob->n].speed )
								--ob->sx;
						}
						else if (ob->x<man.x-16) {
							if ( ob->sx<monsters[ob->n].speed )
								++ob->sx;
						} else {
							ob->sx=0;
						}
                        			if ((waitboom&31)==0) {
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly,(man.x<ob->x)?-5:((man.x>ob->x)?5:0),ob->y>man.y+16?-3:0, 2,enbull);
						}

						if (oldx==ob->x/*ob->y>=man.y*/ && (lf || rt) && dw) {
							ob->sy=-8;
							ob->s=s_jump;
						}
						break;

					case 17: //босс N3
						//if( boss==0 )
						//	break;
						if (ob->x>man.x) {
							if ( ob->sx>-monsters[ob->n].speed )
								--ob->sx;
						}
						else if (ob->x<man.x-48) {
							if ( ob->sx<monsters[ob->n].speed )
								++ob->sx;
						} else {
							if ( man.y<=ob->y ) {
								//вражье семя!
                        					if ((waitboom&7)==0)
									addbullet(man.x+8,ob->y-16*14,man.sx,2+random(2), 2,enbull+256*8);
								//addbullet(man.x+8,ob->y-16*14,man.sx,4, 2,items+256*1*8);
							}
							ob->sx=0;
						}
                        			if ((waitboom&15)==0)
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,(man.x<ob->x)?-5:((man.x>ob->x)?5:0),ob->y>man.y+16?-5:0, 2,enbull);
						break;

					case 18: //босс N1
						//if( boss==0 )
						//	break;
						if (ob->x>man.x+8) {
							if ( ob->sx>-monsters[ob->n].speed )
								--ob->sx;
						}
						else if (ob->x<man.x+8) {
							if ( ob->sx<monsters[ob->n].speed )
								++ob->sx;
						} else {
							ob->sy=-8;
							ob->s=s_jump;
							ob->sx=0;
						}
						break;

					case 19: //босс N4 летающий
						//if( boss==0 )
						//	break;
						if (ob->x+ob->lx/2-2>man.x+man.lx/2) {
							if ( ob->sx>-monsters[ob->n].speed && ob->sy<0)
								--ob->sx;
						}
						else if (ob->x+ob->lx/2+2<man.x+man.lx/2) {
							if ( ob->sx<monsters[ob->n].speed && ob->sy<0)
								++ob->sx;
						} else {
                        				if ((waitboom&3)==0) {
								addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,man.sx,2+random(2), 2,enbull+256*8);
							}
							if ( ob->sy>-4 ) {
								ob->sy--;
								ob->sx=0;
							}
						}
						if (ob->y>man.y-man.ly ) {
							ob->sy--;
						}
						if ( up ) {
							ob->sy++;
						}
						break;

					case 21: //босс СНЕГОВИК!
						//if( boss==0 )
						//	break;
						if (ob->x>man.x) {
							if ( ob->sx>-monsters[ob->n].speed )
								--ob->sx;
							if( lf==16 ) {
								ob->sx=-monsters[ob->n].speed;
								ob->sy=-8;
								ob->s=s_jump;
							}
						}
						else if (ob->x<man.x-21) {
							if ( ob->sx<monsters[ob->n].speed )
								++ob->sx;
							if( rt==16 ) {
								ob->sx=monsters[ob->n].speed;
								ob->sy=-8;
								ob->s=s_jump;
							}
						} else if (man.sx==0) {
								ob->sy=-8;
								ob->s=s_jump;
						}
						//вражье семя!
                        			if ((waitboom&15)==0) {
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly,man.x>ob->x?5:-5,-2+random(4),2,fon+(176*256));
						}
						break;

					case 20: //босс СЛОНОПОТАМ!
						//if( boss==0 )
						//	break;
						if (ob->x>man.x) {
							if ( ob->sx>-monsters[ob->n].speed )
								--ob->sx;
							if( lf==16 ) {
								ob->sx=-monsters[ob->n].speed;
								ob->sy=-8;
								ob->s=s_jump;
							}
						}
						else if (ob->x<man.x-48) {
							if ( ob->sx<monsters[ob->n].speed )
								++ob->sx;
							if( rt==16 ) {
								ob->sx=monsters[ob->n].speed;
								ob->sy=-8;
								ob->s=s_jump;
							}
						} else {
						}
						//вражье семя!
                        			if ((waitboom&15)==0) {
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly,man.x>ob->x?11:-11,3+random(3),2,enbull+256*8);
						}
						break;

					default:
						break;
				}
			}

			if (ob->myrg>0) --ob->myrg;
			if ( draw ) {
				if ((ob->myrg&2)==0) {
					if ( ob->napr ) {
						PutCMasr(ob->x-r_x+16,ob->y-r_y+16-ob->ly,ob->lx,ob->ly,ob->buf);
					} else {
						PutCMas(ob->x-r_x+16,ob->y-r_y+16-ob->ly,ob->lx,ob->ly,ob->buf);
					}
				} else {
					CurrentColor=33;
					if ( ob->napr ) {
						PutCBlinkr(ob->x-r_x+16,ob->y-r_y+16-ob->ly,ob->lx,ob->ly,ob->buf);
					} else {
						PutCBlink(ob->x-r_x+16,ob->y-r_y+16-ob->ly,ob->lx,ob->ly,ob->buf);
					}
				}
				if ( boss && ob->typ>=16 ) {
					//если это БОСС то выводим его силу
					CurrentColor=(32+(ob->myrg&1));
					Bar(24,30, max((int)ob->on*8,1) ,8);
					PutMas16(24+max((int)ob->on*8,1),26,items+256*8*2+((waitboom&7)*256));
				}
			}
		}
	}
}

void Delay( int p ) {
	//int i,j;
	long tic; //для задержки по таймеру
	//моя п/п пауза
	tic=gettic()+p;
	while (gettic()<tic && zend==0);
}

void mult(char * name) {
	//считывание описания мульта и показ
	screen text;
	block txt,tmp;
	block tail; // PATCH: text buffer overflow
	char _char_fgd=char_fgd, _char_bkgd=char_bkgd; /*цвет символа и фона*/

	int x,y;
	int n,i,j,k;

	char str1[20];
	char str2[20];
	char str3[20];
	char str4[20];
	char str5[20];
	char str6[20];
	char str7[20];

	x = y = 0;
	NormalScreen();
	//считаем сценарий
	text=famemalloc(SizeLib(name)+8);
	_fmemset(text,0,SizeLib(name)+8);
	GetLib(name,(block)text); txt=(block)text;
	tail = &txt[SizeLib(name) + 8]; // PATCH: text buffer overflow

	n=1; zend=0;
	while(n>=1 && zend==0) {

		n = tail - txt; n = (n > 80) ? 80 : n; *stroka = 0; // PATCH: text buffer overflow
		//считаем очередн строку
		memcpy((block)&stroka,txt,n);
		stroka[n] = 0; // PATCH: text buffer overflow
		n=sscanf(stroka,"%s %s %s %s %s %s %s ",
		&str1,&str2,&str3,&str4,&str5,&str6,&str7);

		if (stricmp(str1,"CLS")==0) {
			//очиска экр
			SetColor(atoi(str2));
			WVR();
			CBar(0,0,320,200);
		} else
		if (strnicmp(str1,"TI",2)==0) {
			//паркет
			GetLib(str2,(block)hidscr);
			TileBar(MinX,MinY,MaxX-MinX+1,MaxY-MinY+1,atoi(str3),atoi(str4),(block)hidscr);
		} else
		if (stricmp(str1,"BAR")==0) {
			//прямоугольник
			WVR();
			CBar(atoi(str2),atoi(str3),atoi(str4),atoi(str5));
		} else
		if (strnicmp(str1,"PAU",3)==0) {
			//пауза
			tic=gettic()+18*atoi(str2);
			while (gettic()<tic && zend==0);
		} else
		if (stricmp(str1,"CLIP")==0) {
			//обл экрана
			Clip(atoi(str2),atoi(str3),atoi(str4),atoi(str5));
			x=atoi(str2);y=atoi(str3);
		} else
		if (stricmp(str1,"COL")==0) {
			//цвет
			SetColor(atoi(str2));
			char_fgd=CurrentColor;
		} else
		if (stricmp(str1,"XY")==0) {
			//позиция на экране
			x=atoi(str2);y=atoi(str3);
		} else
		if (stricmp(str1,"PSHOW")==0) {
			//показать картинку
			GetLibs(str2,(block)hidscr);
			WVR();
			PutCMas(x,y,atoi(str3),atoi(str4),(block)hidscr);
		} else
		if (stricmp(str1,"ISHOW")==0) {
			//показать картинку без просветов
			GetLibs(str2,(block)hidscr);
			WVR();
			PutCImg(x,y,atoi(str3),atoi(str4),(block)hidscr);
		} else
		if (stricmp(str1,"SSHOW")==0) {
			//показать картинку цветной контур
			GetLibs(str2,(block)hidscr);
			WVR();
			PutCBlink(x,y,atoi(str3),atoi(str4),(block)hidscr);
		} else
		if (stricmp(str1,"LSHOW")==0) {
			//показать картинку линиями выезжая
			GetLibs(str2,(block)hidscr);
			for ( i=0; i<atoi(str4); ++i ) {
				WVR();
				for ( j=atoi(str4)-i-1; j>=0; j-- ) {
					PutCImg(x,y+i+j,atoi(str3),1,(block)hidscr+atoi(str3)*i);
				}
			}
		} else
		if (stricmp(str1,"XSHOW")==0) {
			//показать картинку точками
			GetLibs(str2,(block)hidscr);
			tic=gettic()+18*atoi(str5); //кол-во секунд
			do {
				i=random(atoi(str3)); j=random(atoi(str4));
				//CurrentColor=*((block)hidscr)+i+j*atoi(str3);
				PutImg(x+i,y+j,1,1,(block)hidscr+i+j*atoi(str3));
				/*if ( CurrentColor ) {
					PutPixel(x+i,y+j);
				}*/
			} while (gettic()<tic && zend==0);
			PutCMas(x,y,atoi(str3),atoi(str4),(block)hidscr);
		} else
		if (stricmp(str1,"IROLL")==0) {
			//двигать картинку на экране
			GetLibs(str2,(block)hidscr);
			i=atoi(str7);
			do {
				x+=atoi(str5); y+=atoi(str6);
				tic=gettic()+1;
				WVR();
				PutCImg(x,y,atoi(str3),atoi(str4),(block)hidscr);
				while (gettic()<tic && zend==0);
			} while ( --i>0 && zend==0 );
		} else
		if (stricmp(str1,"MUL")==0) {
			//мульт из неск кадров повторяющийся N секунд
			GetLibs(str2,(block)hidscr);
			k=atoi(str5);	//кол-во кадров
			j=atoi(str6);	//задержка delay
			i=0;
			tic=gettic()+18*atoi(str7);
			do {
				WVR();
				PutCImg(x,y,atoi(str3),atoi(str4),(block)hidscr+(i*atoi(str3)*atoi(str4)));
				Delay(j);
				if ( ++i>=k ) i=0;
			} while (gettic()<tic && zend==0);
		} else
		if (stricmp(str1,"NMUL")==0) {
			//мульт из неск кадров повторяющийся N раз
			GetLibs(str2,(block)hidscr);
			k=atoi(str5);	//кол-во кадров
			j=atoi(str6);	//задержка delay
			i=0;
			n=atoi(str7);
			do {
				WVR();
				PutCImg(x,y,atoi(str3),atoi(str4),(block)hidscr+(i*atoi(str3)*atoi(str4)));
				Delay(j);
				if ( ++i>=k ) {
					i=0;
					if ( (--n)<=0 ) {
						n=1;
						break;
					}
				}
			} while (zend==0);
		} else
		if (strnicmp(str1,"TXT",3)==0) {
			//вывод текста без просветов
			MoveXY(x,y); tmp=txt;
			while ( *tmp && *tmp!='@' ) ++tmp; ++tmp;
			while ( *tmp && *tmp!='@' && zend==0) {
				if ( *tmp=='#' ) {
					tic=gettic()+18;
					while (gettic()<tic && zend==0);
				} else
				if ( *tmp=='\n' ) {
					MoveXY(x,(y+=9));
				} else {
					PutCh((str1[3])?CharUp(*tmp):(*tmp));
					tic=gettic()+1;
					while (gettic()<tic && zend==0);
				}
				tmp++;
			}
		} else
		if (strnicmp(str1,"FTX",3)==0) {
			//вывод теста без просветов и без пауз
			MoveXY(x,y); tmp=txt;
			while ( *tmp && *tmp!='@' ) ++tmp; ++tmp;
			while ( *tmp && *tmp!='@' && zend==0) {
				if ( *tmp=='\n' ) {
					MoveXY(x,(y+=9));
				} else {
					PutCh((str1[4])?CharUp(*tmp):(*tmp));
				}
				tmp++;
			}
		} else
		if (strnicmp(str1,"BTXT",4)==0) {
			//вывоб фразы большими буквами
			MoveXY(x,y); tmp=txt;
			while ( *tmp && *tmp!='@' ) ++tmp; ++tmp;
			while ( *tmp && *tmp!='@' && zend==0) {
				putBch(x,y,(str1[4])?CharUp(*tmp):(*tmp),tfnt);
				x+=24; ++tmp;
				tic=gettic()+1;
				while (gettic()<tic && zend==0);
			}
		} else
		if (stricmp(str1,"LPAL")==0) {
			//считать с диска палитру
			GetLib(str2,palette);
		} else
		if (stricmp(str1,"LIB")==0) {
			//считать с диска новую биб-ку
			SetLib(str2);
		} else
		if (stricmp(str1,"PAL")==0) {
			//уст палитру
			PutPalette(palette);
		} else
		if (stricmp(str1,"OFF")==0) {
			//тушить палитру
			PaletteOff(palette);
		} else
		if (stricmp(str1,"ON")==0) {
			//зажигать палитру
			PaletteOn(palette);
		} else
		if (str1[0]=='Q' || str1[0]=='q') {
			goto m_exit;
		}

		 //идем на след строку
		 // PATCH: text buffer overflow
		 while ( (txt < tail) && ((*txt>27) || (*txt==9)) ) ++txt;
		 while ( (txt < tail) && (*txt<27) ) ++txt;
	}
m_exit:
	if ( zend ) {	//если прервали
		PaletteOff(palette); Cls(0);
	}
	char_fgd=_char_fgd;char_bkgd=_char_bkgd; /*цвет символа и фона*/
	farfree(text);
	return;
}

int GetLibs(char *s, block f) {
	//экономная загрузка
	str0[20]=0;
	if (stricmp(str0,s)!=0) {
		strcpy(str0,s);
		return GetLib(s,f);
	}
	return 0;
}
