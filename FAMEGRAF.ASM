;TASM.EXE /mx /m2 FAMEGRAF.ASM
	IDEAL
	JUMPS
	LOCALS
	SMART
        P286N

	DOSSEG
	MODEL	LARGE
;à®æ¥¤ãàë ¨ äã­ªæ¨¨
        PUBLIC  @Vga256$qv		;ãáâ ­®¢ª  £à ä.à¥¦¨¬ 
        PUBLIC  @SetScreen$qnv	;ãáâ ­®¢ª  áªàëâ®£® íªà ­  ¢ ¯ ¬ïâ¨
        PUBLIC  @NormalScreen$qv	;ãáâ ­®¢ª  ®¡ãç­ íªà ­ 
        PUBLIC  @ScreenCopy$qnvnv     ;ª®¯¨à®¢ ­¨¥ 2å íªà ­®¢
        PUBLIC  @CopyToScreen$qnv   ;ª®¯¨à®¢ ­¨¥ ­  íªà ­ á ®¦¨¤ ­¨¥¬
        PUBLIC  @Cls$qzc	;®ç¨áâª  â¥ª íªà ­ 
        PUBLIC  @WVR$qv            ;®¦¨¤ ­¨¥ ®âà «ãç .???
        PUBLIC  @SetColor$qzc	;ãáâ.â¥ªãé¨© æ¢¥â
        PUBLIC  @PutRGB$qzczczczc ;ãáâ ­®¢¨âì ®¤¨­ æ¢¥â ¯ «¨âàë
        PUBLIC  @GetPalette$qnzc     ;¢§ïâì â¥ªãéãî ¯ «¨âàã
	PUBLIC	@GetCharWidth$qi	;è¨à¨­  á¨¬¢®« 
        PUBLIC  @PutFont$qnzc        ;ãáâ ­®¢¨âì â¥ªãé¨© èà¨äâ (8*8)
        PUBLIC  @DisplayChar$qiiizczc    ;¢ë¢¥áâ¨ á¨¬¢®« â¥ªãé¨¬ èà¨äâ®¬
        PUBLIC  @Clip$qiiii		;ãáâ ­®¢ª  ®ª­  ®¡à¥§ ­¨ï
        PUBLIC  @PutPixel$qii       ;­ à¨á®¢ âì â®çªã â¥ª.æ¢¥â®¬
        PUBLIC  @GetPixel$qii       ;áç¨â âì â®çªã á íªà ­ 
        PUBLIC  @Bar$qiiii		;¯àï¬®ã£®«ì­¨ª
;	PUBLIC  @Rectangle$qiiii	;à ¬ª 
        PUBLIC  @PutImg$qiiiinzc         ;¢ë¢®¤ ¡«®ª  ­  íªà ­
        PUBLIC  @GetImg$qiiiinzc         ;áç¨â âì ¡«®ª á íªà ­ 
        PUBLIC  @PutMas$qiiiinzc         ;¢ë¢®¤ ¡«®ª  á ¯à®§à ç­ë¬ æ¢¥â®¬ ­  íªà ­
        PUBLIC  @PutMasr$qiiiinzc        ;¢ë¢®¤ ¡«®ª  á ¯à®§à ç­ë¬ æ¢¥â®¬ ­  íªà ­ r
        PUBLIC  @PutBlink$qiiiinzc	;¢ë¢®¤ ¬¥àæ îé¥£® á¨«ãíâ 
        PUBLIC  @PutBlinkr$qiiiinzc	;¢ë¢®¤ ¬¥àæ îé¥£® á¨«ãíâ  r

        PUBLIC  @PutSImg$qiiiiiiiinzc	;¢ë¢®¤ á ®¡à¥§ª®© á¯à ©â  ­  íªà ­
        PUBLIC  @PutSMas$qiiiiiiiinzc	;¢ë¢®¤ á ®¡à¥§ª®© á¯à ©â  ­  íªà ­
        PUBLIC  @PutSMasr$qiiiiiiiinzc	;¢ë¢®¤ á ®¡à¥§ª®© á¯à ©â  ­  íªà ­
        PUBLIC  @PutSBlink$qiiiiiiiinzc	;¢ë¢®¤ á ®¡à¥§ª®© ¬¥àæ îé¥£® á¨«ãíâ 
        PUBLIC  @PutSBlinkr$qiiiiiiiinzc	;¢ë¢®¤ á ®¡à¥§ª®© ¬¥àæ îé¥£® á¨«ãíâ  r

	;PUBLIC	@CopyBlock$qiiiinviinv	;ª®¯¨à®¢ ­¨¥ ¯àï¬®ã£®«ì­®£® ¡«®ª  ¯ ¬ïâ¨
	PUBLIC	_CopyBlock	;ª®¯¨à®¢ ­¨¥ ¯àï¬®ã£®«ì­®£® ¡«®ª  ¯ ¬ïâ¨
	PUBLIC	@CopyBlock0$qnv	;ª®¯¨à®¢ ­¨¥ ¯àï¬®ã£ ¡«®ª  ¯ ¬ïâ¨ ­  íªà
        PUBLIC  @PutImg16$qiinzc	;¢ë¢®¤ ¡«®ª  16*16 ­  íªà ­
        PUBLIC  @PutMas16$qiinzc	;¢ë¢®¤ ¡«®ª  16*16 á ¯à®§à ç­ë¬ æ¢¥â®¬ ­  íªà
        PUBLIC  @CalcAdr$qii 	;¢ëç¨á«¨âì ­ ç «ì­ë©  ¤à¥á ­  íªà ­¥
        PUBLIC  @SPutImg16$qv	;¡¥§ ª®®à¤ ¢ë¢®¤ ¡«®ª  16*16 ­  íªà ­
        PUBLIC  @SPutMas16$qv	;¡¥§ ª®®à¤ ¢ë¢®¤ ¡«®ª  16*16 á ¯à®§à ç­ë¬ æ¢¥â®¬
	PUBLIC	@gettic$qv	;¢§ïâì â¨ª

