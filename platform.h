#ifndef __PLATFORM_H
#define __PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

void delay(unsigned short __milliseconds);
short random(short __num);
unsigned long coreleft(void);
#define C80 3
void textmode(short newmode);

// DOS far/near model stub
#define far
#define farfree free
#define _fmemset memset
#define _fmemcpy memcpy
#define farmalloc malloc

void sysDebugOut(const char *fmt, ...);
int sysStart(void);
void sysSetPalNum(unsigned char n, unsigned char r, unsigned char g, unsigned char b);
void *sysGetVidScreen(void);
void sysWaitForVSync(void);
long sysGetTicks(void);

#ifdef __cplusplus
}
#endif

#endif
