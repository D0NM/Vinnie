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

extern long tic; //??? ????প? ?? ⠩????

//void splitcopy(int x, int y, int lx, int ly,screen dest, int sx, int sy, screen src);

extern struct {
	int dev,mix,vol;
	int j;	//??? ?????⨪ ??? ???
	int f;	//???????? ᧠?? 䮭 ??? ???
	int cheat; //???? - ??? ???
} setup;


extern block demobuf;
extern char demo; //???????? 0?몫?祭? 1-????祭?
extern unsigned int pdemo;

extern block menu;
extern block ind; //??? ࠧ??? ???窨 ? ??????????

extern block trase;
extern block level;
extern block blevel;

extern block bfon;
extern block fon;
extern block manspr; //??ࠧ? ?????

extern block tfnt;

extern block mnstrspr; //??ࠧ? ?????
extern unsigned long pmnspr; //??᫥???? ᢮???? ????

extern block items;
extern block additems;
extern block modelspr;
extern block lifts;
extern block tmp;

extern block obloka; //??? ??????
extern block enbull;

extern screen bckg; //?????? 䮭
extern screen hidscr; //??????? ??࠭?窠

//extern char serial_pc; // 1- ???筠? ????誠, 0 - Trident 512k
extern char speedraw; // 1 - ??? ????ன ??設?, 0 - ?????????
extern char draw; //????ᮢ뢠?? ?? ᥩ??? ???-?????

extern char far stroka[255];
extern char levnum[4];
extern int curr_level;

extern char files[9][13];

extern levdef leveldef;

//ࠧ??? ?஢??
extern unsigned int siz_xlev;
extern unsigned int siz_ylev;
extern unsigned long siz_level;

extern char poc[32][16];

extern char nappoc[32];
extern char napypoc[32];

extern int x,y,sx,sy; //⥪ ????????? ??窨
extern unsigned char far *poslab; //⥪ ??? ??窨 ? ???ᨢ? ??????
extern unsigned char far *poslal; //⥪ ??? ??窨 ? ???ᨢ? 䮭?
extern unsigned char tekblock;

//??ன
extern hero man;

extern int coins; //????⪨
extern int hearts; //??थ窨
extern int keys; //?????
extern int beams; //??誨
extern int shields; //??????
extern int power;	//???ࣨ?
extern int times; //?६?
extern unsigned long score,hiscore;
extern int continues; //?த???????
extern char maxjump; //?????? ??릪?
extern char maxspeed;	//???ᨬ??쭠? ᪮?????
extern char glass;	//?窨
extern char shboots;	//訯????
extern char spboots;	//᪮??????? ??謠??
extern char jetpack;	//࠭??
extern char kolun;	//?????

extern char boss;    //?ਧ??? ??⨢????? ?????

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
extern int nmonstr; //⥪?? ???-?? ????? ?????஢

extern strmodels models[maxmodel];

extern sshop shop[11];

extern strup upput[maxnup];
extern int nup; //???-?? ??????⮢ ᢥ???

extern unsigned char curr_fon;
extern unsigned char curr_trase;
extern unsigned char curr_items;
extern unsigned char curr_monstr;
extern char curr_model;

//०??? ।??????
extern unsigned char st_fon;		 //।????஢??? 䮭
extern unsigned char st_trase;	//।????஢??? ??????
extern unsigned char st_items;	//।????஢??? ????
extern unsigned char st_monstr;       //??⠭???? ?? ???? ?????஢
extern unsigned char st_model;       //??⠭???? ?? ???????

extern unsigned int xw;
extern unsigned int yw;

extern int end,pause,key_f,key_b;
extern int left,right,up,down,jump,fire;
extern int t_left,t_right,t_up,t_down,t_jump,t_fire;
extern unsigned int r_x,r_y; //?०??? ???न???? ???. ????? 㣫? ????

extern int dev,mix,vol,state;

