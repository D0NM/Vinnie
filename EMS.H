#define NOTINSTALLED 1
#define NOERROR 0

extern int EmsError;	//?訡??-???? EMS ?ࠩ????
extern int EmsPresent;	//???? ?? EMS ?ࠩ???
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