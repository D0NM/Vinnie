#include <windows.h>
#include <mmsystem.h>
#include "platform.h"
#include "modmusic/modmusic.h"

#include "keys.h"
#include "joy.h"

#pragma pack(push, 1)
typedef struct {
  BITMAPFILEHEADER head;
  BITMAPINFOHEADER info;
  RGBQUAD cpal[256];
  BYTE data[320 * 200];
} bmp_data;

typedef struct {
  BITMAPFILEHEADER head;
  BITMAPINFOHEADER info;
  RGBQUAD cpal[256];
} bmp_head;

typedef struct {
  bmp_head bh; // bitmap image header
  DWORD _w;    // fullscreen (desktop) width
  DWORD _h;    // fullscreen (desktop) height
  DWORD w;     // width of resized image for fullscreen mode
  DWORD h;     // height of resized image for fullscreen mode
  DWORD x;     // x position of image for keep-aspect fullscreen mode (black bars)
  DWORD y;     // y position of image for keep-aspect fullscreen mode (black bars)
  DWORD wx;    // windowed mode top window position
  DWORD wy;    // windowed mode left window position
  BYTE *p;     // bitmap data buffer for resized image
  BOOL b;      // block any operations in fullscreen <-> windowed transition (1/0)
  BOOL f;      // fullscreen flag (1/0)
} scr_head;
#pragma pack(pop)

static bmp_data scr_data;
static scr_head fscr;
static HWND hWndMain;
static UINT hVSyncProc;
static LONG lSyncFlag = 0;

static KB_UPROC kbu_func = NULL;
static char kb_state = 0;

