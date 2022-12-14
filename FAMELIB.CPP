#include <malloc.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <sys\stat.h>
#include <conio.h>

// PATCH: platform code
#ifndef __MSDOS__
#include "platform.h"
#endif

#include "famegraf.h"
extern void fatalerror(char *);

#define MaxBlocks 128

#pragma pack(push, 1)
typedef struct {
	unsigned long posblk;
	unsigned long lenblk;
	unsigned long lenpak;
	char comp;
	char typblk;
	char name[13];
} zapblk;
#pragma pack(pop)
//???? ?????? = 27 ????

// PATCH: separate type declaration and variable
zapblk flib[MaxBlocks];

char name_lib[13];

static int fdz_lib=-1;

int numblk=0;	//???-?? 䠩??? ? ????-??
unsigned long posmap;	//??????? MAP ? 䠩??

unsigned long posblk;
unsigned long lenblk;
unsigned long lenpak;

block pakdest; //????? ??? ???????/??ᯠ?????
char lockunpak=0;	//?????? ?? ࠧ??娢???? ??? ???⨨ ?? ?????

int SetLib(char *name) { //??砫쭠? ???樠???????
	static int i;
	zapblk blk[1];

	if( strlen(name)==0 && fdz_lib!=-1) {
		close(fdz_lib);
		fdz_lib=-1;
		return 1;
	}
	if( fdz_lib!=-1) {
		close(fdz_lib);
	}

	strcpy(name_lib,name);
	if( strstr(name_lib,".")==NULL ) {
		strcat(name_lib,".fml");
	}
	_fmemset(flib,0L,sizeof(flib));

	if ( (fdz_lib= open (name_lib, O_BINARY | O_EXCL | O_RDWR, S_IREAD | S_IWRITE))<0 ) {
		//⠪?? ??? ?????
		if ( (fdz_lib=open (name_lib, O_BINARY | O_CREAT | O_RDWR, S_IREAD | S_IWRITE))<0 ) {
			//?? ᮧ???? ?????
			fdz_lib=-1;
			return 0;
		} else {
			//?????
			numblk=0;
			posmap=4L;
			if( write (fdz_lib, &posmap, 4)==-1
				|| lseek(fdz_lib, posmap , SEEK_SET)==-1
				|| write (fdz_lib, &numblk, 2)==-1 ) {

					close(fdz_lib);
					fdz_lib=-1;
					return 0;
				}
		}
	}

	//???⠥? ??뫪? ?? MAP ??᮪
	if ( 	lseek(fdz_lib, 0L, SEEK_SET)==-1
		|| read (fdz_lib, &posmap, 4) == -1
		|| lseek(fdz_lib, posmap , SEEK_SET)==-1
		//???⠥? ???-?? ?????? ? 䠩??
		|| read (fdz_lib, &numblk, 2) == -1 ) {
		//?? ???? ??????? ?????????
		close(fdz_lib);
		fdz_lib=-1;
		return 0;
	}

	if (numblk>MaxBlocks) numblk=MaxBlocks;
	for (i=0; i<numblk; ++i) {
		//??⠥? ???? ?????? MAP-䠩??
		if (read (fdz_lib, &blk[0], 27) == -1 ) {
			//?? ??⠥??? ??????
			close(fdz_lib); return (0);
		}
		memcpy(&flib[i], &blk[0], 27);
	}
	return 1;
}

int GetLib(char *name, block dest) {
	static int i;
	if ( fdz_lib<0 ) {	//?? ????. ???-??
		return 0;
	}
	for (i=0; i<MaxBlocks; ++i) {
		if ( strcmpi(name, flib[i].name)==0 ) {
			if (	lseek(fdz_lib, flib[i].posblk , SEEK_SET)!=flib[i].posblk
				//䠩? ??? ??? ????? ?? ???????
				|| read(fdz_lib, dest, flib[i].lenpak)==-1 ) {
				//?? ??⠥??? ?????
				return (0);
			}
			_fmeminv(dest,flib[i].lenpak);
			//??ᯠ???뢠??, ?᫨ ࠧ??襭? lockunpak!
			if ( lockunpak==0 ) {
				//??ᯠ???뢠??, ?᫨ ??????????
				if ( flib[i].comp==1 ) {
					if ( (lenblk=UnpackBlock(dest, flib[i].lenpak, flib[i].lenblk))==flib[i].lenblk ) {
						_fmemcpy(dest,pakdest,lenblk);
						farfree(pakdest); //?᫨ 㯠???뢠???? ????...
					} else {
						farfree(pakdest); //?᫨ 㯠???뢠???? ????...
						break;
					}
				}
			}
			return 1;
		}
	}
	return 0;
}

