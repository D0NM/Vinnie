#include	<conio.h>
#include	<stdlib.h>
#include	<bios.h>
#include	<alloc.h>
#include	<string.h>
#include	<time.h>

#include "famegraf.h"
#include "mouse.h"
#include "joy.h"
#include "keys.h"

Joy J;
Mouse m;
void far mz(unsigned char); //обработчик для заставки
Keys k=Keys(mz);
long tic;
int end,pause,key_f,key_b;
int left,right,up,down,jump,fire;

int dev=255,mix=8000,vol=255,stat;


struct {
	int dev,mix,vol;
	int j;	//исп джойстик или нет
	int f;	//скорость вывода на экран
	int cheat;
} setup;

#define needmem (380000+(dev==255?0:150000))

int pos=0;


static char *tmenu[]={
" Компьютер",
" Звуковое устройство",
" Детализация",
" Управление",
" Выход + запись"
};

static char cmenu[]={
0,
6,
0,
0
};

static char mmenu[]={
5,
7,
4,
2
};

static char *typ_cpu[]={
"PC AT 12 Mhz ",
"386 SX 33 Mhz ",
"386 DX 40 Mhz ",
"486 DX2 50 Mhz ",
"Pentium 100 Mhz "
};

static char *typ_speed[]={
"Нет ",
"Плохая ",
"Хорошая ",
"Отличная "
};

static char *typ_control[]={
"Клавиатура ",
"1й джойстик ",
"Клавиатура (Нет джойстика) ",
};

static char *typ_sound[]={
"Внутренний динамик ",
"COVOX в LPT1 ",
"COVOX в LPT2 ",
"COVOX в LPT3 ",
"Sound Blaster (220h) ",
"Sound Blaster (240h) ",
"Без звука "
};

static char cod_sound[]={
0,
1,
2,
3,
7,
8,
255
};

screen bckg; //задний фон
screen hidscr; //скрытая страничка

void showem(void);

void main(int argc, char *argv[]) {
	int i,j;
	char t[10]="y";
	//clrscr();
	/*modinit();
	moddevice( &dev );
	printf("dev: %u",dev);
	getch();*/
	setup.dev=dev;
	setup.vol=vol;
	setup.mix=mix;

	SetLib("graph");
	//printf("Интеллектуальная установка игры Супер Винни-Пух v2.01\n");

	InitGraph();
	GetPalette(palette);
	PaletteOff(palette);
	Vga256();
	GetLib("normal.fit",fonts);
	PutFont(fonts);
	GetLib("normal.col",palette);
	for ( i=0; i<256*3; ++i )
		palette1[i]=0;
	PutPalette(palette1);

	//obloka=(block)famemalloc(SizeLib("obloka.bin"));
	//GetLib("obloka.bin",(block)obloka);

	bckg=(block)famemalloc(SizeLib("reclama.pic"));
	GetLib("reclama.pic",(block)bckg);

	hidscr=(block)famemalloc(SizeLib("reclama.pic"));

	SetScreen(hidscr);
	Clip(0,0,319,199);


	pos=end=left=right=up=down=jump=fire=0;

	showem();
	PaletteOn(palette);

main_menu:
	showem();
	jump=fire=0;
	while ( fire==0 && jump==0 ) {

		//m.off();
		//Rectangle(nmenx-2,nmeny-2+14*i,72,14);
		//m.on();
		//tic=gettic()+2;
		//while (gettic()<tic) {
		//	delay(100);
		//}
		//CurrentColor=wfon;
		//m.off();
		//Rectangle(nmenx-2,nmeny-2+14*i,72,14);
		//m.on();
		tic=gettic()+2;
		while (gettic()<tic) {
			delay(100);
		}

		if ( up || left ) {
			up=left=0;
			if (--pos<0 ) {
				pos=4;
			}
			showem();
		}
		if ( down || right ) {
			down=right=0;
			if (++pos>4 ) {
				pos=0;
			}
			showem();
		}
		if ( end==1 ) {	//выход по еску
			fire=pos=4;
			showem();
		}
	}
	switch ( pos ) {
		case 0:
			cmenu[pos]++;
			if ( cmenu[pos]>=mmenu[pos] ) {
				cmenu[pos]=0;
			}
			switch ( cmenu[pos] ) {
				case 0:
					if ( cmenu[2]>1 ) {
						cmenu[2]=1;
					}
					setup.mix=0;
					cmenu[1]=6;
					break;
				case 1:
					if ( cmenu[2]>2 ) {
						cmenu[2]=2;
					}
					setup.mix=5000;
					break;
				case 2:
					if ( cmenu[2]>2 ) {
						cmenu[2]=2;
					}
					setup.mix=10000;
					break;
				case 3:
					cmenu[2]=3;
					setup.mix=16000;
					break;
				case 4:
					cmenu[2]=3;
					setup.mix=22000;
					break;
			}
			break;
		case 1:
			cmenu[pos]++;
			if ( cmenu[pos]>=mmenu[pos] ) {
				cmenu[pos]=0;
			}
			break;
		case 2:
			cmenu[pos]++;
			if ( cmenu[pos]>=mmenu[pos] ) {
				cmenu[pos]=0;
			}
			break;

		case 3: //джойстик
			if ( J.present ) {
				cmenu[pos]++;
				if ( cmenu[pos]>=mmenu[pos] ) {
					cmenu[pos]=0;
				}
			} else {
				cmenu[pos]=2;
			}
			break;

		case 4:
			if ( end ) {
				fatalerror("Изменений нет");
			}

			//выход в дос с записью
			if( stricmp(argv[1],"cheat")==0 ) {
				setup.cheat=1;
				//puts("(c) 1995 FaMe by BMV");
			}

			setup.dev=cod_sound[cmenu[1]];
			//исп джойстик или нет
			setup.j=(cmenu[3]==1)?1:0;
			//скорость вывода на экран
			setup.f=cmenu[2];

			PutLib("pooh.cfg",(block)&setup,(unsigned long)sizeof(setup));
			PackLib();

			fatalerror("Установка закончена. Спасибо за покупку!\n"
			"Для запуска игры наберите POOH и нажмите ENTER\n");

	}

	goto main_menu;
}

