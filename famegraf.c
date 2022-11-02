#include <string.h>
#include "famegraf.h"

#ifndef __MSDOS__
#include "platform.h"
#endif

int MaxX;
int MaxY;
int MinX;
int MinY;

unsigned char vgapal[256][3];
//unsigned char vgascr[320 * 200];

screen CurrentScreen;
block CurrentAdr;
block CurrentAdr1;
block FontData;
screen SpritesScreen;
screen VScreen;
unsigned char CurrentColor;

// set video mode 320x200 256 colors
void Vga256(void) {
  // FIXME: go to graphic mode
//  memset(vgascr, 0, sizeof(vgascr));
  // FIXME: init palette with DOS VGA default
  VScreen = sysGetVidScreen(); // всегда визуальный экран
  CurrentScreen = VScreen;//vgascr;
  CurrentAdr = (block) VScreen;//vgascr;
  FontData = NULL;
  MinX = 0;
  MinY = 0;
  MaxX = 319;
  MaxY = 199;
  sysStart();
}

// уст адрес вывода графики и тп
void SetScreen(screen x) {
  CurrentScreen = x;
}

// уст адрес вывода графики в A000
void NormalScreen(void) {
  CurrentScreen = VScreen;//vgascr;
}

// копирование buf1 -> buf
void ScreenCopy(screen buf, screen buf1) {
  memmove(buf, buf1, 320 * 200);
}

// ожидание взм. отрисовки луча
void WVR(void) {
  sysWaitForVSync();
}

// копирование buf -> экран
void CopyToScreen(screen buf){
  WVR();
  memmove(VScreen/*vgascr*/, buf, 320 * 200);
}

// очистка экрана
void Cls(char x) {
  memset(CurrentScreen, 0, 320 * 200);
}

// установка текущего цвета
void SetColor(char x) {
  CurrentColor = x;
}

// установить один цвет палитры
void PutRGB(char n, char r, char g, char b) {
  vgapal[(unsigned char) n][0] = (unsigned char) r;
  vgapal[(unsigned char) n][1] = (unsigned char) g;
  vgapal[(unsigned char) n][2] = (unsigned char) b;
  sysSetPalNum((unsigned char) n, (unsigned char) r, (unsigned char) g, (unsigned char) b);
}

//void PutPalette(block a){}

// взять текущую палитру
void GetPalette(block buf) {
  memcpy(buf, vgapal, sizeof(vgapal));
}

// установить текущий шрифт (8*8)
void PutFont(block buf) {
  FontData = buf;
}

// ширина символа
int GetCharWidth(int c) {
  return(FontData ? FontData[(256 * 8) + ((unsigned char) c)] : 0);
}

// вывести символ текущим шрифтом
void DisplayChar(int c, int x, int y, char fgd, char bkgd) {
unsigned char *p, *b;
int i, j;
  if (FontData) {
    p = (unsigned char *) CurrentScreen;
    p += (320 * y) + x;
    b = (unsigned char *) &FontData[((unsigned char) c) * 8];
    for (j = 0; j < 8; j++) {
      for (i = 0; i < 8; i++) {
        if (*b & (0x80 >> i)) {
          p[i] = fgd;
        } else {
          // zero - transparent
          if (bkgd) { p[i] = bkgd; }
        }
      }
      p += 320;
      b++;
    }
  }
}

// установка окна обрезания
void Clip(int nx, int ny, int xx, int xy) {
  MinX = nx;
  MinY = ny;
  MaxX = xx;
  MaxY = xy;
}

// нарисовать точку тек.цветом
void PutPixel(int x, int y) {
unsigned char *p;
  p = (unsigned char *) CurrentScreen;
  p[(y * 320) + x] = CurrentColor;
}

// считать точку с экрана
int GetPixel(int x, int y) {
unsigned char *p;
  p = (unsigned char *) CurrentScreen;
  return(p[(y * 320) + x]);
}

// Закрашенный прямоугольник
void Bar(int x, int y, int xl, int yl)  {
unsigned char *p;
  p = (unsigned char *) CurrentScreen;
  p += (320 * y) + x;
  for (; yl > 0; yl--) {
    memset(p, CurrentColor, xl);
    p += 320;
  }
}

//void Rectangle(int a, int b, int c, int d){}
//void Line(int a, int b, int c, int d){}