unsigned long SizeLib(char *name) {
	static int i;
	for (i=0; i<MaxBlocks; ++i) {
		if ( strcmpi(name, flib[i].name)==0 ) {
				return (flib[i].lenblk);
		}
	}
	return 0L;
}

int PutLib(char *name, block dest, unsigned long lenblk) {
	static int i,j,k;

	if ( fdz_lib<0 || lenblk>(unsigned long)65000UL) {
		return 0;
	}

	k=0;	//?? ?????? ? MAP
	j=-1; ++numblk;
	for (i=0; i<MaxBlocks; ++i) {
		if ( strcmpi(name, flib[i].name)==0 ) {
			--numblk;
			j=i; break;
		}
	}
	//?᫨ ⠪??? ????? ???, ?? ?饬 ???⮩
	if (j==-1) {
		for (i=0; i<MaxBlocks; ++i) {
			if ( strlen(flib[i].name)==0 ) {
				k=1; j=i; break;
			}
		}
	}

	if (j==-1 || numblk>MaxBlocks) { //?᫨ ? ???⮣? ????? ???, ?? ??室
		--numblk;
		//??? ????? ? MAP
		return (0);
	} else {
		if ( (lenpak=PackBlock(dest, lenblk))<lenblk ) {
			_fmemcpy(dest,pakdest,lenpak);
			farfree(pakdest); //?᫨ 㯠???뢠???? ????...
			flib[j].comp=1;
		} else {
			farfree(pakdest);
			lenpak=lenblk; //?? 㯠???????
			flib[j].comp=0;
		}

		if (lenpak<=flib[j].lenpak && k==0) {
			flib[j].posblk = lseek (fdz_lib, flib[j].posblk , SEEK_SET);
		} else {
			flib[j].posblk = lseek (fdz_lib, 0L , SEEK_END);
			k=1;
		}

		_fmeminv(dest,lenpak);
		if ( (write (fdz_lib, dest, lenpak)) == -1 ) {
			//?? ??????? ? ?????
			--numblk;
			_fmeminv(dest,lenpak);
			return 0;
		}
		_fmeminv(dest,lenpak);
		if( k ) {
			//???????? ????? ?????? ? MAP
			posmap = lseek (fdz_lib, 0L , SEEK_END);
		}

		strcpy(flib[j].name, name);
		flib[j].lenblk=lenblk;
		flib[j].lenpak=lenpak;

		return ( SaveMap() );
	}
	//return (1);
}

int DelLib(char *name) {
	static int i;

	if ( fdz_lib<0 ) {
		return 0;
	}

	for (i=0; i<MaxBlocks; ++i) {
		if ( strcmpi(name, flib[i].name)==0 ) {
			--numblk;
			flib[i].name[0]=0;
		}
	}
	return ( SaveMap() );
}

int SaveMap(void) {
	static int i;

	if ( fdz_lib<0 ) {
		return 0;
	}

	if ( lseek(fdz_lib, 0L, SEEK_SET)==-1
		|| write (fdz_lib, &posmap, 4)==-1
		|| lseek(fdz_lib, posmap , SEEK_SET)==-1
		|| write (fdz_lib, &numblk, 2)==-1 ) {
		//?? ??????? ? ?????
		return 0;
	}
	for (i=0; i<MaxBlocks; ++i) {
		if ( strlen(flib[i].name)!=0 ) {
			if (write (fdz_lib, &flib[i], sizeof(flib[0])) == -1 ) {
				//?? ??????? ? ?????
				return 0;
			}
		}
	}
	return 1;
}

