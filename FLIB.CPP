//07-02-95 03:42pm Автор: Михаил Братусь

#include <stdlib.h>
#include <alloc.h>
#include <io.h>
#include <dos.h>
#include <fcntl.h>
#include <string.h>
#include <stat.h>
#include <dir.h>
#include "famegraf.h"

struct zapblk {
	unsigned long posblk;
	unsigned long lenblk;
	unsigned long lenpak;
	char comp;
	char typblk;
	char name[13];
};

extern zapblk flib[];
extern int numblk;
extern char name_lib[13];
extern char name_map[13];

char tektyp[1];
char tekname[32];

char modify=0;

block tmp;
int fdz;

unsigned long filelen;

struct ffblk ffblk;	//для каталога и wildcard

void main(int argc, char *argv[]) {
	unsigned long lenblk_,lenpak_;
	int i,j;

	puts(">>> Работа с библиотеками <<<  FaMe Soft by BMV  (C)1994\n");

	if (argc < 2) {
		puts("■ ОШИБКА: программу FLIB.EXE нужно вызывать так:\n\n"
			"  FLIB <имя библиотеки> [@|%|{+|-|*}имя ...]\n"
			"   + добавить файлы в библиотеку    (шаблон)\n"
			"   ^ переместить файлы в библиотеку (шаблон)\n"
			"   - удалить файл из библиотеки\n"
			"   * извлечь файл из библиотеки\n"
			"   @ извлечь все файлы из библиотеки\n"
			"   % уплотнить библиотеку\n\n"
			"Например: FLIB my +font.m1 *my.bin");
		exit (0);
	}

	if (SetLib(argv[1])==0) {
		printf("■ ОШИБКА: не могу открыть библиотеку '%s'",argv[1]);
		exit (1);
	}
	if (argc > 2 ) {
		//проведем операцию для обработки всех параметров
		for (i=2; i<argc; ++i) {
			strcpy(tektyp,argv[i]);	tekname[12]=0;
			switch (tektyp[0]) {
			case '-':
				printf("Удаляю из библиотеки: %s\n",tekname);
				DelLib(tekname);
				++modify;
				break;
			case '+':
			case '^':
				j = findfirst(tekname,&ffblk,0);
				while (!j) {
					printf("Добавляю: %s ",ffblk.ff_name);
					if ( (fdz=open(ffblk.ff_name, O_RDONLY | O_BINARY, S_IREAD | S_IWRITE)) == -1 ) {
						printf("■ ОШИБКА: не могу открыть файл '%s'",ffblk.ff_name);
						farfree(tmp);
						break;
						//exit (1);
					}
					//узнаем длину файла
					printf("%u байт\n",(unsigned int)(filelen=ffblk.ff_fsize));
					tmp=(block)famemalloc(filelen);
					if ( read(fdz, tmp, filelen) == -1) {
						printf("■ ОШИБКА: не могу считать файл '%s'",ffblk.ff_name);
						farfree(tmp);
						close(fdz);
						break;
					}
					close(fdz);
					if ( PutLib(ffblk.ff_name,tmp,filelen) == 0 ) {
						printf("■ ОШИБКА: не могу записать в библиотеку файл '%s'",ffblk.ff_name);
					} else {
						if (tektyp[0]=='^') {
							printf("Удаляю %s с диска\n",ffblk.ff_name);
							remove(ffblk.ff_name);
						}
						++modify;
					}
					farfree(tmp);
					j = findnext(&ffblk);
				}
				break;

			case '*':
				printf("Извлекаю: %s\n",tekname);
				filelen=SizeLib(tekname);
				tmp=(block)famemalloc(filelen);
				if (GetLib(tekname,tmp)==0) {
					printf("■ Не могу извлечь файл '%s' из библиотеки\n",tekname);
					farfree(tmp);
					break;
				}
				remove(tekname);
				if ( (fdz=open(tekname, O_WRONLY | O_CREAT | O_BINARY, S_IREAD | S_IWRITE)) == -1 ) {
					printf("■ ОШИБКА: не могу открыть файл '%s'",tekname);
					farfree(tmp);
					exit (1);
				}
				if ( write(fdz, tmp, filelen) == -1) {
					printf("■ ОШИБКА: не могу записать файл '%s'\n",tekname);
					farfree(tmp);
					exit (1);
				}
				close(fdz);
				farfree(tmp);
				break;

			case '@':
				tmp=(block)famemalloc((unsigned long)65530);
				for( j=0; j<numblk; ++j ) {
					printf("Извлекаю: %s, N %u\n",flib[j].name,j);
					filelen=flib[j].lenblk;
					if (GetLib(flib[j].name,tmp)==0) {
						printf("■ Не могу извлечь файл '%s' из библиотеки\n",flib[j].name);
						farfree(tmp);
						break;
					}
					remove(tekname);
					if ( (fdz=open(flib[j].name, O_WRONLY | O_CREAT | O_BINARY, S_IREAD | S_IWRITE)) == -1 ) {
						printf("■ ОШИБКА: не могу открыть файл '%s'",flib[j].name);
						farfree(tmp);
						exit (1);
					}
					if ( write(fdz, tmp, filelen) == -1) {
						printf("■ ОШИБКА: не могу записать файл '%s'\n",flib[j].name);
						farfree(tmp);
						exit (1);
					}
					close(fdz);
				}
				farfree(tmp);
				break;

			case '%':
				//уст. признак модификации для упаковки
				modify=1;
				break;

			default:
				printf("Пропускаю ошибочную команду: %s\n",tekname);
			}
		}

		if (modify) {
			puts("Уплотняю библиотеку...");
			PackLib();
		}

	} else {
		//вывод списка библиотеки
		lenblk_=lenpak_=0L;
		puts(" N    Имя файла  Размер Сжато  Арх  %");
		puts("--- ------------ ------ ------ --- ---");
		for (i=0; i<numblk; ++i) {
			//читаем одну запись MAP-файла
			printf("%3u %-12s %6lu %6lu %3s %3u\n",i+1,flib[i].name,flib[i].lenblk,flib[i].lenpak,(flib[i].comp?"RLE":"Нет"),flib[i].lenpak*100/flib[i].lenblk);
			lenblk_+=flib[i].lenblk;
			lenpak_+=flib[i].lenpak;
		}
		puts("--- ------------ ------ ------ --- ---");
		printf("%3u              %6lu %6lu     %3u\n",numblk,lenblk_,lenpak_,(int)(lenpak_*100/lenblk_));
	}
}

void fatalerror(char *t) {
	//выход по фатальной ошибке
	puts(t);
	exit(1);
}
