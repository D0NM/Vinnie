#include <stdlib.h>
#include <stdio.h>
#include <dos.h>

#include "keys.h"

#ifdef __cplusplus
#define __CPPARGS ...
#else
#define __CPPARGS
#endif

typedef void interrupt (*VECTPROC)(__CPPARGS);
static VECTPROC oldint09 = NULL;
static KB_UPROC kbu_func = NULL;

void kb_stub(unsigned char butt) {
/*  printf("Сканкод: %d\n", butt);*/
}

void interrupt newint09(__CPPARGS) {
unsigned char butt, v;
  disable();
  /* считаем введенный символ */
  butt = inp(0x60);
  /* инициализация клавиатуры */
  v = inp(0x61);
  outp(0x61, v | 0x80);
  outp(0x61, v);
  enable();
  /* вызоп п/п обработки клавиши */
  kbu_func(butt);
  /* конец прерывания */
  outp(0x20, 0x20);
/*  asm {
    cli
    in al,60h ; считаем введенный символ
    mov butt, al
    in al, 61h ; инициализация клавиатуры
    mov ah, al
    or al, 80h
    out 61h, al
    mov al, ah
    out 61h, al
    sti
  }
  kbu_func(butt); // вызоп п/п обработки клавиши
  asm {               ; конец прерывания
    mov  al, 20h
    out  20h, al
  }*/
}

void kb_on(void) {
  if (!oldint09) {
    if (!kbu_func) {
      kbu_func = kb_stub;
    }
    oldint09 = getvect(0x09);
    /* уст новый вектор прер */
    setvect(0x09, newint09);
  }
}

void kb_off(void) {
  if (oldint09) {
    /* уст новый вектор прер */
    setvect(0x09, oldint09);
    oldint09 = NULL;
  }
}

void kb_set(KB_UPROC funcaddr) {
  disable(); /* cli */
  kbu_func = funcaddr;
  enable(); /* sti */
}