int PackLib(void) {
	int i;//,j;
	block tmp;
	int fdz;
	if( fdz_lib<0 ) {
		return 0;
	}
	//???????? ??????⥪?
	remove("tmp.tmp");
	if ( (fdz=open ("tmp.tmp",O_WRONLY | O_CREAT | O_BINARY, S_IREAD | S_IWRITE)) < 0) {
		//?? ????뢠???? TMP???? 䠩?
		close(fdz); return 0;
	}
	if (write (fdz, &posmap, 4) == -1 ) {
		close(fdz); return 0;
	}
	for (i=0; i<MaxBlocks; ++i) {
		if ( strlen(flib[i].name)!=0 ) {
			tmp=(block)famemalloc( flib[i].lenpak );
			lockunpak=1; //?????? ?? ࠧ??娢????
			if (GetLib(flib[i].name,tmp)==0) {
				farfree(tmp);
				lockunpak=0; //ࠧ??襭?? ?? ࠧ??娢????
				close(fdz); return (0);
			}
			_fmeminv(tmp,flib[i].lenpak);
			lockunpak=0; //ࠧ??襭?? ?? ࠧ??娢????
			flib[i].posblk = lseek (fdz, 0L , SEEK_END);
			if (write (fdz, tmp, flib[i].lenpak) == -1 ) {
				farfree(tmp);
				close(fdz); return (0);
			}
			farfree(tmp);
		}
	}
	posmap = lseek (fdz, 0L , SEEK_END);
	close(fdz_lib);
	remove(name_lib);
	fdz_lib=fdz;
	if( SaveMap() ) {
		close(fdz_lib);
		rename("tmp.tmp",name_lib);
		if ( (fdz_lib=open (name_lib, O_BINARY, S_IREAD | S_IWRITE))<0 ) {
			fdz_lib=-1;
			return 0;
		}
		return 1;
	}
	return 0;
}

unsigned long PackBlock(block src, unsigned long lenblk) {
	static unsigned char t,l,cl;

	static unsigned long dpos,spos;

	cl=dpos=spos=0;
	pakdest=(block)famemalloc((unsigned long)(lenblk+3));

	t=src[spos];
	for ( spos=0; spos<lenblk; spos++ ) {
		l=t; //?।???騩 ᨬ???
		t=src[spos]; //?????? ????? ᨬ?
		if (l==t) { //????? ᨬ??? ࠢ?? ?।???饬?
			cl++;
			if (cl>63) { //???-?? ?????????? ᨬ? ?ॢ?ᨫ? 64
				//??????? ????????? 11?????? & ᨬ???
				pakdest[dpos++]= (0xff); //63
				pakdest[dpos++]= l;
				cl=1;
			}
		} else { //????? ᨬ??? ?? ࠢ?? ?।???饬?
			if (cl>1) { //?????? 楯?窨
				pakdest[dpos++]= (0xc0 | cl);
				pakdest[dpos++]= l;
			} else { //ᨬ??? ??? 2? ???? ???
				if (l>127) { //?????? 楯?窨
					pakdest[dpos++]= 0xc1; //1
					pakdest[dpos++]= l;
				} else {
					pakdest[dpos++]= l;
				}
			}
			cl=1;
		}
		if (dpos>=lenblk) {
			//?訡??-??९??????? ??? ??娢?樨
			return (dpos);
		}
	} //????? ???. 横??
	if (cl >1) {
		pakdest[dpos++]= (0xc0 | cl);
		pakdest[dpos++]= l;
	} else {
		if (t>127) { //?????? 楯?窨
			pakdest[dpos++]= 0xc1; //63
			pakdest[dpos++]= t;
		} else {
			pakdest[dpos++]= t;
		}
	}
	return (dpos);
}

unsigned long UnpackBlock(block src, unsigned long lenpak, unsigned long lenblk) {
	static unsigned char t,cl;
	static unsigned long dpos,spos;

	dpos=spos=0;
	pakdest=(block)famemalloc((unsigned long)(lenblk+64));

	for ( spos=0; spos<lenpak; ++spos ) {
		t=src[spos]; //?????? ????? ᨬ?
		if ( t>127 ) {
			cl=t&0x3f; //00111111
			t=src[++spos];
			while (cl--) {
				pakdest[dpos++]= t;
			}
		} else {
			pakdest[dpos++]= t;
		}
		if (dpos>lenblk) {
			break; //?訡?? ??? ࠧ??娢?樨
		}
	}
	return (dpos);
}

void far *famemalloc(unsigned long n) {
	void far *t;
	if( (t=farmalloc(n))==NULL ) {
		fatalerror("???? ??????");
	}
	return t;
}

void _fmeminv(block s, unsigned long n) {
	register unsigned long i;
	//??????????? ??ப?
	for (i=0; i<n; ++i) {
		*s=(~*s);
		s++;
	}
}