;¥à¥¬¥­­ë¥
	PUBLIC	_CurrentScreen
	PUBLIC	_CurrentColor
	PUBLIC	_CurrentAdr
	PUBLIC	_CurrentAdr1
	PUBLIC	_MaxX
	PUBLIC	_MaxY
	PUBLIC	_MinX
	PUBLIC	_MinY

;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE
Copyright       db " FaMe Graph v 2.3 (c)1996 FaMe by BMV 04/03/96 16:05 "

_CurrentScreen  dd	0A0000000h
_VScreen	dd	0A0000000h ;¢á¥£¤  ¢¨§ã «ì­ë© íªà ­
_CurrentAdr	dd	0A0000000h
_CurrentAdr1	dd	(?)
_CurrentColor   db      (?)
_MaxX		dw      319
_MaxY           dw      199
_MinX           dw      0
_MinY           dw      0

_tic		dd	(?)	;â¥ªãé¨© â¨ª
	ENDS

;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE
;	void Vga256();	/* video mode 320x200x256 */

	PROC	@Vga256$qv	FAR

	;push	bp
	;mov	bp,sp
	push	ds

	mov	ax,13h
	int	10h

	pop	ds
	;pop	bp
	ret

	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

	PROC	@SetScreen$qnv	FAR	;ãáâ  ¤à¥á ¢ë¢®¤  £à ä¨ª¨ ¨ â¯
	ARG	buf:DWORD

	push	bp
	mov	bp,sp

	mov	ax,[WORD HIGH buf]	;seg
	mov	[WORD HIGH _CurrentScreen],ax
	mov	ax,[WORD LOW buf]	;offset
	mov	[WORD LOW _CurrentScreen],ax

	pop	bp

	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

	PROC	@NormalScreen$qv FAR	;ãáâ  ¤à¥á ¢ë¢®¤  £à ä¨ª¨ ¢ A000

	push	bp
	mov	bp,sp

	mov	[WORD HIGH _CurrentScreen],0A000h
	mov	[WORD LOW _CurrentScreen],0

	pop	bp

	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

        PROC    @WVR$qv	FAR     ;®¦¨¤ ­¨¥ ¢§¬. ®âà¨á®¢ª¨ «ãç 

        push    dx

        mov     dx,3DAh
@@WVR1: ;¦¤¥¬ ª®­æ  â¥ªãè¥£® ¢¥àâ¨ª «ì­®£® ¢®§¢à â  «ãç 
        in      al,dx                   ; port 3DAh, CGA/EGA vid status
        test    al,8
        jnz      @@WVR1                  ; Jump if not zero

        ;pop     dx	;!!!!!!!!!!!!!!!!
	;ret             ;!!!!!!!!!!!!!!!!
@@WVR2: ;¦¤¥¬ ­ ç «  ¢¥àâ¨ª «ì­®£® ¢®§¢à â  «ãç 
        in      al,dx                   ; port 3DAh, CGA/EGA vid status
        test    al,8
        jz     @@WVR2                  ; Jump if not zero

        pop     dx

	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

        PROC    @ScreenCopy$qnvnv      FAR    ;ª®¯¨à®¢ ­¨¥ buf1 -> buf
;ª®¯¨à®¢ ­¨¥ íªà ­  ¨§ buf1 ¢ buf

        ARG     buf:DWORD,buf1:DWORD

	push	bp
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

; ­¥®¡å®¤¨¬® ¯¥à¥á« âì ¨§ DS:SI -> ES:DI
	mov	ax,[WORD HIGH buf1]	;seg
	mov	ds,ax
	mov	si,[WORD LOW buf1]	;offset

	mov	ax,[WORD HIGH buf]	;seg
	mov	es,ax
	mov	di,[WORD LOW buf]	;offset

        cld
	mov	cx,(64000/2)
	rep	movsw

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds
	mov	sp,bp
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

        PROC    @CopyToScreen$qnv   FAR     ;ª®¯¨à®¢ ­¨¥ buf -> íªà ­
;ª®¯¨à®¢ ­¨¥ ­  íªà ­ ¨§ buf

        ARG     buf:DWORD

	push	bp
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

; ­¥®¡å®¤¨¬® ¯¥à¥á« âì ¨§ DS:SI -> ES:DI
        mov     ax,[WORD HIGH buf]     ;seg
	mov	ds,ax
        mov     si,[WORD LOW buf]      ;offset

        mov     ax,0A000h
	mov	es,ax
        mov     di,0

        cld

;-------------------------------®¦¨¤ ­¨¥ ¤«ï ¢ë¢®¤ 
        mov     dx,3DAh
@@WVR1:
        in	al,dx                   ; port 3DAh, CGA/EGA vid status
        test	al,8
        jnz	@@WVR1			; Jump if not zero
@@WVR2:
        in      al,dx			; port 3DAh, CGA/EGA vid status
        test    al,8
        jz     @@WVR2			; Jump if zero
;-----------------------------------------------------
	mov	cx,(64000/2)
	rep	movsw

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds
	mov	sp,bp
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	Cls(int n)	;®ç¨áâª  íªà ­ 

	PROC	@Cls$qzc	FAR
	ARG	n:BYTE

	push	bp
	mov	bp,sp
	push	es;!
	push	di;!

; ­¥®¡å®¤¨¬® ®ç¨áâ¨âì ES:DI
	mov	ax,[WORD HIGH _CurrentScreen]	;seg
	mov	es,ax
	mov	di,[WORD LOW _CurrentScreen]	;offset

	cld
	mov	al,[n]
	mov	ah,[n]
	mov	cx,(64000/2)
	rep	stosw

	pop	di;!
	pop	es;!
	pop	bp
	ret
	ENDP

	ENDS

;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	SetColor(int n)	;ãáâ ­®¢ª  â¥ªãé¥£® æ¢¥â 

	PROC	@SetColor$qzc	FAR
	ARG	n:WORD

	push	bp
	mov	bp,sp

	mov	al,[BYTE LOW n]
	mov	[_CurrentColor],al

	pop	bp
	ret
	ENDP

	ENDS