#ifdef FIGNA
typ_pc:
	printf("\n"
	"1.   IBM PC/XT\n"
	"2.   PC/AT 286\n"
	"3.   386SX (33 Mhz)\n"
	"4.   386DX (40 Mhz)\n"
	"5.   486DX/2 (50 Mhz)\n"
	"6.   PENTIUM\n"
	"7.   лучше чем PENTIUM\n"
	"8.   APPLE\n"
	"9.   LADA\n"
	"0.   Нет машины\n"
	"\nВыберите тип вашей машины: ");

	t[0]=getche();
	setup.f=-1;

	printf("\n");

	switch ( t[0] ) {
	case '1':	//ХТ
		printf("Жаль... но игра на такой машине не может работать\n"
		"ПРЕДЛОЖЕНИЕ: избавьтесь от этой рухляди и купите 286\n");
		exit(1);

	case '2':	//286
		//без звука и фона
		setup.dev=dev=255;
		setup.f=-2;
		printf("Тише едешь - дальше будешь\n"
		"Игра возможна только БЕЗ звука и заднего фона\n"
		"ПРЕДЛОЖЕНИЕ: продайте 286 и купите 386\n");
		goto set_speedraw;

	case '3':	//386sx
		mix = 5000;
		printf("Качество музыки на такой машине неприятно удивляет...\n"
		"ПРЕДЛОЖЕНИЕ: продайте SX и купите DX\n");
		break;

	case '4':	//386dx
		mix = 7000;
		printf("Так вот какая она... средняя машина!\n"
		"ПРЕДЛОЖЕНИЕ: продайте 386DX и купите 486SX\n");
		break;

	case '5':	//486dx
		mix = 10000;
		setup.f=3;
		printf("Приятной вам игры!\n"
		"ПРЕДЛОЖЕНИЕ: продайте 486DX и купите PENTIUM\n");
		break;

	case '6':
		mix = 16000;
		setup.f=3;
		printf("Приятной вам игры!\n"
		"ПРЕДЛОЖЕНИЕ: продайте PENTIUM и махните на месяц на Бермуды...\n");
		break;

	case '7':
		mix = 30000;
		setup.f=3;
		printf("Такого чистого звука вы не слышали даже в опере!\n"
		"ПРЕДЛОЖЕНИЕ: продайте все что у вас есть и положите деньги\n"
		"в швейцарский банк...\n");
		break;

	case '8':
		printf("Ах APPLE...\n"
		"ПРЕДЛОЖЕНИЕ: Сыграйте ка на нем в SUPER MARIO BROS!\n");
		exit(1);
	case '9':
		printf("Перед игрой не забудьте пристегнуть ремни!\n"
		"ПРЕДЛОЖЕНИЕ: продайте вашу LADA и купите PENTIUM!\n");
		exit(1);
	case '0':
		clrscr();
		printf("WARNING!!! CPU NOT FOUND!\n"
		"Press any key to continue\n");
		getch();
		exit(1);

	default:	//нет такого типа
		printf("Вы ошиблись кнопкой!\n\n");
		goto typ_pc;
	}

