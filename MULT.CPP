#include	<stdlib.h>
#include	<alloc.h>
#include	<string.h>

#include "mouse.h"
#include "famegraf.h"
#include "keys.h"
#include "all.h"

//extern Joy J;
extern Mouse m;
#ifdef EDITOR
#include "dialog.h"
extern Panel p;
#endif
extern char str0[25];

extern long tic; //для задержки по таймеру
extern screen hidscr; //скрытая страничка
extern block tfnt;     //под большие шрифты
screen text;
extern char far stroka[255];
extern int end;

void mult(char * name) {
	//считывание описания мульта и показ
	block txt,tmp;
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

	NormalScreen();
	//считаем сценарий
	text=famemalloc(SizeLib(name)+8);
	_fmemset(text,0,SizeLib(name)+8);
	GetLib(name,(block)text); txt=(block)text;

	n=1; end=0;
	while(n>=1 && end==0) {

		//считаем очередн строку
		memcpy((block)&stroka,txt,80);
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
			while (gettic()<tic && end==0);
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
			} while (gettic()<tic && end==0);
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
				while (gettic()<tic && end==0);
			} while ( --i>0 && end==0 );
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
				delay(j);
				if ( ++i>=k ) i=0;
			} while (gettic()<tic && end==0);
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
				delay(j);
				if ( ++i>=k ) {
					i=0;
					if ( (--n)<=0 ) {
						n=1;
						break;
					}
				}
			} while (end==0);
		} else
		if (strnicmp(str1,"TXT",3)==0) {
			//вывод текста без просветов
			MoveXY(x,y); tmp=txt;
			while ( *tmp && *tmp!='@' ) ++tmp; ++tmp;
			while ( *tmp && *tmp!='@' && end==0) {
				if ( *tmp=='#' ) {
					tic=gettic()+18;
					while (gettic()<tic && end==0);
				} else
				if ( *tmp=='\n' ) {
					MoveXY(x,(y+=9));
				} else {
					PutCh((str1[3])?CharUp(*tmp):(*tmp));
					delay(50);
				}
				tmp++;
			}
		} else
		if (strnicmp(str1,"FTX",3)==0) {
			//вывод теста без просветов и без пауз
			MoveXY(x,y); tmp=txt;
			while ( *tmp && *tmp!='@' ) ++tmp; ++tmp;
			while ( *tmp && *tmp!='@' && end==0) {
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
			while ( *tmp && *tmp!='@' && end==0) {
				putBch(x,y,(str1[4])?CharUp(*tmp):(*tmp),tfnt);
				x+=24; ++tmp;
				delay(50);
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
		while ( ((*txt>>27) || (*txt==9)) && (txt < tail) ) ++txt;
		 while ( *txt<27 ) ++txt;
	}
m_exit:
	if ( end ) {	//если прервали
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