;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	Clip(int MinX,int MinY,int MaxX,int MaxY) ;ãáâ ­®¢ª  ®ª­  ®¡à¥§ª¨

	PROC	@Clip$qiiii	FAR
	ARG	MinX:WORD, MinY:WORD, MaxX:WORD, MaxY:WORD

	push	bp
	mov	bp,sp

	mov	ax,[MinX]
	mov	[_MinX],ax

	mov	ax,[MinY]
	mov	[_MinY],ax

	mov	ax,[MaxX]
	mov	[_MaxX],ax

	mov	ax,[MaxY]
	mov	[_MaxY],ax

	pop	bp
	ret
	ENDP

	ENDS

;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutPixel(int X,int Y) ;¢ë¢®¤ á¨¬¢®«  â¥ªãé¨¬ æ¢¥â®¬

	PROC	@PutPixel$qii	FAR
	ARG	x:WORD, y:WORD

	push	bp
	mov	bp,sp
	push	es;!
	push	ax
	push	bx

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]	; BX := byte offset in video buffer
	mov	es,[WORD HIGH _CurrentScreen]	; ES:BX := byte address of pixel

	mov     al,[_CurrentColor]
	mov	[es:bx],al

	pop	bx
	pop	ax
	pop	es;!
	pop	bp

	ret
	ENDP

	ENDS

;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	int GetPixel(int X,int Y) ;¢ë¢®¤ á¨¬¢®«  â¥ªãé¨¬ æ¢¥â®¬

	PROC	@GetPixel$qii	FAR
	ARG	x:WORD, y:WORD

	push	bp
	mov	bp,sp
	push	es;!

	mov	ax,[y]
	mov	dx,140h
	imul	dx			; dx:ax = reg * ax
	add	ax,[x]
	mov     es,[WORD HIGH _CurrentScreen]
	add     ax,[WORD LOW _CurrentScreen]
	mov	bx,ax
	mov	al,[es:bx]
	mov	ah,0
	jmp	short $+2		; delay for I/O

	pop	es;!
	pop	bp
	ret
	ENDP

	ENDS

;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	Bar(int x,int y,int xl,int yl) ; ‡ ªà è¥­­ë© ¯àï¬®ã£®«ì­¨ª
	PROC	@Bar$qiiii	FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD

	push	bp		; preserve caller registers
	mov	bp,sp
	push	es;!
	push	di;!
	push	ax
	push	bx
	push	cx

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]	; BX := byte offset in video buffer
	mov	es,[WORD HIGH _CurrentScreen]	; ES:BX := byte address of pixel
        mov     di,bx           ; ES:di ->buf

	mov	al,[_CurrentColor]	;æ¢¥â
	mov	cx,[yl]
	mov	bx,[xl]

	;cld
@@local1:
	push	cx
	mov	cx,bx
	rep	stosb		;§ ªà á¨¬ áâà®çªã...
	sub	di,bx
	add	di,320
	pop	cx
	loop	@@local1

	pop	cx
	pop	bx
	pop	ax
	pop	di;!
	pop	es;!
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutSImg(x,y,xl,yl,xr,yr,xrl,yrl,buf)
;	int x,y,xl,yl;
;	long;buf;

	PROC	@PutSImg$qiiiiiiiinzc FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, xr:WORD, yr:WORD, xrl:WORD, yrl:WORD, buf:DWORD

        push    bp
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]
	mov	es,[WORD HIGH _CurrentScreen]
        mov     di,bx           ; ES:di ->  ¤à¥á ¯¥à¢®£® ¯¨ªá¥«  ¤«ï ¢ë¢®¤ 

	mov	ax,[xl]		;¢ëç¨á«ï¥¬ á¬¥é¥­¨¥ ¯àï¬®ã£®«ì­®©
	mov	bx,[yr]		;®¡« áâ¨ ¢ á¯à-â¥
	mul	bx
	add	ax,[xr]

	add	ax,[WORD LOW buf]
	mov	si,ax
	;¯à®¢¥àª  ­  ¯¥à¥¯®«­¥­¨¥?!
        mov     ax,[WORD HIGH buf]
	mov	ds,ax

        ;mov     si,[WORD LOW buf]

	mov	cx,[yrl]	;¢ëá®â  ®¡à § 
@@local2:
	push	cx

	mov	cx,[xrl]	;è¨à¨­  ®¡à § 

	rep	movsb		;¢ë¢®¤ áâà®çª¨ ­  íªà ­

	sub	di,[xrl]	;¯¥à¥å®¤ ­  á«¥¤.áâà®ªã
	add	di,320		;íªà ­ 

	sub	si,[xrl]	;¯¥à¥å®¤ ­  á«¥¤.áâà®ªã
	add	si,[xl]         ;¢­ãâà¨ ¡®«ìè®£® á¯à-â 

	pop	cx
	loop	@@local2

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds
	mov	sp,bp
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutSMas(x,y,xl,yl,xr,yr,xrl,yrl,buf)
;	int x,y,xl,yl;
;	long;buf;

	PROC	@PutSMas$qiiiiiiiinzc FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, xr:WORD, yr:WORD, xrl:WORD, yrl:WORD, buf:DWORD

        push    bp
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]
	mov	es,[WORD HIGH _CurrentScreen]
        mov     di,bx           ; ES:di ->  ¤à¥á ¯¥à¢®£® ¯¨ªá¥«  ¤«ï ¢ë¢®¤ 
	DEC	di

	mov	ax,[xl]		;¢ëç¨á«ï¥¬ á¬¥é¥­¨¥ ¯àï¬®ã£®«ì­®©
	mov	bx,[yr]		;®¡« áâ¨ ¢ á¯à-â¥
	mul	bx
	add	ax,[xr]

	add	ax,[WORD LOW buf]
	mov	si,ax
	;¯à®¢¥àª  ­  ¯¥à¥¯®«­¥­¨¥?!
        mov     ax,[WORD HIGH buf]
	mov	ds,ax

        ;mov     si,[WORD LOW buf]

	mov	cx,[yrl]	;ãáâ ª®«-¢® ¯®¢â®à®¢ ¯® Y

