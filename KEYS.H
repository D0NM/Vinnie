#ifndef __KEYS_H
#define __KEYS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void far (*KB_UPROC)(unsigned char);

void kb_on(void);
void kb_off(void);
void kb_set(KB_UPROC funcaddr);

#ifdef __cplusplus
}
#endif

#endif