// вывод блока на экран
void PutImg(int x, int y, int xl, int yl, block buf) {
unsigned char *p, *d;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  for (; yl > 0; yl--) {
    memmove(p, d, xl);
    p += 320;
    d += xl;
  }
}

// считать блок с экрана
void GetImg(int x, int y, int xl, int yl, block buf) {
unsigned char *p, *d;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  for (; yl > 0; yl--) {
    memmove(d, p, xl);
    p += 320;
    d += xl;
  }
}

// вывод блока с прозрачным цветом на экран
void PutMas(int x, int y, int xl, int yl, block buf) {
unsigned char *p, *d;
int i;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  for (; yl > 0; yl--) {
    for (i = 0; i < xl; i++) {
      // zero - transparent
      if (*d) { p[i] = *d; }
      d++;
    }
    p += 320;
  }
}

// r - right/reverse (horizontal inversion)
void PutMasr(int x, int y, int xl, int yl, block buf) {
unsigned char *p, *d;
int i;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  for (; yl > 0; yl--) {
    d += xl;
    for (i = 0; i < xl; i++) {
      d--;
      // zero - transparent
      if (*d) { p[i] = *d; }
    }
    p += 320;
    d += xl;
  }
}

// вывод мерцающего силуэта
void PutBlink(int x, int y, int xl, int yl, block buf) {
unsigned char *p, *d;
int i;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  for (; yl > 0; yl--) {
    for (i = 0; i < xl; i++) {
      // zero - transparent
      if (*d) { p[i] = CurrentColor; }
      d++;
    }
    p += 320;
  }
}

// r - right/reverse (horizontal inversion)
void PutBlinkr(int x, int y, int xl, int yl, block buf) {
unsigned char *p, *d;
int i;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  for (; yl > 0; yl--) {
    d += xl;
    for (i = 0; i < xl; i++) {
      d--;
      // zero - transparent
      if (*d) { p[i] = CurrentColor; }
    }
    p += 320;
    d += xl;
  }
}

// вывод блока 16*16 на экран
void PutImg16(int x, int y, block buf) {
unsigned char *p, *d;
int l;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  l = 16;
  while (l--) {
    memmove(p, d, 16);
    d += 16;
    p += 320;
  }
}

// вывод блока 16*16 с прозрачным цветом на экр
void PutMas16(int x, int y, block buf) {
unsigned char *p, *d;
int l, i;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  l = 16;
  while (l--) {
    for (i = 0; i < 16; i++) {
      // zero - transparent
      if (*d) { p[i] = *d; }
      d++;
    }
    p += 320;
  }
}

// вычислить начальный адрес на экране
void CalcAdr(int x, int y) {
  CurrentAdr = (block) CurrentScreen;
  CurrentAdr += (y * 320) + x;
}

// без коорд вывод блока 16*16 на экран
// вывод по адресу
void SPutImg16(void) {
unsigned char *p, *d;
int l;
  p = (unsigned char *) CurrentAdr;
  d = (unsigned char *) CurrentAdr1;
  l = 16;
  while (l--) {
    memmove(p, d, 16);
    d += 16;
    p += 320;
  }
}

// без коорд вывод блока 16*16 с прозрачным цветом
// вывод по адресу
void SPutMas16(void) {
unsigned char *p, *d;
int l, i;
  p = (unsigned char *) CurrentAdr;
  d = (unsigned char *) CurrentAdr1;
  l = 16;
  while (l--) {
    for (i = 0; i < 16; i++) {
      // zero - transparent
      if (*d) { p[i] = *d; }
      d++;
    }
    p += 320;
  }
}

//void PutCImg(int a, int b, int c, int d, block z){}
//void PutCMas(int a, int b, int c, int d, block z){}
//void PutCMasr(int a, int b, int c, int d, block z){}
//void PutCBlink(int a, int b, int c, int d, block z){}
//void PutCBlinkr(int a, int b, int c, int d, block z){}

// вывод с обрезкой спрайта на экран
void PutSMas(int x, int y, int xl, int yl, int xr, int yr, int xrl, int yrl, block buf) {
unsigned char *p, *d;
int l, i;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  p--;
  d += (xl * yr) + xr; // вычисляем смещение прямоугольной области в спр-те
  for (l = yrl; l > 0; l--) {
    for (i = 0; i < xrl; i++) {
      if (d[i]) { p[i] = d[i]; }
    }
    d += xl;
    p += 320;
  }
}

