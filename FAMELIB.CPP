#include <alloc.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <sys\stat.h>
#include <conio.h>

#include "famegraf.h"
extern void fatalerror(char *);

#define MaxBlocks 128

struct zapblk {
	unsigned long posblk;
	unsigned long lenblk;
	unsigned long lenpak;
	char comp;
	char typblk;
	char name[13];
} flib[MaxBlocks];
//одна запись = 27 байт

char name_lib[13];

static int fdz_lib=-1;

int numblk=0;	//кол-во файлов в библ-ке
unsigned long posmap;	//позиция MAP в файле

unsigned long posblk;
unsigned long lenblk;
unsigned long lenpak;

block pakdest; //буфер для паковки/распаковки
char lockunpak=0;	//запрет на разархивацию при взятии из бибки

int SetLib(char *name) { //начальная инициализация
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
		//такой нет бибки
		if ( (fdz_lib=open (name_lib, O_BINARY | O_CREAT | O_RDWR, S_IREAD | S_IWRITE))<0 ) {
			//не создали новую
			fdz_lib=-1;
			return 0;
		} else {
			//новая
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

	//считаем ссылку на MAP кусок
	if ( 	lseek(fdz_lib, 0L, SEEK_SET)==-1
		|| read (fdz_lib, &posmap, 4) == -1
		|| lseek(fdz_lib, posmap , SEEK_SET)==-1
		//считаем кол-во блоков в файле
		|| read (fdz_lib, &numblk, 2) == -1 ) {
		//не могу считать заголовок
		close(fdz_lib);
		fdz_lib=-1;
		return 0;
	}

	if (numblk>MaxBlocks) numblk=MaxBlocks;
	for (i=0; i<numblk; ++i) {
		//читаем одну запись MAP-файла
		if (read (fdz_lib, &blk[0], 27) == -1 ) {
			//не читается запись
			close(fdz_lib); return (0);
		}
		memcpy(&flib[i], &blk[0], 27);
	}
	return 1;
}

int GetLib(char *name, block dest) {
	static int i;
	if ( fdz_lib<0 ) {	//не выбр. биб-ка
		return 0;
	}
	for (i=0; i<MaxBlocks; ++i) {
		if ( strcmpi(name, flib[i].name)==0 ) {
			if (	lseek(fdz_lib, flib[i].posblk , SEEK_SET)!=flib[i].posblk
				//файл мал или бибка не открыта
				|| read(fdz_lib, dest, flib[i].lenpak)==-1 ) {
				//не читается бибка
				return (0);
			}
			_fmeminv(dest,flib[i].lenpak);
			//распаковываем, если разрешено lockunpak!
			if ( lockunpak==0 ) {
				//распаковываем, если запаковано
				if ( flib[i].comp==1 ) {
					if ( (lenblk=UnpackBlock(dest, flib[i].lenpak, flib[i].lenblk))==flib[i].lenblk ) {
						_fmemcpy(dest,pakdest,lenblk);
						farfree(pakdest); //если упаковывается блок...
					} else {
						farfree(pakdest); //если упаковывается блок...
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

	if ( fdz_lib<0 || lenblk>(unsigned long)65000) {
		return 0;
	}

	k=0;	//не добавл в MAP
	j=-1; ++numblk;
	for (i=0; i<MaxBlocks; ++i) {
		if ( strcmpi(name, flib[i].name)==0 ) {
			--numblk;
			j=i; break;
		}
	}
	//Если такого блока нет, то ищем пустой
	if (j==-1) {
		for (i=0; i<MaxBlocks; ++i) {
			if ( strlen(flib[i].name)==0 ) {
				k=1; j=i; break;
			}
		}
	}

	if (j==-1 || numblk>MaxBlocks) { //Если и пустого блока нет, то выход
		--numblk;
		//Нет места в MAP
		return (0);
	} else {
		if ( (lenpak=PackBlock(dest, lenblk))<lenblk ) {
			_fmemcpy(dest,pakdest,lenpak);
			farfree(pakdest); //если упаковывается блок...
			flib[j].comp=1;
		} else {
			farfree(pakdest);
			lenpak=lenblk; //не упаковался
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
			//не пишется в бибку
			--numblk;
			_fmeminv(dest,lenpak);
			return 0;
		}
		_fmeminv(dest,lenpak);
		if( k ) {
			//добавлен новая запись в MAP
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
		//не пишется в бибку
		return 0;
	}
	for (i=0; i<MaxBlocks; ++i) {
		if ( strlen(flib[i].name)!=0 ) {
			if (write (fdz_lib, &flib[i], sizeof(flib[0])) == -1 ) {
				//не пишется в бибку
				return 0;
			}
		}
	}
	return 1;
}

int PackLib(void) {
	int i,j;
	block tmp;
	int fdz;
	if( fdz_lib<0 ) {
		return 0;
	}
	//Упаковка библиотеки
	remove("tmp.tmp");
	if ( (fdz=open ("tmp.tmp",O_WRONLY | O_CREAT | O_BINARY, S_IREAD | S_IWRITE)) < 0) {
		//не открывается TMPшный файл
		close(fdz); return 0;
	}
	if (write (fdz, &posmap, 4) == -1 ) {
		close(fdz); return 0;
	}
	for (i=0; i<MaxBlocks; ++i) {
		if ( strlen(flib[i].name)!=0 ) {
			tmp=(block)famemalloc( flib[i].lenpak );
			lockunpak=1; //запрет на разархивацию
			if (GetLib(flib[i].name,tmp)==0) {
				farfree(tmp);
				lockunpak=0; //разрешение на разархивацию
				close(fdz); return (0);
			}
			_fmeminv(tmp,flib[i].lenpak);
			lockunpak=0; //разрешение на разархивацию
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
		l=t; //предыдущий символ
		t=src[spos]; //возмем новый симв
		if (l==t) { //новый символ равен предыдущему
			cl++;
			if (cl>63) { //кол-во одинаковых симв превысило 64
				//закодир заголовок 11хххххх & символ
				pakdest[dpos++]= (0xff); //63
				pakdest[dpos++]= l;
				cl=1;
			}
		} else { //новый символ не равен предыдущему
			if (cl>1) { //запись цепочки
				pakdest[dpos++]= (0xc0 | cl);
				pakdest[dpos++]= l;
			} else { //символ без 2х перв бит
				if (l>127) { //запись цепочки
					pakdest[dpos++]= 0xc1; //1
					pakdest[dpos++]= l;
				} else {
					pakdest[dpos++]= l;
				}
			}
			cl=1;
		}
		if (dpos>=lenblk) {
			//ошибка-переполнение при архивации
			return (dpos);
		}
	} //конец осн. цикла
	if (cl >1) {
		pakdest[dpos++]= (0xc0 | cl);
		pakdest[dpos++]= l;
	} else {
		if (t>127) { //запись цепочки
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
		t=src[spos]; //возмем новый симв
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
			break; //ошибка при разархивации
		}
	}
	return (dpos);
}

void far *famemalloc(unsigned long n) {
	void far *t;
	if( (t=farmalloc(n))==NULL ) {
		fatalerror("Мало памяти");
	}
	return t;
}

void _fmeminv(block s, unsigned long n) {
	register unsigned long i;
	//инвертирует строку
	for (i=0; i<n; ++i) {
		*s=(~*s);
		s++;
	}
}
