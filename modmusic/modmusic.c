#include <windows.h>
#include <mmsystem.h>
#include "in2.h"

#include "modmusic.h"

// == Winamp plugin stubs ================

static HANDLE he = 0;
static HWAVEOUT hwo = 0;
static HINSTANCE g_hLib = 0;
static In_Module *plugin = NULL;
// for loop song
static DWORD g_maxsize = 0;
static DWORD g_cursize = 0;
static DWORD g_secsize = 0;
static Out_Module outMod;
#define MAX_BUFF 8
#define MAX_SIZE 2304
static WAVEHDR wh[MAX_BUFF];
static BYTE wavedata[MAX_BUFF][MAX_SIZE];

#ifndef __udivdi3
// short version of function since (d) less than 32 bit anyway
// https://www.cs.usfca.edu/~benson/cs326/pintos/pintos/src/lib/arithmetic.c
inline DWORDLONG __udivdi3(DWORDLONG n, DWORDLONG d) {
DWORD n1, n0, q, r, x;
  n1 = n >> 32;
  n0 = n;
  x = d;
  asm ("divl %4"
       : "=d" (r), "=a" (q)
       : "0" (n1), "1" (n0), "rm" (x));
  return(q);
}
#endif

int modOpen(int samplerate, int numchannels, int bitspersamp, int bufferlenms, int prebufferms) {
WAVEFORMATEX wfe;
DWORD i;
  // create auto-reset event
  he = CreateEvent(NULL, FALSE, TRUE, NULL);
  if (!he) {
    return(-1);
  }
  // create waveOut handle
  wfe.wFormatTag = WAVE_FORMAT_PCM;
  wfe.nChannels = numchannels;
  wfe.nSamplesPerSec = samplerate;
  wfe.wBitsPerSample = bitspersamp;
  wfe.nBlockAlign = (wfe.wBitsPerSample / 8) * wfe.nChannels;
  wfe.nAvgBytesPerSec = wfe.nSamplesPerSec * wfe.nBlockAlign;
  wfe.cbSize = 0;
  if (waveOutOpen(&hwo, WAVE_MAPPER, &wfe, (DWORD) he, 0, CALLBACK_EVENT) != MMSYSERR_NOERROR) {
    CloseHandle(he);
    he = 0;
    return(-2);
  }
  // prepare buffers
  ZeroMemory(wavedata, MAX_BUFF * MAX_SIZE);
  for (i = 0; i < MAX_BUFF; i++) {
    ZeroMemory(&wh[i], sizeof(wh[0]));
    wh[i].lpData = (LPSTR) wavedata[i];
    wh[i].dwBufferLength = MAX_SIZE;
    if (waveOutPrepareHeader(hwo, &wh[i], sizeof(wh[0])) != MMSYSERR_NOERROR) {
      break;
    }
    // set flag so free buffer can be found easily later
    wh[i].dwFlags |= WHDR_DONE;
  }
  // something goes wrong
  if (i < MAX_BUFF) {
    for (; i; i--) {
      waveOutUnprepareHeader(hwo, &wh[i - 1], sizeof(wh[0]));
    }
    waveOutClose(hwo);
    CloseHandle(he);
    he = 0;
    return(-3);
  }
  // used for loop song later
  g_secsize = wfe.nAvgBytesPerSec;
  return(0);
}

void modClose(void) {
DWORD i;
  if (he) {
    // this should break WaitForSingleObject()
    CloseHandle(he);
    he = 0;
    // stop any playing buffers
    waveOutReset(hwo);
    // release buffers
    for (i = 0; i < MAX_BUFF; i++) {
      waveOutUnprepareHeader(hwo, &wh[i], sizeof(wh[0]));
    }
    waveOutClose(hwo);
  }
}

