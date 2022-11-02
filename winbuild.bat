@echo off
gcc -m32 -fno-exceptions -fno-rtti -mwindows -Wl,--strip-all -fwritable-strings -Os -Wall -pedantic modmusic/modmusic.c platform.c famegraf.c famemisc.cpp famelib.cpp pooh.cpp -o poohw.exe -l gdi32 -l kernel32 -l user32 -l winmm
gcc -m32 -fno-exceptions -fno-rtti -mwindows -Wl,--strip-all -fwritable-strings -Os -Wall -pedantic famelib.cpp setpoohw.c -o setpoohw.exe -l gdi32 -l kernel32 -l user32 -l winmm
