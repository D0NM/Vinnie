#ifndef __JOY_H
#define __JOY_H

#ifdef __cplusplus
extern "C" {
#endif

#define JOY1_L 0x01
#define JOY1_R 0x02
#define JOY1_U 0x04
#define JOY1_D 0x08
#define JOY2_L 0x10
#define JOY2_R 0x20
#define JOY2_U 0x40
#define JOY2_D 0x80
#define JOY1_A 0x100
#define JOY1_B 0x200
#define JOY2_A 0x400
#define JOY2_B 0x800

typedef void far (*JS_UPROC)(unsigned short);

void js_init(void);
int js_state(void);
void js_on(void);
void js_off(void);
void js_set(JS_UPROC funcaddr);
void js_read(void);
void js_test(void);

#ifdef __cplusplus
}
#endif

#endif
