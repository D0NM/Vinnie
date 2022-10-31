#include	<stdlib.h>
#include	<bios.h>
#include	<alloc.h>
#include	<string.h>

#include "famegraf.h"
#include "mouse.h"
#include "joy.h"
#include "keys.h"
#include "all.h"

Joy J;
Mouse m;
#ifdef EDITOR
#include "dialog.h"
Panel p;
#endif
Keys k=Keys(md);

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

levdef leveldef;

//размер уровня
unsigned int siz_xlev=500;
unsigned int siz_ylev=60;
unsigned long siz_level=(unsigned long)siz_xlev*siz_ylev;
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
struct {
	int x,y; /*начальная позиция*/
	int ly;	/*высота полоски*/
	int sx,sy; /**/
	int st;
	int typ;
} strip[maxstrip];
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
int end,pause,key_f,key_b;
int left,right,up,down,jump,fire;
int t_left,t_right,t_up,t_down,t_jump,t_fire;

unsigned int r_x,r_y; //прежние координаты лев. верхн угла окна

struct {
	int dev,mix,vol;
	int j;	//исп джойстик или нет
	int f;	//включить сзади фон или нет
	int cheat; //кряк - или нет
} setup;
int dev,mix,vol,state;

void main(void) {
	int i,j;
	int x,y,p_up,p_down,p_left,p_right;
	static char f_help[13]="help0.pic";

	//считаю конфиг из файла
	SetLib("graph");
	GetLib("pooh.cfg",(block)&stroka);
	_fmemcpy((block)&setup.dev,(block)&stroka,(unsigned long)sizeof(setup));
	vol=setup.vol; dev=setup.dev; mix=setup.mix;
	if (J.present)	//если есть жустик
		J.present=setup.j;

#ifdef EDITOR
	dev=255;
	speedraw=setup.f=3;
	G_lx=G_ly=6;
#else
	speedraw=setup.f;	//фон надо?
	//k.off();
	modinit();
	//moddevice( &dev );

#endif

#define needmem (380000+(dev==255?64000:140000))

	if ( coreleft()<needmem ) {
		sprintf(stroka,"Надо еще %lu байт ОЗУ\n"
		,needmem-coreleft());
		fatalerror(stroka);
	}
	modvolume(vol,vol,vol,vol);

#ifdef EDITOR
	J.present=0;
#else
	if ( J.present ) {
		printf("Калибровка 1го джойстика:\n"
			"Отцентрируйте джойстик и нажмите кнопку,\n"
			"Наклоните джойстик в левый верхний угол и нажмите кнопку,\n"
			"потом в правый нижний угол и нажмите кнопку.");
		if ( dev!=255 ) {
			//вкл музыку настройки джойстика
			modsetup("intro.fms", 4, 0 ,mix, dev, &state );
			modvolume(vol,vol,vol,vol);
		}
		J.init();
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
	curr_level=0; end=0;

	while ( end==0 ) {
		//пока не нажата какая-либо клавиша - засставка
		k.set(md);
		SetLib("mult"); //мульт на начало уровня
		mult("zast.rec");
		SetLib("graph");
		if ( end ) {
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
		end=0;
                while (gettic()<tic && end==0 /*&& m.left.press==0*/) {
			delay(500);
		}
		//if ( m.left.press ) end=1;

		PaletteOff(palette);
mainmenu:

		if ( dev!=255 ) {
			//выкл муз
			modstop();
		}

		if ( end ) {
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

			//меню
			for ( i=0; i<6; ++i ) {
				MoveXY(nmenx,nmeny+1+14*i);
				WPut(nmenx-1,nmeny-1+14*i,70,12);
				vputsc(tmenu[i]);
			}

			MoveXY(5,10);
			vputBs("Супер Винни-Пух",tfnt+144*4);
			MoveXY(22,170);
			vputBs("(C) 1995 FaMe",tfnt+144*1);
#ifdef DEMO
			MoveXY(10,192);
			vputsc("Это демонстрационная версия игры Супер Винни-Пух");
#endif
			MoveXY(10,192);
			vprint("Св.пам: %lu байт",(unsigned long) coreleft());

			PaletteOn(palette);
			//m.on();
			k.set(mz);

			if ( dev!=255 ) {
				//вкл музыку
				modsetup("intro.fms", 4, 0 ,mix, dev, &state );
				modvolume(vol,vol,vol,vol);
			}
			j=end=left=right=up=down=jump=fire=i=0;

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
				if ( end==1 ) {	//выход по еску
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
					end=0;
					break;
				case 1:
					//m.off();
					demo=0;
					MoveXY(nmenx+60,nmeny+14*1);
					k.off();
					stroka[0]=0;
					WPut(nmenx+55,nmeny+14*1-5,60,19);
					MoveXY(nmenx+60,nmeny+14*1);
					GetString(stroka,5);
					k.on();
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
						SetScreen(hidscr);
						f_game();
					}
					curr_level=0;
					end=0;
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
					str0[0]=i=left=right=end=0;
					do {
						PaletteOff(palette);
						f_help[4]='0'+(char)i;
						GetLibs(f_help,(block)hidscr);
						PutImg(0,0,320,200,(block)hidscr);
						PaletteOn(palette);
						while( left==0 && right==0 && end==0 );
						if( left && i>0) {
							i--;
						} else
						if( right && i<5) {
							i++;
						}
						left=right=0;
					} while( end==0 );
					PaletteOff(palette);
					end=1;
					goto mainmenu;

				case 4:	//об авторе
					//m.off();
					if ( dev!=255 ) {
						//выкл муз
						modstop();
					}
					end=0;
					mult("author.rec");
					end=1;
					goto mainmenu;

				case 5:
					//выход в дос
					end=1;
					//break;

			}


		} else if ( end==0 ) {
			//демонстрация
do_demo:
			//m.off();
			demo=1;

			//SetLib("graph");
			//GetLib("normal.col",palette);
			SetLib("mult");

			farfree(bckg); farfree(mnstrspr);
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
			bckg=famemalloc((unsigned long)320*200);
			mnstrspr=(block)famemalloc((unsigned long)320*200);
			level=(block)famemalloc(maxsiz_level);
			blevel=(block)famemalloc(maxsiz_level);

#ifndef DEMO
			if ( ++curr_level>4 ) {
#else
			if ( ++curr_level>2 ) {
#endif
				curr_level=1;
			}
			//m.on();
			f_game();
			end=0;
		}

	}
#endif
	fatalerror("Спасибо за игру!");
	return;
}

void readlev(void) {
	SetLib("levels");
	sprintf(levnum,"%02d",curr_level);
	for (int i=0; i<=9; ++i) {
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
	if( bckg!=NULL ) {
		GetLib(f_bckg,(block)bckg);
	}
	GetLib(f_dstrip,(block)tmp);
	readstrip();
	GetLib(f_dmonstr,(block)tmp);
	readmonstr();
}

void savelev(void) {
	SetLib("levels");
	sprintf(levnum,"%02d",curr_level);
	for (int i=0; i<=9; ++i) {
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

	k.set(mz);

	CurrentColor=draw=char_bkgd=31;
	l=i=j=end=fire=jump=down=left=right=up=
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
		if ( J.present ) {
			J.status();
			up=J.a.up;
			down=J.a.down;
			left=J.a.left;
			right=J.a.right;
			fire=J.a.a;
			jump=J.a.b;
		}

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
	char_bkgd=up=down=left=right=fire=end=jump=0;
	SetScreen(old);
	k.set(mm);
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
	k.off();
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

void do_strip(int sx=0) {
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