static dwShotNum = 0;
void DumpScreen(void) {
bmp_data scr_copy;
TCHAR s[1025];
HANDLE fl;
DWORD dw;
  // make a copy of current frame since searching for non-existing file may take a while
  CopyMemory(&scr_copy, &scr_data, sizeof(scr_copy));
  do {
    dwShotNum = (dwShotNum + 1) % 10000;
    wsprintf(s, TEXT("POOH%04u.BMP"), dwShotNum);
  } while (dwShotNum && (GetFileAttributes(s) != INVALID_FILE_ATTRIBUTES));
  if (dwShotNum) {
    fl = CreateFile(s, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (fl != INVALID_HANDLE_VALUE) {
      WriteFile(fl, &scr_copy, sizeof(scr_copy), &dw, NULL);
      CloseHandle(fl);
    }
  }
}

void WINAPIV sysDebugOut(const char *fmt, ...) {
char s[1025];
va_list args;
  va_start(args, fmt);
  wvsprintfA(s, fmt, args);
  va_end(args);
  OutputDebugStringA(s);
}

#define BMPLINESIZE(w, b) (((((w) * (b)) + 31) & ~31) >> 3)
int FillBitmapHeader(BYTE *buf, int iwidth, int iheight, int ibpp) {
BITMAPFILEHEADER *head;
BITMAPINFOHEADER *info;
int hsize, dsize;
  hsize = sizeof(head[0]) + sizeof(info[0]) + (((ibpp <= 8) ? (1 << ibpp) : 0) * 4);
  dsize = BMPLINESIZE(iwidth, ibpp) * iheight;
  if (buf) {
    head = (BITMAPFILEHEADER *) buf;
    info = (BITMAPINFOHEADER *) &buf[sizeof(head[0])];
    ZeroMemory(head, sizeof(head[0]));
    ZeroMemory(info, sizeof(info[0]));
    info->biSize = sizeof(info[0]);
    info->biWidth = iwidth;
    info->biHeight = -iheight; // negative height: top to bottom image
    info->biPlanes = 1;
    info->biBitCount = ibpp;
    head->bfType = 0x4D42;
    head->bfSize = hsize + dsize;
    head->bfOffBits = hsize;
    hsize = sizeof(head[0]) + sizeof(info[0]);
    dsize = 0;
  }
  return(hsize + dsize);
}

// fast image resize
void ResizeToScreen(const BYTE *sb) {
DWORD i, j, coffx, coffy, cx, cy;
BYTE *p;
  if (sb && fscr.f && fscr.p && (!fscr.b)) {
    // TODO: replace hardcoded 320 and 200
    coffx = (320UL << 16) / fscr.w;
    coffy = (200UL << 16) / fscr.h;
    cy = 0;
    for (j = 0; j < fscr.h; j++) {
      p = (BYTE *) &fscr.p[((j + fscr.y) * fscr._w) + fscr.x];
      cx = 0;
      for (i = 0; i < fscr.w; i++) {
        *p = sb[((cy >> 16) * 320) + (cx >> 16)];
        cx += coffx;
        p++;
      }
      cy += coffy;
    }
  }
}

#define GetMem(x) ((void *) LocalAlloc(LPTR, (x)))
#define FreeMem(x) LocalFree((x))

void SetWndStyle(HWND wnd) {
DWORD dwStyle, x, y;
RECT rc;
  fscr.b = TRUE;
  if (fscr.f) {
    dwStyle = WS_VISIBLE | WS_SYSMENU | WS_POPUP;
    x = 0;
    y = 0;
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    // ---
    fscr._w = rc.right;
    fscr._h = rc.bottom;
    FillBitmapHeader((BYTE *) &fscr.bh, fscr._w, fscr._h, 8);
    fscr.w = fscr._w;
    fscr.h = fscr._h;
    if (1) { /* FIXME: preserve aspect */
      if ((fscr.w * 3) != (fscr.h * 4)) {
        if ((fscr.w / 4) > (fscr.h / 3)) {
          fscr.w = (fscr.h * 4) / 3;
        } else {
          fscr.h = (fscr.w * 3) / 4;
        }
      }
    }
    fscr.x = (fscr._w - fscr.w) / 2;
    fscr.y = (fscr._h - fscr.h) / 2;
    fscr.p = GetMem(fscr._w * fscr._h);
    if (fscr.p) { ZeroMemory(fscr.p, fscr._w * fscr._h); }
  } else {
    dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
    x = fscr.wx;
    y = fscr.wy;
    rc.left = 0;
    rc.top = 0;
    rc.right = 320;
    rc.bottom = 200;
    // ---
    if (fscr.p) {
      FreeMem(fscr.p);
      fscr.p = NULL;
    }
  }
  AdjustWindowRect(&rc, dwStyle, FALSE);
  SetWindowLong(wnd, GWL_STYLE, dwStyle);
  SetWindowPos(wnd, fscr.f ? HWND_TOPMOST : HWND_NOTOPMOST,
    x, y, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED
  );
  fscr.b = FALSE;
}

void GameDraw(HWND wnd, bmp_data *scr_copy) {
HDC hc;
  if (wnd) {
    hc = GetDC(wnd);
    if (hc) {
      if ((!fscr.f) || (!fscr.p) || (fscr.b)) {
        // direct screen output
        SetDIBitsToDevice(hc,
          0, 0, 320, 200,
          0, 0, 0, 200,
          (void *) scr_copy->data,
          (BITMAPINFO *) &scr_copy->info,
          DIB_RGB_COLORS
        );
      } else {
        // resize image - a lot faster than call to StretchDIBits()
        ResizeToScreen(scr_copy->data);
        // update palette
        CopyMemory(fscr.bh.cpal, scr_copy->cpal, sizeof(fscr.bh.cpal));
        // direct screen output
        SetDIBitsToDevice(hc,
          0, 0, fscr._w, fscr._h,
          0, 0, 0, fscr._h,
          (void *) fscr.p,
          (BITMAPINFO *) &fscr.bh.info,
          DIB_RGB_COLORS
        );
      }
      ReleaseDC(wnd, hc);
      ValidateRect(wnd, NULL);
    }
  }
}

void CALLBACK FrameUpdate(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) {
bmp_data scr_copy;
  // draw only if not busy
  if (!InterlockedExchange(&lSyncFlag, 1)) {
    // make a local copy of whole frame
    CopyMemory(&scr_copy, &scr_data, sizeof(scr_copy));
    // draw frame
    GameDraw((HWND) dwUser, &scr_copy);
    // unlock state
    InterlockedExchange(&lSyncFlag, 0);
  }
}

int GameInit(HWND wnd) {
  ZeroMemory(&scr_data, sizeof(scr_data));
  FillBitmapHeader((BYTE *) &scr_data, 320, 200, 8);
  // screen update
  hVSyncProc = timeSetEvent(/*dwTicksPerFrame*/1000 / 60, 0, FrameUpdate, (DWORD) wnd, TIME_PERIODIC);
  timeBeginPeriod(1);
  return(hVSyncProc ? 1 : 0);
}

// class and title
static TCHAR WndClassName[] = TEXT("PoohWin32");

#define WM_MODE_XCHG (WM_USER + 101)
LRESULT CALLBACK WndProc(HWND wnd, UINT umsg, WPARAM wparm, LPARAM lparm) {
RECT rc;
  switch (umsg) {
    // main window just created - center it
    case WM_CREATE:
      ZeroMemory(&fscr, sizeof(fscr));
      SetWndStyle(wnd);
      GetWindowRect(wnd, &rc);
      fscr.wx = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2;
      fscr.wy = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2;
      SetWindowPos(wnd, 0, fscr.wx, fscr.wy, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
      if (!GameInit(wnd)) {
        MessageBox(wnd, WndClassName, WndClassName, MB_ICONERROR);
        return(-1);
      }
      hWndMain = wnd;
      PostMessage(wnd, WM_MODE_XCHG, 0, 0);
      break;
    // fullscreen / windowed mode change
    case WM_MODE_XCHG:
      fscr.f ^= TRUE;
      if (fscr.f) {
        GetWindowRect(wnd, &rc);
        fscr.wx = rc.left;
        fscr.wy = rc.top;
      }
      SetWndStyle(wnd);
      return(0);
    // prevent system menu and hotkeys
    // Alt, F10, Alt+Space and left-click on window icon
    case WM_SYSCOMMAND:
      if (!(
        // Alt, F10, Alt+Space
        (wparm == SC_KEYMENU) ||
        // click on the window icon, 0xF093 == SC_MOUSEMENU | HTSYSMENU
        (wparm == (SC_MOUSEMENU | HTSYSMENU)) ||
        // double click on the window icon, 0xF063 == SC_CLOSE | HTSYSMENU
        (wparm == (SC_CLOSE | HTSYSMENU))
      )) {
        break;
      }
      // fallthrough
    // right click menu on the window caption
    case WM_CONTEXTMENU:
      // fallthrough
    // do not redraw
    case WM_PAINT:
      return(0);
      break;
    // hide cursor only for client area and allow for the title bar and controls
    case WM_SETCURSOR:
      // hide in fullscreen only
      if (fscr.f) {
        SetCursor((LOWORD(lparm) == HTCLIENT) ? NULL : LoadCursor(NULL, IDC_ARROW));
        return(TRUE);
      }
      break;
    // key pressed
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
      // switch between fullscreen / windowed mode
      if (wparm == VK_F11) {
        PostMessage(wnd, WM_MODE_XCHG, 0, 0);
        break;
      }
      // take a screenshot
      if (wparm == VK_F12) {
        DumpScreen();
        break;
      }
      if (kb_state && kbu_func) { kbu_func(MapVirtualKey(wparm, 0) & 0x7F); }
      break;
    // key released
    case WM_SYSKEYUP:
    case WM_KEYUP:
      // from fullscreen / windowed mode and screenshots
      if ((wparm == VK_F11) || (wparm == VK_F12)) { break; }
      if (kb_state && kbu_func) { kbu_func((MapVirtualKey(wparm, 0) & 0x7F) | 0x80); }
      break;
    // cleanup game engine
    case WM_DESTROY:
      timeEndPeriod(1);
      if (hVSyncProc) { timeKillEvent(hVSyncProc); hVSyncProc = 0; }
      hWndMain = 0;
      PostQuitMessage(0);
      return(0);
      break;
  }
  return(DefWindowProc(wnd, umsg, wparm, lparm));
}

DWORD WINAPI MainThread(LPVOID parm) {
WNDCLASSEX wce;
MSG wmsg;
HWND wnd;
RECT rc;
//  InitCommonControls();

  ZeroMemory(&wce, sizeof(wce));
  wce.lpszClassName = WndClassName;
  wce.cbSize = sizeof(wce);
  wce.hInstance = GetModuleHandle(NULL);
  wce.lpfnWndProc = &WndProc;
  wce.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW; // CS_OWNDC ???
  wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wce.hCursor = LoadCursor(NULL, IDC_ARROW);

  // only one instance allowed
  wnd = FindWindow(wce.lpszClassName, NULL);
  if (!wnd) {
    // register window class
    if (RegisterClassEx(&wce)) {
      // create window
      wnd = CreateWindowEx(
        0, wce.lpszClassName, wce.lpszClassName,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
        0, 0, 320, 200,
        0, 0,
        wce.hInstance, NULL
      );
      // window created
      if (wnd) {
        // set new window size and position
        rc.left   = 0;
        rc.top    = 0;
        rc.right  = 320;
        rc.bottom = 200;
        AdjustWindowRect(&rc, GetWindowLong(wnd, GWL_STYLE), 0);
        rc.right  -= rc.left;
        rc.bottom -= rc.top;
        rc.left    = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
        rc.top     = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
        SetWindowPos(wnd, HWND_TOP, rc.left, rc.top, rc.right, rc.bottom, 0);
        // default windows message loop
        while (GetMessage(&wmsg, 0, 0, 0)) {
          TranslateMessage(&wmsg);
          DispatchMessage(&wmsg);
        }
      }
      // unregister class
      UnregisterClass(wce.lpszClassName, wce.hInstance);
    }
    if (!wnd) {
      // can't register window class
      MessageBox(0, WndClassName, NULL, MB_ICONERROR);
    }
  } else {
    SetForegroundWindow(wnd);
  }
  ExitProcess(0); // FIXME:!!!
  return(0);
}

int sysStart(void) {
HANDLE hThread;
DWORD ti;
  hWndMain = 0;
  hThread = CreateThread(NULL, 0, MainThread, NULL, 0, &ti);
  if (hThread) {
    CloseHandle(hThread);
    ti = 0;
    while (!hWndMain) {
      Sleep(10);
      ti++;
      if (ti >= 200) { break; }
    }
  }
  return((hThread && hWndMain) ? 1 : 0);
}

void sysSetPalNum(unsigned char n, unsigned char r, unsigned char g, unsigned char b) {
  scr_data.cpal[n].rgbRed   = (r << 2) | (r >> 4);
  scr_data.cpal[n].rgbGreen = (g << 2) | (g >> 4);
  scr_data.cpal[n].rgbBlue  = (b << 2) | (b >> 4);
}

void *sysGetVidScreen(void) {
  return(scr_data.data);
}

// 60 FPS - DOS monitor refresh rate
DWORD dwPF = 0;
void sysWaitForVSync(void) {
DWORDLONG x;
//  if (hVSyncEvent) {
//    WaitForSingleObject(hVSyncEvent, INFINITE);
//  }
  do {
    x = timeGetTime();
    x *= 3;
    x /= 50;
  } while (((DWORD) x) == dwPF);
  dwPF = x;
}

// 1193180 / 65536 ~ 18.206482 - DOS timer update rate
long sysGetTicks(void) {
DWORDLONG x;
  x = timeGetTime();
  x *= 298295;
  x /= (1000 * 16384);
  return(x);
}

/* Borland C random routines */

static long BC_RandSeed = 1;

void BC_srand(short value) {
  BC_RandSeed = value;
}

short BC_rand(void) {
  BC_RandSeed = (0x015A4E35L * BC_RandSeed) + 1;
  return((BC_RandSeed >> 16) & 0x7FFF);
}

/* ======================================================================== */

short random(short __num) {
  return( (short)(((long) BC_rand() * __num) / (0x7FFFU + 1)) );
}

void delay(unsigned short __milliseconds) {
  Sleep(__milliseconds);
}

unsigned long coreleft(void) {
  return(0x7FFFFFFF);
}

void textmode(short newmode) {
  // FIXME: add more cleanup code here
  ModFreeMusic();
}

/* ======================================================================== */

void modinit(void) {
  ModInitMusic();
}

void moddevice(int *device) {
  // FIXME: not used
}

void modvolume(int vol1, int vol2, int vol3, int vol4) {
  ModLoudMusic((vol1 + vol2 + vol3 + vol4) / 4);
}

void modsetup(char *filenm, int looping, int prot, int mixspeed, int device, int *status) {
  // looping always 4 (loop infinite)
  ModPlayMusic(filenm);
}

void modstop(void) {
  ModPlayMusic(NULL);
}

/* ======================================================================== */

void kb_stub(unsigned char butt) {
}

void kb_on(void) {
  if (!kbu_func) {
    kbu_func = kb_stub;
  }
  kb_state = 1;
}

void kb_off(void) {
  kb_state = 0;
}

void kb_set(KB_UPROC funcaddr) {
  kbu_func = funcaddr;
}

/* ======================================================================== */

static JS_UPROC jsu_func = NULL;
static char js_enabled = 0;
static unsigned short js_devices = 0;

void js_stub(unsigned short butt) {
}

void js_init(void) {
JOYINFOEX jie;
int i, n;
  if (!jsu_func) {
    jsu_func = js_stub;
  }
  js_devices = 0;
  js_enabled = 0;
  n = joyGetNumDevs();
  for (i = 0; i < n; i++) {
    jie.dwSize = sizeof(jie);
    jie.dwFlags = JOY_RETURNALL;
    // get first connected joystick
    if (joyGetPosEx(i, &jie) == JOYERR_NOERROR) {
      js_devices = i + 1; // FIXME: not a bitmask
      break;
    }
  }
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
  jsu_func = funcaddr;
}

void js_read(void) {
JOYINFOEX jie;
unsigned short butt;
  /* joystick enabled */
  if (js_enabled) {
    butt = 0;
    /* joystick 1 */
    jie.dwSize = sizeof(jie);
    jie.dwFlags = JOY_RETURNALL;
    // FIXME: js_devices not a bitmask
    if (joyGetPosEx(js_devices - 1, &jie) == JOYERR_NOERROR) {
      if (jie.dwXpos < 0x5555) { butt |= JOY1_L; }
      if (jie.dwXpos > 0xAAAA) { butt |= JOY1_R; }
      if (jie.dwYpos < 0x5555) { butt |= JOY1_U; }
      if (jie.dwYpos > 0xAAAA) { butt |= JOY1_D; }
      if (jie.dwButtons & JOY_BUTTON1) { butt |= JOY1_A; }
      if (jie.dwButtons & JOY_BUTTON2) { butt |= JOY1_B; }
    }
    /* call user defined function */
    jsu_func(butt);
  }
}

void js_test(void) {
  // FIXME: not used
}