@@local3:
	mov	bx,cx		;á®åà ­¨¬ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y

	mov	cx,[xrl]	;ãáâ ­®¢¨¬ ª®«-¢® ¯®¢â®à®¢ ¯® ª®®à¤ •
@@local6:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@local4	;â® ­¥ ¢ë¢®¤¨¬

	mov	[es:di],al
@@local4:
	loop	@@local6	;ª®­¥æ æ¨ª«  ¯® •

	sub	di,[xrl]	;¯¥à¥å®¤ ­  á«¥¤.áâà®ªã
	add	di,320		;íªà ­ 

	sub	si,[xrl]	;¯¥à¥å®¤ ­  á«¥¤.áâà®ªã
	add	si,[xl]         ;¢­ãâà¨ ¡®«ìè®£® á¯à-â 

	mov	cx,bx		; ¢®ááâ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y
	loop	@@local3

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds		;¢®ááâ à¥£¨áâà á¥£¬¥­â  ¤ ­­ëå
	mov	sp,bp
	pop	bp
	ret

	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutSMasr(x,y,xl,yl,xr,yr,xrl,yrl,buf)
;	int x,y,xl,yl;
;	long;buf;

	PROC	@PutSMasr$qiiiiiiiinzc FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, xr:WORD, yr:WORD, xrl:WORD, yrl:WORD, buf:DWORD

        push    bp
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x
	add	bx,[xrl]

	add	bx,[WORD LOW _CurrentScreen]
	mov	es,[WORD HIGH _CurrentScreen]
        mov     di,bx           ; ES:di ->  ¤à¥á ¯¥à¢®£® ¯¨ªá¥«  ¤«ï ¢ë¢®¤ 
	inc	di

	mov	ax,[xl]		;¢ëç¨á«ï¥¬ á¬¥é¥­¨¥ ¯àï¬®ã£®«ì­®©
	mov	bx,[yr]		;®¡« áâ¨ ¢ á¯à-â¥
	mul	bx
	add	ax,[xr]

	add	ax,[WORD LOW buf]
	mov	si,ax
	;¯à®¢¥àª  ­  ¯¥à¥¯®«­¥­¨¥?!
        mov     ax,[WORD HIGH buf]
	mov	ds,ax

        ;mov     si,[WORD LOW buf]

	mov	cx,[yrl]	;ãáâ ª®«-¢® ¯®¢â®à®¢ ¯® Y

@@local3:
	mov	bx,cx		;á®åà ­¨¬ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y

	mov	cx,[xrl]	;ãáâ ­®¢¨¬ ª®«-¢® ¯®¢â®à®¢ ¯® ª®®à¤ •
@@local6:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	dec	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@local4	;â® ­¥ ¢ë¢®¤¨¬

	mov	[es:di],al
@@local4:
	loop	@@local6	;ª®­¥æ æ¨ª«  ¯® •

	;sub	di,[xrl]	;¯¥à¥å®¤ ­  á«¥¤.áâà®ªã
	add	di,320		;íªà ­ 
	add	di,[xrl]

	sub	si,[xrl]	;¯¥à¥å®¤ ­  á«¥¤.áâà®ªã
	add	si,[xl]         ;¢­ãâà¨ ¡®«ìè®£® á¯à-â 

	mov	cx,bx		; ¢®ááâ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y
	loop	@@local3

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds		;¢®ááâ à¥£¨áâà á¥£¬¥­â  ¤ ­­ëå
	mov	sp,bp
	pop	bp
	ret

	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutSBlink(x,y,xl,yl,xr,yr,xrl,yrl,buf)
;	int x,y,xl,yl;
;	long;buf;

	PROC	@PutSBlink$qiiiiiiiinzc FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, xr:WORD, yr:WORD, xrl:WORD, yrl:WORD, buf:DWORD

        push    bp
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]
	mov	es,[WORD HIGH _CurrentScreen]
        mov     di,bx           ; ES:di ->  ¤à¥á ¯¥à¢®£® ¯¨ªá¥«  ¤«ï ¢ë¢®¤ 
	dec	di

	mov	ax,[xl]		;¢ëç¨á«ï¥¬ á¬¥é¥­¨¥ ¯àï¬®ã£®«ì­®©
	mov	bx,[yr]		;®¡« áâ¨ ¢ á¯à-â¥
	mul	bx
	add	ax,[xr]

	add	ax,[WORD LOW buf]
	mov	si,ax
	;¯à®¢¥àª  ­  ¯¥à¥¯®«­¥­¨¥?!
        mov     ax,[WORD HIGH buf]
	mov	ds,ax

        ;mov     si,[WORD LOW buf]

	mov	cx,[yrl]	;ãáâ ª®«-¢® ¯®¢â®à®¢ ¯® Y
	mov	ah,[_CurrentColor]	;§ ¯®¬­¨¬ â¥ª æ¢¥â

@@local3:
	mov	bx,cx		;á®åà ­¨¬ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y

	mov	cx,[xrl]	;ãáâ ­®¢¨¬ ª®«-¢® ¯®¢â®à®¢ ¯® ª®®à¤ •
@@local6:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@local4	;â® ­¥ ¢ë¢®¤¨¬

	mov	[es:di],ah
@@local4:
	loop	@@local6	;ª®­¥æ æ¨ª«  ¯® •

	sub	di,[xrl]	;¯¥à¥å®¤ ­  á«¥¤.áâà®ªã
	add	di,320		;íªà ­ 

	sub	si,[xrl]	;¯¥à¥å®¤ ­  á«¥¤.áâà®ªã
	add	si,[xl]         ;¢­ãâà¨ ¡®«ìè®£® á¯à-â 

	mov	cx,bx		; ¢®ááâ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y
	loop	@@local3

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds		;¢®ááâ à¥£¨áâà á¥£¬¥­â  ¤ ­­ëå
	mov	sp,bp
	pop	bp
	ret

	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutSBlinkr(x,y,xl,yl,xr,yr,xrl,yrl,buf)
