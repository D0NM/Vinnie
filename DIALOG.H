#ifndef H_DIALOG
#define H_DIALOG
#include "famegraf.h"
#define MaxPanel 32

class Panel {
	struct {
		int x;
		int y;
		int lx;
		int ly;
		char ico;
		char typ;
		char * txt;
		block pic;
		void (*prog)(void);
		char status;
	} pan[MaxPanel];

public:
	int num;
	int last,current;

	Panel() {
		init();
		last=-1;
		current=-1;
	}

	void init(void); //начальн инициализация
	void show(void); //показать все кнопки на экране
	void show(int); //показать кнопку под номером
	int add(int,int,int,int,char *txt=NULL,block pic=NULL,void (*prog)()=NULL, ...);
	//добавить кнопку
	int addr(int,int,int,int, char *txt=NULL, void (*prog)()=NULL, ...);
	//добавить область
	void del(int); //удалить кнопку или область
	int press(void); //вызов пп по кнопке и отрабокка нажатий
	//возвращает номер нажат кнопки или области
};

#endif
