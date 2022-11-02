#include <stdlib.h>
#include <stdio.h>
#include <dos.h>

#include "joy.h"

extern char far	a_1;
extern char far	a_2;
extern char far	b_1;
extern char far	b_2;

extern int far	xa;
extern int far	ya;
extern int far	xb;
extern int far	yb;

extern void far jstat(void);

static JS_UPROC jsu_func = NULL;
static char js_enabled = 0;
static char js_devices = 0;
/* пороги чувствительности */
unsigned int fleft, fright, fup, fdown;

void js_stub(unsigned short butt) {
}

void js_init(void) {
  if (!jsu_func) {
    jsu_func = js_stub;
  }
  jstat();
  js_devices = 0;
  js_enabled = 0;
  js_devices |= (xa || ya) ? 1 : 0;
  js_devices |= (xb || yb) ? 2 : 0;
}

int js_state(void) {
  return(js_devices);
}

void js_on(void) {
  js_enabled = js_devices ? 1 : 0;
}

void js_off(void) {
  js_enabled = 0;
}

void js_set(JS_UPROC funcaddr) {
  disable(); /* cli */
  jsu_func = funcaddr;
  enable(); /* sti */
}

void js_read(void) {
unsigned short butt;
  /* joystick enabled */
  if (js_enabled) {
    jstat();
    butt = 0;
    /* joystick 1 */
    if (js_devices & 1) {
      butt |= (xa <  fleft) ? JOY1_L : 0;
      butt |= (xa > fright) ? JOY1_R : 0;
      butt |= (ya <    fup) ? JOY1_U : 0;
      butt |= (ya >  fdown) ? JOY1_D : 0;
      /* button state inverse in DOS */
      butt |= a_1 ? 0 : JOY1_A;
      butt |= a_2 ? 0 : JOY1_B;
    }
    if (js_devices & 2) {
      /* joystick 2 */
      butt |= (xb <  fleft) ? JOY2_L : 0;
      butt |= (xb > fright) ? JOY2_R : 0;
      butt |= (yb <    fup) ? JOY2_U : 0;
      butt |= (yb >  fdown) ? JOY2_D : 0;
      /* button state inverse in DOS */
      butt |= b_1 ? 0 : JOY2_A;
      butt |= b_2 ? 0 : JOY2_B;
    }
    /* call user defined function */
    jsu_func(butt);
  }
}

/* настройка джойстика */
void js_test(void) {
unsigned int mleft, mright, mup, mdown, x, y; /* мах значения */
  /* ручка в центре */
  do {
    jstat();
  } while (a_1 && a_2);
  x = xa;
  y = ya;
  do {
    jstat();
  } while ((!a_1) || (!a_2));
  /* ручка в левом вехнем углу */
  do {
    jstat();
  } while (a_1 && a_2);
  mleft = xa;
  mup = ya;
  do {
    jstat();
  } while ((!a_1) || (!a_2));
  /* ручка в правом нижнем углу */
  do {
    jstat();
  } while (a_1 && a_2);
  mright = xa;
  mdown = ya;
  do {
    jstat();
  } while ((!a_1) || (!a_2));
  fleft = mleft + ((x - mleft) / 2);
  fright = mright - ((mright - x) / 2);
  fup = mup + ((y - mup) / 2);
  fdown = mdown - ((mdown - y) / 2);
}
