#include "joy.h"

void Joy::status(void) {
	jstat();
	Joy::a.x=xa;
	Joy::a.y=ya;
	Joy::b.x=xb;
	Joy::b.y=yb;
	Joy::a.a=a_1?0:1;
	Joy::a.b=a_2?0:1;
	Joy::b.b=b_1?0:1;
	Joy::b.b=b_2?0:1;
	//??ࠡ??稪 ?????⨪?
	a.left=a.x<a.fleft;
	a.right=a.x>a.fright;
	a.up=a.y<a.fup;
	a.down=a.y>a.fdown;
};

void Joy::init(void) {	//????ன?? ?????⨪?
        //??窠 ? 業???
	do {
		jstat();
	} while ( a_1 && a_2 );
        Joy::a.x=xa;
        Joy::a.y=ya;
	do {
		jstat();
	} while ( a_1==0 || a_2==0 );
        //??窠 ? ????? ??孥? 㣫?
	do {
		jstat();
	} while ( a_1 && a_2 );
	Joy::a.mleft=xa;
	Joy::a.mup=ya;
	do {
		jstat();
	} while ( a_1==0 || a_2==0 );
        //??窠 ? ?ࠢ?? ?????? 㣫?
	do {
		jstat();
	} while ( a_1 && a_2 );
	Joy::a.mright=xa;
	Joy::a.mdown=ya;
	do {
		jstat();
	} while ( a_1==0 || a_2==0 );

        Joy::a.fleft=(a.mleft+(a.x-a.mleft)/2);
        Joy::a.fright=(a.mright-(a.mright-a.x)/2);
        Joy::a.fup=(a.mup+(a.y-a.mup)/2);
        Joy::a.fdown=(a.mdown-(a.mdown-a.y)/2);
};