;	int x,y,xl,yl;
;	long;buf;

	PROC	@PutSBlinkr$qiiiiiiiinzc FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, xr:WORD, yr:WORD, xrl:WORD, yrl:WORD, buf:DWORD

        push    bp
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x
	add	bx,[xrl]

	add	bx,[WORD LOW _CurrentScreen]
	mov	es,[WORD HIGH _CurrentScreen]
        mov     di,bx           ; ES:di ->  ¤à¥á ¯¥à¢®£® ¯¨ªá¥«  ¤«ï ¢ë¢®¤ 
	inc	di

	mov	ax,[xl]		;¢ëç¨á«ï¥¬ á¬¥é¥­¨¥ ¯àï¬®ã£®«ì­®©
	mov	bx,[yr]		;®¡« áâ¨ ¢ á¯à-â¥
	mul	bx
	add	ax,[xr]

	add	ax,[WORD LOW buf]
	mov	si,ax
	;¯à®¢¥àª  ­  ¯¥à¥¯®«­¥­¨¥?!
        mov     ax,[WORD HIGH buf]
	mov	ds,ax

        ;mov     si,[WORD LOW buf]

	mov	cx,[yrl]	;ãáâ ª®«-¢® ¯®¢â®à®¢ ¯® Y
	mov	ah,[_CurrentColor]	;§ ¯®¬­¨¬ â¥ª æ¢¥â

@@local3:
	mov	bx,cx		;á®åà ­¨¬ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y

	mov	cx,[xrl]	;ãáâ ­®¢¨¬ ª®«-¢® ¯®¢â®à®¢ ¯® ª®®à¤ •
@@local6:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	dec	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@local4	;â® ­¥ ¢ë¢®¤¨¬

	mov	[es:di],ah
@@local4:
	loop	@@local6	;ª®­¥æ æ¨ª«  ¯® •

	;sub	di,[xrl]	;¯¥à¥å®¤ ­  á«¥¤.áâà®ªã
	add	di,320		;íªà ­ 
	add	di,[xrl]

	sub	si,[xrl]	;¯¥à¥å®¤ ­  á«¥¤.áâà®ªã
	add	si,[xl]         ;¢­ãâà¨ ¡®«ìè®£® á¯à-â 

	mov	cx,bx		; ¢®ááâ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y
	loop	@@local3

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds		;¢®ááâ à¥£¨áâà á¥£¬¥­â  ¤ ­­ëå
	mov	sp,bp
	pop	bp
	ret

	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð


SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutImg(x,y,xl,yl,buf)
;	int x,y,xl,yl;
;	long;buf;

	PROC	@PutImg$qiiiinzc FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, buf:DWORD

        push    bp
	mov	bp,sp
	push	ds
	push    es;!
	push    si;!
	push    di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]	; BX := byte offset in video buffer
	mov	es,[WORD HIGH _CurrentScreen]	; ES:BX := byte address of pixel
        mov     di,bx           ; ES:di ->buf

        mov     ax,[WORD HIGH buf]
	mov	ds,ax
        mov     si,[WORD LOW buf]

	mov	cx,[yl]
@@local2:
	push	cx

	mov	cx,[xl]

	rep	movsb

	sub	di,[xl]
	add	di,320
	pop	cx
	loop	@@local2

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds
	mov	sp,bp
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð


SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutImg16(x,y,buf)
;	int x,y;
;	long;buf;

	PROC	@PutImg16$qiinzc FAR
	ARG	x:WORD, y:WORD, buf:DWORD

        push    bp
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]	; BX := byte offset in video buffer
	mov	es,[WORD HIGH _CurrentScreen]	; ES:BX := byte address of pixel
        mov     di,bx	; ES:di ->buf

        mov     ax,[WORD HIGH buf]
	mov	ds,ax
        mov     si,[WORD LOW buf]


	mov	cx,16
@@local2:
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,320-16
	loop	@@local2

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds
	mov	sp,bp
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	GetImg(x,y,xl,yl,buf)
;	int x,y,xl,yl;
;	long;buf;

	PROC	@GetImg$qiiiinzc	FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, buf:DWORD

	push	bp
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]	; BX
        mov     ds,[WORD HIGH _CurrentScreen]   ; DS:BX
        mov     si,bx           ; dS:si ->íªà ­

        mov     ax,[WORD HIGH buf]
        mov     es,ax
        mov     di,[WORD LOW buf]

; ­¥®¡å®¤¨¬® ¯¥à¥á« âì ¨§ DS:SI -> ES:DI

	mov	cx,[yl]
@@local0:
	push	cx

	mov	cx,[xl]

	rep	movsb

	sub	si,[xl]
	add	si,320
	pop	cx
	loop	@@local0

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds
	mov	sp,bp
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutMas(x,y,xl,yl,buf)
;	int x,y,xl,yl;
;	long;buf;

	PROC	@PutMas$qiiiinzc FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, buf:DWORD

	push	bp		; preserve caller registers
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]	; BX := byte offset in video buffer
	mov	es,[WORD HIGH _CurrentScreen]	; ES:BX := byte address of pixel
        mov     di,bx           ; ES:di ->buf
	dec	di

        mov     ax,[WORD HIGH buf]
	mov	ds,ax
        mov     si,[WORD LOW buf]

	mov	cx,[yl]		;ãáâ ª®«-¢® ¯®¢â®à®¢ ¯® Y

@@local3:
	mov	bx,cx		;á®åà ­¨¬ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y

	mov	cx,[xl]		;ãáâ ­®¢¨¬ ª®«-¢® ¯®¢â®à®¢ ¯® ª®®à¤ •
@@local6:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@local4		;â® ­¥ ¢ë¢®¤¨¬

	mov	[es:di],al
