#ifndef __MODMUSIC_H
#define __MODMUSIC_H

unsigned long ModPlayMusic(char *filename);
void ModFreeMusic(void);
void ModInitMusic(void);
void ModLoudMusic(int volume);

#endif
