#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "famegraf.h"
typedef /*unsigned*/ char *block;
typedef void *screen;

extern int SetLib(char *);
extern int PutLib(char *, block, unsigned long);
extern int GetLib(char *, block);

#include <windows.h>

void fatalerror(char *t) {
  MessageBox(0, t, "SetPoohWin32", MB_ICONERROR);
  exit(1);
}

#pragma pack(push, 1)
struct {
  short dev,mix,vol;
  short j;
  short f;
  short cheat;
} setup;
#pragma pack(pop)

int AskAbout(char *t) {
  return(
    (MessageBox(0,
      t ? t : "Done!",
      "WinSetPooh32",
      t ? (MB_YESNO | MB_ICONQUESTION) : (MB_OK | MB_ICONINFORMATION)
    ) == IDYES) ? 1 : 0
  );
}

int main(int argc, char *argv[]) {
  SetLib("graph");
  memset(&setup, 0, sizeof(setup));
  GetLib("pooh.cfg", (block) &setup);
  setup.dev = AskAbout("Allow music?") ? 7 : (-1);
  setup.vol = 255;
  setup.j = AskAbout("Allow joystick / gamepad / controller?") ? 1 : 0;
  setup.f = 3;
  setup.cheat = ((argc == 2) && (stricmp(argv[1],"cheat")==0)) ? 1 : 0;
  PutLib("pooh.cfg", (block) &setup, sizeof(setup));
  AskAbout(NULL);
  return(0);
}