@@local4:
	loop	@@local6		;ª®­¥æ æ¨ª«  ¯® •
	sub	di,[xl]		;¢ëçâ¥¬ è¨à¨­ã ®¡à § 
	add	di,320		;¯à¨¡ ¢¨¬ ª®«-¢® â®ç¥ª ¢ áâà®ª¥
	mov	cx,bx		; ¢®ááâ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y
	loop	@@local3
	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds		;¢®ááâ à¥£¨áâà á¥£¬¥­â  ¤ ­­ëå
	mov	sp,bp
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutBlink(x,y,xl,yl,buf)
;	int x,y,xl,yl;
;	long;buf;

	PROC	@PutBlink$qiiiinzc FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, buf:DWORD

	push	bp		; preserve caller registers
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]	; BX := byte offset in video buffer
	mov	es,[WORD HIGH _CurrentScreen]	; ES:BX := byte address of pixel
        mov     di,bx           ; ES:di ->buf
	dec	di

        mov     ax,[WORD HIGH buf]
	mov	ds,ax
        mov     si,[WORD LOW buf]

	mov	cx,[yl]		;ãáâ ª®«-¢® ¯®¢â®à®¢ ¯® Y
	mov	ah,[_CurrentColor]	;§ ¯®¬­¨¬ â¥ª æ¢¥â

@@local3:
	mov	bx,cx		;á®åà ­¨¬ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y

	mov	cx,[xl]		;ãáâ ­®¢¨¬ ª®«-¢® ¯®¢â®à®¢ ¯® ª®®à¤ •
@@local6:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@local4	;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],ah
@@local4:
	loop	@@local6		;ª®­¥æ æ¨ª«  ¯® •
	sub	di,[xl]		;¢ëçâ¥¬ è¨à¨­ã ®¡à § 
	add	di,320		;¯à¨¡ ¢¨¬ ª®«-¢® â®ç¥ª ¢ áâà®ª¥
	mov	cx,bx		; ¢®ááâ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y
	loop	@@local3
	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds		;¢®ááâ à¥£¨áâà á¥£¬¥­â  ¤ ­­ëå
	mov	sp,bp
	pop	bp
	ret

	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutMasr(x,y,xl,yl,buf)
;	int x,y,xl,yl;
;	long;buf;

	PROC	@PutMasr$qiiiinzc FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, buf:DWORD

	push	bp		; preserve caller registers
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x
	add	bx,[xl]		; + è¨à¨­ 

	add	bx,[WORD LOW _CurrentScreen]	; BX := byte offset in video buffer
	mov	es,[WORD HIGH _CurrentScreen]	; ES:BX := byte address of pixel
        mov     di,bx           ; ES:di ->buf
	inc	di

        mov     ax,[WORD HIGH buf]
	mov	ds,ax
        mov     si,[WORD LOW buf]

	mov	cx,[yl]		;ãáâ ª®«-¢® ¯®¢â®à®¢ ¯® Y

	;cld			;íâ® á«¥¢  ­  ¯à ¢®

@@local3:
	mov	bx,cx		;á®åà ­¨¬ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y

	mov	cx,[xl]		;ãáâ ­®¢¨¬ ª®«-¢® ¯®¢â®à®¢ ¯® ª®®à¤ •
@@local6:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	dec	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@local4	;â® ­¥ ¢ë¢®¤¨¬

	mov	[es:di],al
@@local4:
	loop	@@local6		;ª®­¥æ æ¨ª«  ¯® •

	;sub	di,[xl]		;¢ëçâ¥¬ è¨à¨­ã ®¡à § 
	add	di,320		;¯à¨¡ ¢¨¬ ª®«-¢® â®ç¥ª ¢ áâà®ª¥
	add	di,[xl]

	mov	cx,bx		; ¢®ááâ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y
	loop	@@local3
	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds		;¢®ááâ à¥£¨áâà á¥£¬¥­â  ¤ ­­ëå
	mov	sp,bp
	pop	bp
	ret

	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutBlinkr(x,y,xl,yl,buf)
;	int x,y,xl,yl;
;	long;buf;

	PROC	@PutBlinkr$qiiiinzc FAR
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, buf:DWORD

	push	bp		; preserve caller registers
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x
	add	bx,[xl]		; + è¨à¨­ 

	add	bx,[WORD LOW _CurrentScreen]	; BX := byte offset in video buffer
	mov	es,[WORD HIGH _CurrentScreen]	; ES:BX := byte address of pixel
        mov     di,bx           ; ES:di ->buf
	inc	di

        mov     ax,[WORD HIGH buf]
	mov	ds,ax
        mov     si,[WORD LOW buf]

	mov	cx,[yl]		;ãáâ ª®«-¢® ¯®¢â®à®¢ ¯® Y
	mov	ah,[_CurrentColor]	;§ ¯®¬­¨¬ â¥ª æ¢¥â
	;cld			;íâ® á«¥¢  ­  ¯à ¢®

@@local3:
	mov	bx,cx		;á®åà ­¨¬ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y

	mov	cx,[xl]		;ãáâ ­®¢¨¬ ª®«-¢® ¯®¢â®à®¢ ¯® ª®®à¤ •
@@local6:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	dec	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@local4		;â® ­¥ ¢ë¢®¤¨¬

	mov	[es:di],ah
@@local4:
	loop	@@local6		;ª®­¥æ æ¨ª«  ¯® •

	;sub	di,[xl]		;¢ëçâ¥¬ è¨à¨­ã ®¡à § 
	add	di,320		;¯à¨¡ ¢¨¬ ª®«-¢® â®ç¥ª ¢ áâà®ª¥
	add	di,[xl]

	mov	cx,bx		; ¢®ááâ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y
	loop	@@local3
	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds		;¢®ááâ à¥£¨áâà á¥£¬¥­â  ¤ ­­ëå
	mov	sp,bp
	pop	bp
	ret

	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutMas16(x,y,buf)
;	int x,y;
;	long;buf;

	PROC	@PutMas16$qiinzc FAR
	ARG	x:WORD, y:WORD, buf:DWORD

	push	bp		; preserve caller registers
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]	; BX := byte offset in video buffer
	mov	es,[WORD HIGH _CurrentScreen]	; ES:BX := byte address of pixel
        mov     di,bx           ; ES:di ->buf
	dec	di

        mov     ax,[WORD HIGH buf]
	mov	ds,ax
        mov     si,[WORD LOW buf]

	mov	cx,16		;ãáâ ª®«-¢® ¯®¢â®à®¢ ¯® Y