// r - right/reverse (horizontal inversion)
void PutSMasr(int x, int y, int xl, int yl, int xr, int yr, int xrl, int yrl, block buf) {
unsigned char *p, *d;
int l, i;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  p++;
  d += (xl * yr) + xr; // вычисляем смещение прямоугольной области в спр-те
  for (l = yrl; l > 0; l--) {
    for (i = 0; i < xrl; i++) {
      if (d[i]) { p[xrl - 1 - i] = d[i]; }
    }
    d += xl;
    p += 320;
  }
}

// вывод с обрезкой спрайта на экран
void PutSImg(int x, int y, int xl, int yl, int xr, int yr, int xrl, int yrl, block buf) {
unsigned char *p, *d;
int l;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  d += (xl * yr) + xr; // вычисляем смещение прямоугольной области в спр-те
  for (l = yrl; l > 0; l--) {
    memmove(p, d, xrl);
    d += xl;
    p += 320;
  }
}

// вывод с обрезкой мерцающего силуэта
void PutSBlink(int x, int y, int xl, int yl, int xr, int yr, int xrl, int yrl, block buf) {
unsigned char *p, *d;
int l, i;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  p--;
  d += (xl * yr) + xr; // вычисляем смещение прямоугольной области в спр-те
  for (l = yrl; l > 0; l--) {
    for (i = 0; i < xrl; i++) {
      if (d[i]) { p[i] = CurrentColor; }
    }
    d += xl;
    p += 320;
  }
}

// r - right/reverse (horizontal inversion)
void PutSBlinkr(int x, int y, int xl, int yl, int xr, int yr, int xrl, int yrl, block buf) {
unsigned char *p, *d;
int l, i;
  p = (unsigned char *) CurrentScreen;
  d = (unsigned char *) buf;
  p += (320 * y) + x;
  p++;
  d += (xl * yr) + xr; // вычисляем смещение прямоугольной области в спр-те
  for (l = yrl; l > 0; l--) {
    for (i = 0; i < xrl; i++) {
      if (d[i]) { p[xrl - 1 - i] = CurrentColor; }
    }
    d += xl;
    p += 320;
  }
}

// копирование прямоугольного блока памяти
void CopyBlock(int x, int y, int xl, int yl, screen buf, int x1, int y1, screen buf1) {
unsigned char *p, *s;
  p = buf;
  s = buf1;
  p += (320 * y) + x;
  s += (320 * y1) + x1;
  while (yl--) {
    memmove(p, s, xl);
    p += 320;
    s += 320;
  }
}

// копирование прямоуг блока памяти на экр
void CopyBlock0(screen buf) {
unsigned char *p, *s;
int l;
  p = VScreen;//vgascr;
  s = buf;
  p += (320 * 16) + 16;
  s += (320 * 16) + 16;
  l = 16 * 10;
  while (l--) {
    memmove(p, s, 16 * 16);
    p += 320;
    s += 320;
  }
  WVR();
}

long gettic(void) {
  return(sysGetTicks());
}

/*void CBar(int a, int b, int c, int d){}
void TileBar(int a, int b, int c, int d, int e,int f,block g){}
void CopyCBlock(int a, int b, int c, int d, screen dest, int e, int f, screen src){}

void PaletteOn(block f) {}
void PaletteOff(block f) {}
block fonts;
block palette;
block palette1;

void InitGraph(void) {}
void CloseGraph(void) {}
void MoveXY(int x, int y) {}
void vputs(char * f) {}
void WPut(int x, int y, int lx, int ly) {}
void vputsc(char * f){}
void vputBs(char * f,block buf) {}


unsigned int famestrlen(unsigned char *s) { return(8); }
int GetString(char *str,int lstr) { return(0); }
char CharUp(unsigned char i) { return(0); }
void PutMtb(int x,int y,int lx,int ly,int mx,int my,block buffer) {}
int vprintB( block buf, char *fmt, ... ) {}
int vprint( char *fmt, ... ) {}
void Swap (int *pa,int *pb) {}

char char_fgd=17;
char char_bkgd=0;

void CopyBlock(int a,int b,int c,int d,screen dest,int e,int f,screen src) {}
void putBch(int x, int y, unsigned char c, block buf) {}
void PutCh(unsigned char c_) {}
*/
