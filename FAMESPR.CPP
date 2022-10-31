#include "famegraf.h"
#include <alloc.h>
#include <stdlib.h>

#define MaxSprite 64

screen SpritesScreen;

// Определение функций для работы со спрайтами
sprites sprite[MaxSprite]; //создание структуры для спрайтов

void InitSpr(void) { //начальная инициализация
	int  i;

	SpritesScreen=farmalloc((unsigned long)320*200);

	for (i=0; i<MaxSprite; ++i) {
		sprite[i].map=NULL; sprite[i].on=0;
		sprite[i].ox=sprite[i].oy=0;
		sprite[i].lx=sprite[i].ly=1;
	}
	return;
}

int CreateSpr(int lx,int ly, block map) { //создать спрайт
     int  i;
     for (i=0; i<MaxSprite; ++i) {
	  if (sprite[i].map==NULL) {
	       break;
	  }
     }
     sprite[i].map=map; sprite[i].on=1;
     sprite[i].ox=sprite[i].x=sprite[i].oy=sprite[i].y=0;
     sprite[i].lx=lx; sprite[i].ly=ly;
     return i;
}

void ClearSpr(int num) { //удалить спрайт из списка
     sprite[num].map=NULL;
     return;
}

void SprOn(int num) {
     sprite[num].on=1;
     return;
}

void SprOff(int num) {
     sprite[num].on=0;
     return;
}

void MoveSpr(int num, int x, int y) {
     sprite[num].x=x;
     sprite[num].y=y;
     return;
}

void SetSpr(int num, block buf) {
     sprite[num].map=buf;
     return;
}

void ShowSprites(void) {
	int  i;
	for (i=0; i<MaxSprite; ++i) {
		if (sprite[i].map!=NULL && sprite[i].on!=0)
			PutCMas(sprite[i].x,sprite[i].y,sprite[i].lx,sprite[i].ly,sprite[i].map);
	}
	return;
}

void EraseSprites(void) {
	int  i;
	//if (SpritesScreen==NULL)
	//	return;
	for (i=0; i<MaxSprite; ++i) {
		if (sprite[i].map!=NULL && sprite[i].on!=0)
			CopyCBlock(sprite[i].x,sprite[i].y,sprite[i].lx,sprite[i].ly,CurrentScreen,sprite[i].x,sprite[i].y,SpritesScreen);
	}
	return;
}
