#include	<conio.h>
#include	<stdlib.h>
#include	<bios.h>
#include	<alloc.h>
#include	<string.h>

#include "famegraf.h"
#include "joy.h"
#include "mouse.h"
#include "keys.h"
#include "all.h"

extern Joy J;
extern Mouse m;
#ifdef EDITOR
#include "dialog.h"
extern Panel p;
#endif

extern long tic; //для задержки по таймеру

//void splitcopy(int x, int y, int lx, int ly,screen dest, int sx, int sy, screen src);

extern struct {
	int dev,mix,vol;
	int j;	//исп джойстик или нет
	int f;	//включить сзади фон или нет
	int cheat; //кряк - или нет
} setup;


extern block demobuf;
extern char demo; //демонстр 0выключена 1-включена
extern unsigned int pdemo;

extern block menu;
extern block ind; //под разные значки и индикаторы

extern block trase;
extern block level;
extern block blevel;

extern block bfon;
extern block fon;
extern block manspr; //образы героя

extern block tfnt;

extern block mnstrspr; //образы гадов
extern unsigned long pmnspr; //последний свободн байт

extern block items;
extern block additems;
extern block modelspr;
extern block lifts;
extern block tmp;

extern block obloka; //под взрывы
extern block enbull;

extern screen bckg; //задний фон
extern screen hidscr; //скрытая страничка

//extern char serial_pc; // 1- обычная писишка, 0 - Trident 512k
extern char speedraw; // 1 - для быстрой машины, 0 - медленная
extern char draw; //отрисовывать ли сейчас что-либо?

extern char far stroka[255];
extern char levnum[4];
extern int curr_level;

extern char files[9][13];

extern levdef leveldef;

//размер уровня
extern unsigned int siz_xlev;
extern unsigned int siz_ylev;
extern unsigned long siz_level;

extern char poc[32][16];

extern char nappoc[32];
extern char napypoc[32];

extern int x,y,sx,sy; //тек положение точки
extern unsigned char far *poslab; //тек поз точки в массиве трассы
extern unsigned char far *poslal; //тек поз точки в массиве фона
extern unsigned char tekblock;

//герой
extern hero man;

extern int coins; //монетки
extern int hearts; //сердечки
extern int keys; //ключи
extern int beams; //шишки
extern int shields; //защита
extern int power;	//энергия
extern int times; //время
extern unsigned long score,hiscore;
extern int continues; //продолжения
extern char maxjump; //высота прыжка
extern char maxspeed;	//максимальная скорость
extern char glass;	//очки
extern char shboots;	//шиповки
extern char spboots;	//скоростные башмаки
extern char jetpack;	//ранец
extern char kolun;	//колун

extern char boss;    //признак активности БОССА

extern object obj[maxobj];
extern strkobj kobj[maxobj];

extern boom booms[maxboom];
extern strbullets bullets[maxbullet];

extern char waitboom;
extern char waitfire;
extern unsigned char waitmess;
extern int messy;
extern char messsy;
extern char * mess;

extern monstr monsters[maxmonstr];
extern int nmonstr; //текущ кол-во видов монстров

extern strmodels models[maxmodel];

extern sshop shop[11];

extern strup upput[maxnup];
extern int nup; //кол-во элементов сверху

extern unsigned char curr_fon;
extern unsigned char curr_trase;
extern unsigned char curr_items;
extern unsigned char curr_monstr;
extern char curr_model;

//режимы редактора
extern unsigned char st_fon;		 //редактировать фон
extern unsigned char st_trase;	//редактировать трассу
extern unsigned char st_items;	//редактировать вещи
extern unsigned char st_monstr;       //установка на поле монстров
extern unsigned char st_model;       //установка на моделей

extern unsigned int xw;
extern unsigned int yw;

extern int end,pause,key_f,key_b;
extern int left,right,up,down,jump,fire;
extern int t_left,t_right,t_up,t_down,t_jump,t_fire;
extern unsigned int r_x,r_y; //прежние координаты лев. верхн угла окна

extern int dev,mix,vol,state;

extern int striply;
void show_strip(int y);
void do_strip(int sx);


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
						end=1;
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
		end=c;
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

void far md(unsigned char butt) {
	//обработка прерывания по клавиатуре
	//для демонстрации
	end=1;
}

//=============================================================

