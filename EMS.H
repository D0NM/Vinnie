#define NOTINSTALLED 1
#define NOERROR 0

extern int EmsError;	//ошибка-сост EMS драйвера
extern int EmsPresent;	//есть ли EMS драйвер
int EmsInstalled(void);
int EmsVersion(void);
int EmsPagesAvail(void);
int EmsAlloc(int Pages);
block EmsFrameAdr(void);
int EmsMap(int Handle, int Pages[]);
int EmsFree(int Handle);
int EmsSave(int Handle);
int EmsRestore(int Handle);
char *getEMMerror(int errnum);