set_snd:
	printf("\n");
	//dev=255;
	//modinit();
	//moddevice( &dev );
	setup.dev=dev;
	setup.vol=vol;
	setup.mix=mix;

/*
	if ( dev!=255 ) {
		modsetup("fun.mod", 4, 0 ,mix, dev, &stat );

		printf("\nГромкость музыки во время игры (10...255):");
		scanf("%d",&vol);
		setup.vol=vol;
		modvolume(vol,vol,vol,vol);

		printf("\nЧастота воспроизведения музыки во время игры (8000...16000):");
		scanf("%d",&mix);
		setup.mix=mix;
		modstop();
		modsetup("fun.mod", 4, 0 ,mix, dev, &stat );
	}

	if ( dev!=255 ) {
		//выкл муз
		modstop();
	}
*/
set_speedraw:
	if( setup.f>=0 ) {
		goto set_jst;
	}
	printf("\n"
	"1.   Движение экрана рывками без заднего фона\n"
	"2.   Плавное движение экрана без заднего фона\n"
	"3.   Движение экрана рывками + задний фон\n");
	if( setup.f!=-2 )
	printf(
	"4.   Плавное движение экрана + задний фон\n");
	printf(
	"\nВыберите качество вывода графики: ");

	t[0]=getche();
	printf("\n");

	switch ( t[0] ) {
	case '1':
		setup.f=0;
		break;
	case '2':
		setup.f=1;
		break;
	case '3':
		setup.f=2;
		break;
	case '4':
		setup.f=3;
		break;
	default:	//нет такого типа
		printf("Вы ошиблись кнопкой!\n\n");
		goto set_speedraw;
	}

set_jst:
	if ( J.present ) {
		printf("\nИспользовать в игре 1й джойстик? (д,y/н,n):");
		t[0]=getche();
		setup.j=(t[0]=='y' || t[0]=='Y' || t[0]=='д' || t[0]=='Д');
	}

	clrscr();
	printf("Установка закончена. Спасибо за покупку!\n"
	"Для запуска игры наберите POOH и нажмите ENTER\n");
	if( stricmp(argv[1],"cheat")==0 ) {
		setup.cheat=1;
		puts("(c) 1995 FaMe by BMV");
	}
	PutLib("pooh.cfg",(block)&setup,(unsigned long)sizeof(setup));
	PackLib();
	return;
}
#endif

void fatalerror(char *t) {
	//выход по фатальной ошибке
	SetLib("");
	CloseGraph();
	puts(t);
	k.off();
	exit(1);
}

void far mz(unsigned char butt) {
	//обработка прерывания по клавиатуре
	if ( butt==1 ) //Esc
		end=1;
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

void showem(void) {
	int i;
#define nmenx 18
#define nmeny 5 //80

	//меню
	PutImg(0,0,320,200,(block)bckg);
	WPut(0/*nmenx-5*/,nmeny-5,320/*294*/,8*5+16);
	for ( i=0; i<5; ++i ) {
		char_fgd=(i==pos)?17:31;
		char_bkgd=0;
		MoveXY(nmenx,nmeny+1+9*i);
		vputs(tmenu[i]);
		if ( i<4 ) {
		vputs(" : ");
		}
		switch ( i ) {
			case 0:
			vputs(typ_cpu[cmenu[i]]);
			break;

			case 1:
			vputs(typ_sound[cmenu[i]]);
			break;

			case 2:
			vputs(typ_speed[cmenu[i]]);
			break;

			case 3:
			vputs(typ_control[cmenu[i]]);
			break;
		}
	}
	CopyToScreen(hidscr);
}