void doman() { //??ࠢ????? ??஥?
	static int oldx,oldy,i; //???? ???? ?????
	static int dw1,dw2,lf1,lf2,rt1,rt2,up1,up2; //???? ??????
	static int dwi1,dwi2,lfi1,lfi2,rti1,rti2,upi1,upi2; //???? ??饩
	static int sx1,sx2;

	oldx=man.x; oldy=man.y;

	//?ਡ???? ?᪮७?? ?? ????
	man.x+=(man.sx+(man.dsx2+man.dsx1)/2); //max!!!!
	man.y+=(man.sy+man.dsy);

	//??-?? ?? ??室 ?? ?࠭??? ????ਭ??
	if (man.x<0) man.x=0;
	if (man.x>siz_xlev*16-man.lx) man.x=siz_xlev*16-man.lx-1;
	if (man.y<man.ly) man.y=man.ly;

	//??? ??ࠢ?, ᫥?? ? ???????
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
	//㧭??? ??? ??? ????
	dw1=checkdw(man.x+man.dwx1,man.y-man.dwy);
	dwi1=tekblock; sx1=sx;
	dw2=checkdw(man.x+man.dwx2,man.y-man.dwy);
	dwi2=tekblock; sx2=sx;

	if (++man.f0>6-abs(man.sx+max(man.dsx1,man.dsx2))) {
		man.f0=0; man.f++;
	}

	if ( man.fw>0 )	{
		//?᫨ ???? ????প? ? ᬥ?? 䠧
		--man.fw;
	} else
	switch ( man.s ) {
		case s_go:
			//????
			if ( abs(man.sx+(man.dsx1+man.dsx2)/2)>1 ) {
				if ( man.f>=man.maxgo ) man.f=0;
				man.buf=man.gospr+man.f*man.lx*man.ly;
				man.fw=1;
			} else {
				//????????
				if ( dw1==0 && sx1<8 && ((dw2>15 && dw2<20) || dw2==1) ) {
					//???? ᫥??
					man.napr=0;
					man.buf=man.specspr+man.lx*man.ly*(1+(waitboom&1));
					man.fw=2;
					if( (waitboom&15)==0 ) {
						addboom(man.x+12,man.y-man.ly,8,8,0,-1,b_buh,obloka+256*10+32*32*10+128*5+64*2);
					}

				} else if ( dw2==0 && sx2>8 && ((dw1>15 && dw1<20) || dw1==1) ) {
					//???? ??ࠢ?
					man.napr=1;
					man.buf=man.specspr+man.lx*man.ly*(1+(waitboom&1));
					man.fw=2;
					if( (waitboom&15)==0 ) {
						addboom(man.x+12,man.y-man.ly,8,8,0,-1,b_buh,obloka+256*10+32*32*10+128*5+64*2);
					}
				} else
				//?⮨?
				if ( up ) {
				//ᬮ???? ?????
					man.buf=man.stspr+man.lx*man.ly;
				} else if ( down ) {
				//????
					man.buf=man.stspr+2*man.lx*man.ly;
				} else {
					//?⮨?
					if ((waitboom&31)!=0)
						man.buf=man.stspr;
					else {
						//??࣠??? ????
						man.buf=man.specspr;
						man.fw=2;
					}
				}
				man.f=0;
			}
			break;

		case s_jump:
		case s_down:
			//??릮?
			if ( man.sy<-2 ) {
				man.f=0;
			} else if ( man.sy<2 ) {
				man.f=1; man.fw=4;
			} else {
				man.f=2;
			}
			if ( man.sy>7 ) {	//??? ᨫ쭮? ???????
				man.buf=man.stspr+3*man.lx*man.ly;
				man.fw=2;
			} else {
				//䠧? ??? ??릪?
				man.buf=man.jmspr+man.f*man.lx*man.ly;
			}
			break;

		case s_ouch:
		case s_crash:
			//???-?? ???ᨫ
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
			if ( power==0 && man.sy<=0) {	//?᫨ ???稫??? ????祥
				++man.sy;
			}
			if( man.sy<0 ) {
				//??? ?? ⮯??
				addboom(man.napr?(man.x):(man.x+16),man.y-8,16,16,-man.sx,1,b_boom,obloka+256*10);
			}
			break;

		default:
			//????
			man.buf=man.stspr;
			break;
	}

	if ( man.s!=s_crash ) { //??? ???⢥?? ?? ???? ⠪?? ?஢?ப!

		//???᪨????? ?㤠 ?? ????
		if ( (lf1==16 || rt1==16) && dw2==16 && dw1==16 && up1==16 && up2==16 ) {
			man.x=oldx;
			man.sx=man.dsx1=man.dsx2=0;
			man.y=oldy;
			man.sy/=2;
		}

		//?⮫???????? ?? ?⥭???
		if ( ((lf1==16 || lf2==16) && man.sx<0 ) || ((rt1==16 || rt2==16) && man.sx>0)) {
			man.x=oldx;
			man.sx=man.dsx1=man.dsx2=0;
		}

		//?஢?ઠ ?? ᮯਪ?᭮????? ? ?????ࠬ?
		if ( shields && power ) {
		} else
		if (man.myrg==0 ) {//?᫨ ?㦨? ?? ??࣠??
			for ( i=0; i<maxobj; ++i ) {
				if ( obj[i].on!=0 && obj[i].myrg==0) {
					//?᫨ ?????? ??? ? ?? ??࣠??
					if ( man.x+man.lx>obj[i].x && man.x<obj[i].x+obj[i].lx
						&&  man.y-man.ly<obj[i].y && man.y>obj[i].y-obj[i].ly ) {
						//?᫨ ???? 訯?-㡨???
						if( shboots && man.s==s_down && man.y<obj[i].y) {
							//?᫨ ?????? ᢥ??? ?? ????
							man.sy=-8;
							man.s=s_jump;
							//?? ??ꥬ ???
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
							//?᫨ ?㦨? ?? ? ?????ﭨ? ??䥪??,
							//?? ? 讪 ???!
							obj[i].myrg=9;

							man.sy=-8; man.s=s_ouch;
							man.myrg=wmyrg;
							addmess("??!",30);

							if ( --man.on<=0 ) {
								//??? ???稫?? - ?????
								man.s=s_crash;
			     					addmess("???????? ?????",60);
							}
							break;
						}
					}
				}
			}

			//?஢?ઠ ?? ?⮫???????? ? ???஭??? ?ࠣ?
			//if (man.myrg==0 && shields==0) //?᫨ ?㦨? ?? ??࣠??
			for ( i=0; i<maxbullet; ++i ) {
				if ( bullets[i].x>=man.x && bullets[i].x<=man.x+man.lx &&
					bullets[i].y<=man.y && bullets[i].y>=man.y-man.ly ) {
						switch ( bullets[i].typ ) {
						case 1:	//??誠
							break;
						case 2:
							bullets[i].typ=-1;
							addboom(bullets[i].x-8,bullets[i].y-8,16,16,0,0,b_boom,obloka);
							obj[i].myrg=9;

							man.sy=-4;
							man.s=s_ouch;
							man.myrg=wmyrg;
							addmess("??!",30);

							if ( --man.on<=0 ) {
								//??? ???稫?? - ?????
								man.s=s_crash;
								addmess("???????? ?????",60);
							}
							break;
						case 3:	//????⪠
							bullets[i].typ=-1;
							addboom(bullets[i].x-8,bullets[i].y-8,16,16,0,0,b_boom,obloka);
							++score;
							if ( coins<9999 ) {
								++coins;
							}
							break;
						case 4:	//??窠
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
		//???ᬮ?? ???⮢ ? ??.
		for ( i=0; i<maxmodel; ++i ) {
			if ( models[i].typ>=0 )	{
				if ( man.x+(man.dwx1+man.dwx2)/2>=models[i].x && man.x+(man.dwx1+man.dwx2)/2<=models[i].x+models[i].lx
				&&  man.y<=models[i].y+models[i].ly && man.y>=models[i].y ) {
					if ( models[i].typ==m_lift || models[i].typ==m_lift0 ) {
						//?????
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

	if ( man.myrg>0 ) { //?४??頥? ??࣠???
		--man.myrg;
	}
	if ( power>0 ) { //??????-?? ???砥???
		--power;
	}

	if (  (dw1==31 || dw2==31 || up1==31 || up2==31) && man.s!=s_crash) {
		man.sy=-8;	//訯? - ? 讪 ???!
		man.s=s_crash;
		addmess("?????? ??????",60);
	}

	//?஢?ઠ ?? ?ய????
	//???????
	if ( man.s==s_down ) {
		if( man.sy<8 && (leveldef.typ!=t_water || (waitboom&1)==0) ) {
			++man.sy;
		}
		if (dw1!=0 || dw2!=0) {
			//????? ??? ??????
			man.s=s_go;

			man.sy=0;
			if ( min(poc[dw1][sx1],poc[dw2][sx2])<0 ) {
				//??ࠢ?????? ?? ᪠???
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
				//᪮?짪?
				man.dsx1*=2;
				man.dsx2*=2;
			}
			man.buf=man.jmspr+3*man.lx*man.ly;
			man.fw=2;
			addboom(man.x+8,man.y-4,16,16,0,0,b_boom,obloka+16*16*5);

			if ( down && kolun) { //?᫨ ???? ?????
				if ( dw1>16 && dw1<=19 ) { //?????? ???㭮?
					check(man.x+man.dwx1,man.y-man.dwy);
					if ( (--*poslal)<=15 ) {
						addbullet(x*16+8,y*16+8,-random(2),0,8,fon+(*poslal+1)*256);
						*poslab=*poslal=0;
					} else {
						addboom(x*16+2-random(4),y*16,16,16,0,0,b_boom,obloka+16*16*5);
					}
				} else
				if ( dw2>16 && dw2<=19 ) { //?????? ???㭮?
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
	//??릮?
	if ( man.s==s_jump || man.s==s_ouch ) {
		if ( dw1 || dw2 ) {
		   //??릮? ? ???஭? ???????
		   man.dsx1=nappoc[dw1];
		   man.dsx2=nappoc[dw2];
		} //??? up1
		if ( up1>=16 && up1<=19 ) { //㤠? ? ??௨???? ??? ??祧??騩 ??⮫??
				man.s=s_down;
				man.sy=1;
				check(man.x+man.upx1,man.y-man.upy);

				if ( up1!=16 && (--*poslal)<=15 ) { //?᫨ [?] ??-?ਧ
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
		} else //??? up2
		if ( up2>=16 && up2<=19 ) { //㤠? ? ??௨???? ??? ??祧??騩 ??⮫??
				man.s=s_down;
				man.sy=1;
				check(man.x+man.upx2,man.y-man.upy);

				if ( up2!=16 && (--*poslal)<=15 ) { //?᫨ [?] ??-?ਧ
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
		if ( man.y<=man.ly ) { //??室 ?? ??࠭
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
		//?????
		if( jump==0 && man.sy<5 ) {
			++man.sy;
		}
		if ( dw1 || dw2 ) {
			//????⨫??
			man.s=down;
			man.y=oldy;
		}
		if ( up1>1 || up2>1 ) { //㤠? ? ??⮫??
			man.s=s_down;
			man.sy=1;
		}
	} else
	if ( man.s==s_crash ) {
		//??? ᮢᥬ ?????
		if(man.sy<32) ++man.sy;
		man.sx=man.dsx1=man.dsx2=0;
		if (man.y>siz_ylev*16) {
			//?믠????? ???? ????ਭ??
			man.s=s_over;
			end=1;
		}
	} else //??ଠ
	if ( man.s==s_go ) {
		man.dsy=0;
		if ( dw1==0 && dw2==0 ) {
				man.s=s_down;
				man.sy=1; //2
		} else
		if ( dw1==19 ) { //??祧????? ?⥭??
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
		if ( dw2==19 ) { //??祧????? ?⥭??
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
			//?᫨ ?????ઠ ... ??????!
			man.y-=16;
			dw1=check(man.x+man.dwx1,man.y-man.dwy);
			dwi1=tekblock; sx1=sx;
			dw2=check(man.x+man.dwx2,man.y-man.dwy);
			dwi2=tekblock; sx2=sx;
		}
		//?᫨ ???-?? ??? ??????
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
		//?஢?ઠ ?? ᡮ? ??饩
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
		//?????? ???ࣨ?
		Bar(24,20, max((int)power>>2,1) ,8);
		PutMas16(24+max((int)power>>2,1),16,items+256*8*6+((waitboom&7)*256));
	}
	if ( (power>32 || power&1) && shields ) {
			//?뢮? ⥭? ?? ???
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
			if ( man.napr ) { //?᫨ ?㦨? ????????, ?? ?뢮??? ????? ?? ??।
				PutCMasr(man.x-r_x+16,man.y-r_y+16-man.ly,man.lx,man.ly,man.buf);
			} else {
				PutCMas(man.x-r_x+16,man.y-r_y+16-man.ly,man.lx,man.ly,man.buf);
			}
		} else {
			CurrentColor=15; //?? ࠭?? ? ??????
			if ( man.napr ) { //?᫨ ?㦨? ????????, ?? ?뢮??? ????? ?? ??।
				PutCBlinkr(man.x-r_x+16,man.y-r_y+16-man.ly,man.lx,man.ly,man.buf);
			} else {
				PutCBlink(man.x-r_x+16,man.y-r_y+16-man.ly,man.lx,man.ly,man.buf);
			}
		}
	}
}

void getitem(int x_, int y_) { //???⨥ ???? ?? ???न??⠬

	check(x_,y_); //??ᬮ?ਬ ?? ??, ??? ? ??? ?? ?⨬ ?????.
	switch ( tekblock&224 ) {
		case 1<<5: //??????
			*poslab=*poslab&31;
			addboom(x*16+4,y*16+4,8,8,0,-1,b_buh,obloka+256*10+32*32*10+128*5);
			++score;
			if ( coins<9999 ) {
				++coins;
			}
			break;

		case 2<<5: //??? - ???ࣨ?
			*poslab=*poslab&31;
			addboom(x*16,y*16,16,8,0,-1,b_buh,obloka+256*10+32*32*10+128*1);
			score+=200;
			if ( man.on<5 ) {
				++man.on;
			} else {
				coins+=5;
			}
			break;

		case 3<<5: //?????? - ?????
			*poslab=*poslab&31;
			addboom(x*16,y*16,16,8,0,-1,b_buh,obloka+256*10+32*32*10+128*3);
			score+=1000;
			if ( hearts<9 ) {
				++hearts;
				put_additems();
			}
			break;

		case 4<<5: //?????
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

		case 5<<5: //??誨
			*poslab=*poslab&31;
			addboom(x*16+4,y*16+4,8,8,0,-1,b_buh,obloka+256*10+32*32*10+128*5+64);
			if ( beams<99 ) {
				++beams;
			}
			break;

		case 6<<5: //?६?
			*poslab=*poslab&31;
			addboom(x*16,y*16,16,8,0,-1,b_buh,obloka+256*10+32*32*10+128*2);
			score+=500;
			times=9999;
			break;

		case 7<<5: //???ࣨ?
        		*poslab=*poslab&31;
			addboom(x*16,y*16,16,16,0,0,b_boom,obloka+256*5*4);
			addboom(x*16,y*16,16,8,0,-1,b_buh,obloka+256*10+32*32*10+128*2);
			score+=500;
			power=500;
			//break;
		}
}

void put_score(void) {
	//?뢮? ?窮?
	static int i;
	if ( draw==0 ) {
		return;	//?? ?ᥣ?? ?뢮???? ?窨!
	}

	screen old=CurrentScreen;
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
     //?뢮? ???????⥫??? ??饩
	screen old=CurrentScreen;
	NormalScreen();
	Clip(0,0,319,199);

	char_bkgd=218;
	if ( shields ) { //???
		PutMas16(16*18,20+20*0,additems+256*12);
	}
	if ( glass ) { //࠭楢. ????
		PutMas16(16*18,20+20*1,additems+256*8);
	}
	if ( maxjump<-8 ) { //??㦨???
		PutMas16(16*18,20+20*2,additems+256*2);
	}
	if ( shboots ) { //訯????
		PutMas16(16*18,20+20*3,additems+256*4);
	}
	if ( kolun ) { //?????
		PutMas16(16*18,20+20*4,additems+256*10);
	}
	if ( jetpack ) { //࠭楢. ????
		PutMas16(16*18,20+20*5,additems+256*6);
	}

	if ( continues>0 ) { //?த???????
		PutMas16(16*18,20+20*6,additems);
		if ( continues>1 ) {
			MoveXY(16*18,20+20*6);
			vprint("%1d",continues);
		}
	}
	//?????
	PutMas16(16*18,20+20*7,items+256*8*2);
	MoveXY(16*18,20+20*7); vprint("%1d",hearts);

	char_bkgd=0;

	Clip(16,16,16*17-1,16*11-1);
	SetScreen(old);
}

void makemonstr(int i, int n, unsigned int x,unsigned int y) {
	object *ob;
	//????? ???????, ⨯ ???????, ??? ????? ???????
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
	//????????? ?ࠣ??
	static int oldx,oldy,i,j;
	static int dw,lf,rt,up_;
	static object *ob;

	for ( i=0; i<maxobj; ++i ) {
		if ( obj[i].on!=0 ) {
			//?᫨ ??⨢??, ?? ???砥?
			ob=&obj[i];
			oldx=ob->x; oldy=ob->y;

			//??-?? ?? ??室 ?? ?࠭??? ????ਭ??
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

			//?ਡ???? ?᪮७?? ?? ????
			ob->x+=(ob->sx+ob->dsx);
			ob->y+=(ob->sy+ob->dsy);

			//??? ??ࠢ?, ᫥?? ? ???????
			lf=check(ob->x+ob->lfx,ob->y-ob->lfy);
			rt=check(ob->x+ob->rtx,ob->y-ob->rty);
			up_=check(ob->x+ob->upx,ob->y-ob->upy);
			//㧭??? ??? ??? ????
			dw=check(ob->x+ob->dwx,ob->y-ob->dwy);
			if ( dw==23 ) {
				dw=0;
			}
			if ( ob->typ==3 || ob->typ==6 || ob->typ==19 ) //?????
			{
				ob->s=s_fly;
			}

			if (++ob->f0>6-abs(ob->sx+ob->dsx)) {
				ob->f0=0;
				ob->f++;
			}

			if ( (ob->s==s_jump || ob->s==s_down) && monsters[ob->n].maxjm && abs(ob->sy)>2) {
				//??룠??
				ob->buf=monsters[ob->n].jmspr;
			} else
			if ( ob->sx+ob->dsx!=0 && ob->x!=oldx) {
				//????
				if ( ob->f>=monsters[ob->n].maxgo ) ob->f=0;
				ob->buf=monsters[ob->n].gospr+ob->f*ob->lx*ob->ly;
			} else {
				//?⮨?
				ob->buf=monsters[ob->n].stspr;
				ob->f=0;
			}
			if (ob->sx>0) { //??????稢??? ??ࠩ? ? ????ᨬ???? ?? ???ࠢ?????
				ob->napr=1;
			} else if ( ob->sx<0 ){
				ob->napr=0;
			}

			//?⮫???????? ?? ?⥭???
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

			if ( ob->myrg<=0 ) //?᫨ ?? ??࣠??
			for ( j=0; j<maxbullet; ++j ) {	//?⮫???????? ? ???ﬨ ? ???????
				if ( (bullets[j].typ==1 || bullets[j].typ>=8) && bullets[j].x>=ob->x && bullets[j].x<=ob->x+ob->lx &&
					bullets[j].y<=ob->y && bullets[j].y>=ob->y-ob->ly
				) {
					bullets[j].typ=-1;
					addboom(bullets[j].x-8,bullets[j].y-8,16,16,0,0,b_boom,obloka);
					ob->myrg=16;
					if ( --ob->on<=0 ) {
						if ( ob->typ>=16 ) {
							//?᫨ 㡨? ????!!!!!!!
							boss=0;
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,-2,-2,3,items);	//????⪠
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,2,-2,3,items);	//????⪠
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,-1,-3,3,items);	//????⪠
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,1,-3,3,items);	//????⪠
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,0,-4,3,items);	//????⪠
							score+=5000;
							coins+=30;
							open_exit();
							addboom(ob->x,ob->y-ob->ly,ob->lx,ob->ly,bullets[j].sx,-random(5),b_fall,ob->buf);
							addboom(ob->x,ob->y-ob->ly,16,8,0,-2,b_buh,obloka+256*10+32*32*10+128*4);
							if ( dev!=255 ) {
								//????砥? ??ଠ????? ??????
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
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,0,-3,3,items);	//????⪠
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
				//?????
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
			//?஢?ઠ ?? ?ய????
			//???????
			if ( ob->s==s_down ) {
				if(ob->sy<7) ++ob->sy;
				//if (dw!=0 && (dw1!=0 || dw2!=0)) {
				if (dw!=0) {
					if ( dw==31 ) {
						//?? 訯?? ???६!
						if ( ob->myrg==0 ) {
							--ob->on;
							ob->myrg=16;
						}
					}
					//????? ??? ??????
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
			//??릮?
			if ( ob->s==s_jump) {
				if ( up_==16 ) { //㤠? ? ??⮫??
						ob->s=s_down;
						ob->sy=0;
				} else
				if ( ob->y<=ob->ly || ob->y<=0) { //??室 ?? ??࠭
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
				//??ଠ
				if ( ob->s==s_go ) {
					if ( dw==0 ) {
						ob->s=s_down;
						ob->sy=2;
						//ob->dsx=ob->dsx/2;
						ob->dsy=0;
					} else if ( dw==31 ) {
						//?? 訯?? ???६!
						if ( ob->myrg==0 ) {
							ob->myrg=16;
							if (--ob->on<=0) {
								//?????? ???஦?????? ??᫥ 訯??
								addboom(ob->x,ob->y-ob->ly,32,32,0,0,b_boom,obloka+(10*256+32*32*5));
								addboom(kobj[i].x,kobj[i].y-ob->ly,32,32,0,0,b_boom,obloka+(10*256+32*32*5));
								makemonstr(i,kobj[i].n,kobj[i].x,kobj[i].y);
								break;
							}
						}
					} else {
						ob->sy=0;
						//?ਡ???? ?? ५???? ᪮????? ????????

						//??ࠢ??????? ?? ५????
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
					case 0: //?த ?? ?⥭? ?? ?⥭?
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

					case 1: //㬭?? ???᫥????⥫?
						if ( dw==0 ) {	//????? ?ய????!
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

					case 2: //??????
						if ( dw ) {
							ob->sy=-random(10);
							ob->s=s_jump;
							ob->napr=(man.x<ob->x)?0:1;
						}
						break;

					case 3: //????? ?? ?⥭? ?? ?⥭?
						if ( ob->sx==0 ) {
							if (lf) ob->sx=monsters[ob->n].speed;
							else if (rt) ob->sx=-monsters[ob->n].speed;
							else ob->sx=monsters[ob->n].speed-random(monsters[ob->n].speed*2);
						}
						break;

					case 4: //????? ?? ??⮫?? ?? ???? ?????? ? ?.?.
						if ( dw ) {
							ob->on=0;
							addboom(ob->x+ob->lx/2-8,ob->y-ob->ly/2-8,16,16,0,0,b_boom,obloka);
						}
						break;

					case 5: //?????? ?? ?⥭? ?? ?⥭?
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

					case 6: //?????騩 㬭?? ???᫥????⥫?
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

					case 16: //???? N2
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

					case 17: //???? N3
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
								//?ࠦ?? ᥬ?!
                        					if ((waitboom&7)==0)
									addbullet(man.x+8,ob->y-16*14,man.sx,2+random(2), 2,enbull+256*8);
								//addbullet(man.x+8,ob->y-16*14,man.sx,4, 2,items+256*1*8);
							}
							ob->sx=0;
						}
                        			if ((waitboom&15)==0)
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly/2,(man.x<ob->x)?-5:((man.x>ob->x)?5:0),ob->y>man.y+16?-5:0, 2,enbull);
						break;

					case 18: //???? N1
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

					case 19: //???? N4 ?????騩
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

					case 21: //???? ????????!
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
						//?ࠦ?? ᥬ?!
                        			if ((waitboom&15)==0) {
							addbullet(ob->x+ob->lx/2,ob->y-ob->ly,man.x>ob->x?5:-5,-2+random(4),2,fon+(176*256));
						}
						break;

					case 20: //???? ??????????!
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
						//?ࠦ?? ᥬ?!
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
					//?᫨ ??? ???? ?? ?뢮??? ??? ᨫ?
					CurrentColor=(32+(ob->myrg&1));
					Bar(24,30, max((int)ob->on*8,1) ,8);
					PutMas16(24+max((int)ob->on*8,1),26,items+256*8*2+((waitboom&7)*256));
				}
			}
		}
	}
}