@@local3:
	mov	bx,cx		;á®åà ­¨¬ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y

	mov	cx,16		;ãáâ ­®¢¨¬ ª®«-¢® ¯®¢â®à®¢ ¯® ª®®à¤ •
@@local6:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@local4		;â® ­¥ ¢ë¢®¤¨¬

	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬
@@local4:
	loop	@@local6		;ª®­¥æ æ¨ª«  ¯® •
	;sub	di,[xl]		;¢ëçâ¥¬ è¨à¨­ã ®¡à § 
	add	di,320-16		;¯à¨¡ ¢¨¬ ª®«-¢® â®ç¥ª ¢ áâà®ª¥
	mov	cx,bx		; ¢®ááâ ¢­¥è­ áç¥âç¨ª æ¨ª«  Y
	loop	@@local3
	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds		;¢®ááâ à¥£¨áâà á¥£¬¥­â  ¤ ­­ëå
	mov	sp,bp
	pop	bp
	ret

	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð


SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	PutRGB(n,r,g,b)

	PROC	@PutRGB$qzczczczc FAR
	ARG	n:BYTE,r:BYTE,g:BYTE,b:BYTE

	push	bp		; preserve caller registers
	mov	bp,sp

	mov	dx,3c8h
	mov	al,[n]
	out	dx,al

	mov	cx,2
@@loop1:
	loop @@loop1

	inc dx
	mov	al,[r]
	out	dx,al
	mov	al,[g]
	out	dx,al
	mov	al,[b]
	out	dx,al

	mov	sp,bp
	pop	bp
	ret
	ENDP


	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	GetPalette(buf)
;	long;buf;

	PROC	@GetPalette$qnzc FAR
	ARG	buf:DWORD

	push	bp		; preserve caller registers
	mov	bp,sp
	push	es;!

	mov	ax,[WORD HIGH buf]	;seg
	mov	es,ax
	mov	dx,[WORD LOW buf]	;offset
	mov	bx,0			;ãáâ ­®¢ N ¯¥à¢®£® à¥£¨áâà  ¯ «¨âàë
	mov	cx,256			;ª®«-¢® à¥£¨áâà®¢ ¯ «¨âàë

	mov	ax,1017h		;äã­ªæ¨ï áç¨âë¢ ­¨ï ¯ «¨âàë
	int	10h

	pop	es;!
	mov	sp,bp
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

FontOffset     dw      0               ; byte offset of (0,0)
FontSeg  dw      0C000h

;	void DisplayChar(c,x,y,fgd,bkgd);
;	int c;			/* character code */
;	int x,y;		/* upper left pixel */
;	char fgd,bkgd;		/* foreground and background*/

	PROC	@DisplayChar$qiiizczc FAR
	ARG	c:BYTE, x:WORD, y:WORD, fgd:BYTE, bkgd:BYTE

	push	bp		; preserve caller registers
	mov	bp,sp
	push	si
	push	di
	push	ds
	push	es;!

; calculate first pixel address

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]	; BX := byte offset in video buffer
	mov	es,[WORD HIGH _CurrentScreen]	; ES:BX := byte address of pixel
		mov     di,bx           ; ES:di ->buf

		mov     cx,8	; 8 - ¢ëá®â  á¨¬¢®« 

		mov     ax,[WORD FontSeg]      ;seg
		mov     ds,ax
		mov     si,[WORD FontOffset]       ;offset

	mov	al,[c]		; AL := character code
	xor	ah,ah
	mul	cl		; AX := offset into char def table
				;  (POINTS * char code)
	add	si,ax		; SI := addr of char def

; store the character in the video buffer

	mov	bl,[fgd]	; BL := foreground pixel value
	mov	bh,[bkgd]	; BH := background pixel value

	mov	cx,8
@@L102:	push	cx		; preserve CX across loop
	mov	cx,8		; CX := character width in pixels
	lodsb
	mov	ah,al		; AH := bit pattern for next pixel row

@@L112:	mov	al,bl		; AL := foreground pixel value
	shl	ah,1		; carry flag := high-order bit
	jc	@@L122		; jump if bit pattern specifies a
				;  foreground pixel (bit = 1)

	mov	al,bh		; AL := background pixel value
	cmp	bh,0h		;  ¥á«¨ ¯ãáâ®© æ¢¥â?
	jnz	@@L999
	mov	al,[es:di]	; AL := background pixel value
@@L999:

@@L122:	stosb			; update one pixel in the buffer
	loop	@@L112

	add	di,320-8 	; increment buffer address to next
				;  row of pixels
	pop	cx
	loop	@@L102		; loop down character

	pop	es;!
	pop	ds		; restore registers and return
	pop	di
	pop	si
	mov	sp,bp
	pop	bp
	ret

	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;       PutFont(buf)
;	long;buf;

        PROC    @PutFont$qnzc FAR
	ARG	buf:DWORD

	push	bp		; preserve caller registers
	mov	bp,sp

	mov	ax,[WORD HIGH buf]	;seg
        mov     [FontSeg],ax
	mov	dx,[WORD LOW buf]	;offset
        mov     [FontOffset],dx
	mov	sp,bp
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	int GetCharWidth(unsigned char c) ;è¨à¨­  á¨¬¢®« 

	PROC	@GetCharWidth$qi	FAR
	ARG	c:WORD

	push	bp
	mov	bp,sp
	push	es;!
	push	di;!

        mov     es,[FontSeg]
        mov     di,[FontOffset]
	add	di,2048
	add	di,[c]

	mov	al,[es:di]
	mov	ah,0

	pop	di;!
	pop	es;!
	pop	bp
	ret
	ENDP

	ENDS

;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	CopyBlock(x,y,xl,yl,buf,x1,y1,buf1)
;	int x,y,xl,yl;
;	long;buf;

	;PROC	@CopyBlock$qiiiinviinv FAR
	PROC	_CopyBlock
	ARG	x:WORD, y:WORD, xl:WORD, yl:WORD, buf:DWORD, x1:WORD, y1:WORD, buf1:DWORD

        push    bp
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW buf]	; BX := byte offset in video buffer
	mov	es,[WORD HIGH buf]	; ES:BX := byte address of pixel
        mov     di,bx           ; ES:di ->buf