void f_game(void) { //проба скролла и объектов
	static int x,y,sx,sy,i,scrsdvig,scrsdvig_;
	Keys k=Keys(mm);

#ifdef EDITOR
	m.off();
#endif

	if ( demo ) {
		//для демонструхи другой обработч клавишь
		k.set(md);
	} else {
		PaletteOff(palette);
	}

	SetLib("graph");
	GetLib("normal.col",palette);

	shields=kolun=glass=shboots=jetpack=end=score=0; //монетки,продолжения
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
			for (int i=0; i<=8; ++i) {
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
			if ( demo==0 && curr_level>1) {
				MoveXY(26,160);	makepass(curr_level);
				vprintB(tfnt+144*3,"ПАРОЛЬ: %s",stroka);
				tic=gettic()+18*20;
			} else {
				tic=gettic()+18*3;
			}

			farfree(bckg); farfree(mnstrspr);
			farfree(level); farfree(blevel);
			if ( dev!=255 ) {
				//вкл музыку
				modsetup(leveldef.musicname, 4, 0 ,mix, dev, &state );
				modvolume(vol,vol,vol,vol);
			}
			PaletteOn(palette);

			end=0;while (gettic()<tic && end==0);

			PaletteOff(palette);

			if ( demo==0 ) {
				SetLib("mult"); //мульт на начало уровня
				mult(leveldef.mname0);
			}
			bckg=famemalloc((unsigned long)320*200);
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
			GetLib("rama.bin",(block)hidscr);
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

		while ( end==0 ) {

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
			if ( J.present ) {
				J.status();
				up=J.a.up; down=J.a.down;
				left=J.a.left; right=J.a.right;
				fire=J.a.a; jump=J.a.b;
			}

			if ( demo ) {
				//если демонстрация, то расшифруем
				i=demobuf[pdemo++];
				left=i&1;	right=i&2;
				up=i&4;         down=i&8;
				jump=i&16;      fire=i&32;
				end=i&64;
				if ( pdemo>=maxdemo ) {
					end=1;
				}
			} else {
				//иначе пишем демонстрацию
				if ( pdemo<maxdemo ) {
					demobuf[pdemo++]=left|(right<<1)|(up<<2)|(down<<3)|(jump<<4)|(fire<<5)|(end<<6);
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
					delay(50);
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
							end=1;
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
				PutMas(56,56,32,32,man.stspr);
				MoveXY(130,50);
				vputs("ПОДОЖИТЕ 15 СЕКУНД!");
				MoveXY(100,80);
				vputs("Это демонстрационная версия!");
				MoveXY(34,110);
				vputs("Автор: Михаил Братусь, тел.(351-35) 2-14-60");
				MoveXY(40,120);
				vputs("456321 г. Миасс ул. Вернадского д.4, кв.52");
				MoveXY(40,130);
				vputs("E-mail: migel@intech.telecom.chel.su");
				MoveXY(40,140);
				vputs("(c) 1995 FaMe SoftWare by BMV");
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
			end=hearts=0;
		} else if ( man.s==s_over ) {
			//подох
			end=0;

			//убавляем жизнь
			if (--hearts<=0) {
				//OVER?
				NormalScreen(); Cls(0);
				Clip(0,0,319,199);
				MoveXY(23,170);
				if ( continues ) {
					SetLib("mult");
					mult("continue.rec");
					vprintB(tfnt+144*3,"Продолжение %1u",continues);
				} else {
					SetLib("mult");
					mult("theend.rec");
				}
				//k.set(md);
				if ( dev!=255 ) {
					modsetup("intro.fms", 4, 0 ,mix, dev, &state );
					modvolume(vol,vol,vol,vol);
					PaletteOn(palette);
					if ( continues ) {
						tic=gettic()+18*8;
					} else {
						tic=gettic()+18*15;
					}
					end=0;
					while (gettic()<tic && end==0);
					//выкл муз
					modstop();
				} else {
					PaletteOn(palette);
					delay(2000);
				}
				PaletteOff(palette);
			}

		} else if ( man.s==s_end ) {
			SetLib("mult"); //мульт на конец уровня
			farfree(bckg); farfree(mnstrspr);
			farfree(level); farfree(blevel);
			if ( dev!=255 ) {
				//вкл музыку
				modsetup(leveldef.musicname, 4, 0 ,mix, dev, &state );
				modvolume(vol,vol,vol,vol);
			}
			mult(leveldef.mname1);
			if ( dev!=255 ) {
				//выкл муз
				modstop();
			}
			bckg=famemalloc((unsigned long)320*200);
			mnstrspr=(block)famemalloc((unsigned long)320*200);
			level=(block)famemalloc(maxsiz_level);
			blevel=(block)famemalloc(maxsiz_level);
			//на следующь уровень

			if ( leveldef.end==0 ) {
				//инкремент уровня
				++curr_level;
				end=shields=0; //убираем щит, если есть
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
				farfree(bckg); farfree(mnstrspr);
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
				bckg=famemalloc((unsigned long)320*200);
				mnstrspr=(block)famemalloc((unsigned long)320*200);
				level=(block)famemalloc(maxsiz_level);
				blevel=(block)famemalloc(maxsiz_level);
#ifdef EDITOR
				//GetLib("normal.col",palette);
				SetScreen(hidscr);
#endif
				continues=end=hearts=0;
			}
		} else {
			//прерываение игры
			continues=end=hearts=0;
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
							Swap(&md->x2,&md->x1);
							Swap(&md->y2,&md->y1);
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
								end=1;
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