int modWrite(char *buf, int len) {
DWORD i, dwNum;
  // boost priority for playback thread
  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
  // sanity check
  if (buf && len) {
    // find free buffer
    dwNum = 0;
    while (!dwNum) {
      // for each buffer
      for (i = 0; i < MAX_BUFF; i++) {
        // check buffer status
        if (wh[i].dwFlags & WHDR_DONE) {
          // free buffer found
          dwNum = i + 1;
          break;
        }
      }
      // still no buffer found
      if (!dwNum) {
        // wait for one
        if (WaitForSingleObject(he, INFINITE) != WAIT_OBJECT_0) {
          // looks like playing stopped
          dwNum = MAX_BUFF + 1;
        }
      }
    }
    dwNum--;
    // buffer found and playing not stopped
    if (dwNum < MAX_BUFF) {
      wh[dwNum].dwFlags ^= WHDR_DONE;
      CopyMemory(wh[dwNum].lpData, buf, min(len, MAX_SIZE));
      waveOutWrite(hwo, &wh[dwNum], sizeof(wh[0]));
    }
  }
  if (!g_maxsize) {
//    // reset wave buffers
//    waveOutReset(hwo);
    // calc max song size
    g_maxsize = (((DWORDLONG) plugin->GetLength()) * (DWORDLONG) g_secsize) / 1000;
    // add data or song will stops before it can be looped
    // SetOutputTime() will not work for song that done playing and stopped
    g_cursize = MAX_SIZE;
  }
  g_cursize += len;
  if (g_cursize >= g_maxsize) {
    // reinit
    g_maxsize = 0;
    // go to the start
    plugin->SetOutputTime(0);
  }
  return(0);
}

int modCanWrite(void) {
  return(MAX_SIZE);
}

// this MUST return non-zero or some plugins doesn't send anything to Write()
int dsp_dosamples(short int *samples, int numsamples, int bps, int nch, int srate) {
  return(numsamples);
}

int ModStubProc(void) {
  return(0);
}

typedef In_Module *(*LPGETINMODULE2) (void);

// =======================================

unsigned long ModPlayMusic(char *filename) {
unsigned long result;
  result = 0;
  // plugin initialized
  if (plugin) {
    // stop if plays anything
    plugin->Stop();
    g_maxsize = 0;
    // try to play a new tune
    if (filename && (!plugin->Play(filename))) {
      result = plugin->GetLength();
    }
  }
  // return time so the tune can be looped if necessary
  return(result);
}

void ModFreeMusic(void) {
  // if initialized
  if (g_hLib) {
    // stop music
    ModPlayMusic(NULL);
    // cleanup plugin
    if (plugin) {
      plugin->Quit();
    }
    plugin = NULL;
    // unload
    FreeLibrary(g_hLib);
    g_hLib = 0;
  }
}

void ModInitMusic(void) {
DWORD i, *d;
  // just in case
  ModFreeMusic();
  // load library
  g_hLib = LoadLibrary("IN_SK00L.DLL");
  if (g_hLib) {
    plugin = (In_Module *) GetProcAddress(g_hLib, "winampGetInModule2");
    if (plugin) {
      // output plugin
      ZeroMemory(&outMod, sizeof(outMod));
      outMod.version = OUT_VER;
      outMod.hDllInstance = g_hLib;
      // output plugin stubs
      d = (DWORD *) &outMod.Config;
      for (i = 0; i < 15; i++) {
        d[i] = (DWORD) &ModStubProc; // MAKEWORD(i, 0xF0);
      }
      outMod.Open = modOpen;
      outMod.Close = modClose;
      outMod.Write = modWrite;
      outMod.CanWrite = modCanWrite;
      // input plugin
      plugin = ((LPGETINMODULE2) plugin)();
      plugin->outMod = &outMod;
      plugin->hDllInstance = g_hLib;
      // input plugin stubs
      d = (DWORD *) &plugin->SAVSAInit;
      for (i = 0; i < 13; i++) {
        if (!d[i]) {
          d[i] = (DWORD) &ModStubProc; // MAKEWORD(i, 0xF1);
        }
      }
      // this must be set
      plugin->dsp_dosamples = dsp_dosamples;
      // init internal structs
      plugin->Init();
    } else {
      // not a Winamp plugin
      ModFreeMusic();
    }
  }
}

void ModLoudMusic(int volume) {
  if (he) {
    // byte to word (0xFF => 0xFFFF)
    waveOutSetVolume(hwo, volume * 0x0101);
  }
}