;--------------------
	mov	ax,[y1]		; AX := y1
	mov	bx,[x1]		; BX := x1

	xchg	ah,al		; AX := 256*y1
	add	bx,ax		; BX := 256*y1 + x1
	shr	ax,1
	shr	ax,1		; AX := 64*y1
	add	bx,ax		; BX := 320*y1 + x1

	add	bx,[WORD LOW buf1]	; BX := byte offset in video buffer
	mov	ds,[WORD HIGH buf1]	; ES:BX := byte address of pixel
        mov     si,bx           ; DS:si ->buf1

	mov	cx,[yl]
@@local2:
	push	cx

	mov	cx,[xl]

	rep	movsb

	sub	di,[xl]
	add	di,320

	sub	si,[xl]
	add	si,320

	pop	cx
	loop	@@local2

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds
	mov	sp,bp
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	CopyBlock0(buf)
;	long;buf;

	PROC	@CopyBlock0$qnv FAR
	ARG	buf:DWORD

        push    bp
	mov	bp,sp
	push	ds
	push	es;!
	push	si;!
	push	di;!

	mov	di,320*16+16
	mov	es,[WORD HIGH _VScreen]
	;mov	es,0A000h	; ES:BX := byte address of pixel
        ;mov     di,bx           ; ES:di ->buf

;--------------------
	mov	bx,320*16+16
	add	bx,[WORD LOW buf]	; BX := byte offset in video buffer
	mov	ds,[WORD HIGH buf]	; ES:BX := byte address of pixel
        mov     si,bx           ; DS:si ->buf1


	mov	cx,16*10

;-------------------------------®¦¨¤ ­¨¥ ¤«ï ¢ë¢®¤ 
        mov     dx,3DAh
@@WVR1:
        in	al,dx                   ; port 3DAh, CGA/EGA vid status
        test	al,8
        jnz	@@WVR1			; Jump if not zero
@@WVR2:
        in      al,dx			; port 3DAh, CGA/EGA vid status
        test    al,8
        jz     @@WVR2			; Jump if zero
;-------------------------------------------

@@local2:
	push	cx

	mov	cx,16*16/2

	rep	movsw

	add	di,320-16*16

	add	si,320-16*16

	pop	cx
	loop	@@local2

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds
	mov	sp,bp
	pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	CalcAdr(int X,int Y) ;¢ëç¨á«  ¤à¥á 

	PROC	@CalcAdr$qii	FAR
	ARG	x:WORD, y:WORD

	push	bp
	mov	bp,sp

	mov	ax,[y]		; AX := y
	mov	bx,[x]		; BX := x

	xchg	ah,al		; AX := 256*y
	add	bx,ax		; BX := 256*y + x
	shr	ax,1
	shr	ax,1		; AX := 64*y
	add	bx,ax		; BX := 320*y + x

	add	bx,[WORD LOW _CurrentScreen]
	mov	[WORD LOW _CurrentAdr],bx
	mov	ax,[WORD HIGH _CurrentScreen]
	mov	[WORD HIGH _CurrentAdr],ax

	pop	bp

	ret
	ENDP

	ENDS

;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð
SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	SPutImg16();	¢ë¢®¤ ¯®  ¤à¥áã

	PROC	@SPutImg16$qv FAR
	;push	bp
	;mov	bp,sp
	mov	bx,ds
	push	ds
	push	es;!
	push	si;!
	push	di;!


	mov	es,[WORD HIGH _CurrentAdr]
	mov	di,[WORD LOW _CurrentAdr]

	mov	ds,[WORD HIGH _CurrentAdr1]
        mov     si,[WORD LOW _CurrentAdr1]

	mov	cx,16
@@local2:
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	movsw
	add	di,320-16
	loop	@@local2

	mov	ds,bx
	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds
	;pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	SPutMas16()

	PROC	@SPutMas16$qv FAR
	;push	bp
	;mov	bp,sp
	;mov	bx,ds
	push	ds;!
	push	es;!
	push	si;!
	push	di;!


	mov	es,[WORD HIGH _CurrentAdr]
	mov	di,[WORD LOW _CurrentAdr]
	dec	di

	mov	ds,[WORD HIGH _CurrentAdr1]
        mov     si,[WORD LOW _CurrentAdr1]

	mov	cx,16		;ãáâ ª®«-¢® ¯®¢â®à®¢ ¯® Y
@@begstr:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put01		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put01:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put02		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put02:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put03		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put03:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put04		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put04:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put05		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put05:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put06		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put06:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put07		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put07:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put08		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put08:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put09		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put09:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put10		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put10:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put11		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put11:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put12		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put12:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put13		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put13:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put14		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put14:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put15		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬

@@put15:
	lodsb			;áç¨â ¥¬ ¨§ ¡ãä¥à  â®çªã
	inc	di
	and	al,al		;¥á«¨ à ¢­  ­ã«î (¯à®§à ç­ )
	jz	@@put16		;â® ­¥ ¢ë¢®¤¨¬
	mov	[es:di],al	;¨­ ç¥ ¢ë¢®¤¨¬
@@put16:
	add	di,320-16	;¯à¨¡ ¢¨¬ ª®«-¢® â®ç¥ª ¢ áâà®ª¥
	loop	@@begstr

	pop	di;!
	pop	si;!
	pop	es;!
	pop	ds
	;mov	ds,bx
	;pop	bp
	ret

	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

SEGMENT	CODE	WORD
	ASSUME	cs:CODE

;	gettic(void)

	PROC	@gettic$qv FAR
        ;push    bp
	mov	ah,0
	int	1ah
	mov	ax,dx	;¨â®£ ¢ DX AX
	mov	dx,cx	;¨â®£ ¢ DX AX
	jmp	short $+2		; delay for I/O
	;pop	bp
	ret
	ENDP

	ENDS
;ðððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððððð

	END
