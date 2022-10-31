#ifndef H_JOY
#define H_JOY

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Графические переменные
extern char far	a_1;
extern char far	a_2;
extern char far	b_1;
extern char far	b_2;

extern int far	xa;
extern int far	ya;
extern int far	xb;
extern int far	yb;

extern void far jstat(void);

#ifdef __cplusplus
}
#endif


struct manipulator {
	int x;
	int y;
	char a;
	char b;
	unsigned char left,right,up,down;	//положение жустика
	unsigned int fleft,fright,fup,fdown;	//пороги чувствительности
	unsigned int mleft,mright,mup,mdown;	//мах значения
};

class Joy {
private:

public:
	manipulator a;
	manipulator b;
	unsigned char present;	//кол-во джойстиков

	Joy(void) {
		jstat();
		present=0;
		if (xa || ya)
			present=1;
		if (xb || yb)
			present=2;
	}

	~Joy(void) {
	}

	void status(void);

	void init(void);	//настройка джойстика
};

#endif
