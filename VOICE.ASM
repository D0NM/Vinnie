page 58,132
.286c
.MODEL LARGE

;**********************************************************************
;*                                                                    *
;*                             Equates                                *
;*                                                                    *
;**********************************************************************

tccount		equ	72	;reload count to produce 16572 Hz
countmax_18hz	equ	910	;ratio of new timer rate to old rate

tcaddrc		equ	43h	;timer/counter control register address
tcaddrd		equ	40h	;timer/counter data register zero

tcmode		equ	34h	;mode control byte for timer/counter

ppiaddr		equ	61h	;programmable peripheral interface address

;**********************************************************************
;*                                                                    *
;*                             Macros                                 *
;*                                                                    *
;**********************************************************************

ljz	macro	dest
	local	skip
	jnz	skip
	jmp	dest
skip:
	endm

ljb	macro	dest
	local	skip
	jnb	skip
	jmp	dest
skip:
	endm

ljbe	macro	dest
	local	skip
	jnbe	skip
	jmp	dest
skip:
	endm

;**********************************************************************
;*                                                                    *
;*                         Global variables                           *
;*                                                                    *
;**********************************************************************

.DATA
		even		;don't put these on an odd boundary

blockaddr	dd	0	;beginning of memory block
blocksize	dd	0	;size of memory block
blockend	dd	0	;last address of memory block + 1
currentaddr	dd	0	;pointer to current play position in memory
endaddr		dd	0	;address of last byte to be played + 1
datalength	dd	0	;total number of data bytes
datacount	dd	0	;number of data bytes already played
filladdr	dd	0	;address of next byte to be filled from disk
fillcount	dd	0	;number of bytes already read from disk

goflag		dw	0	;indicates whether playback is in progress
fdoneflag	dw	0	;indicates whether unread data remains in file
fileflag	dw	0	;indicates whether data is read from a file
filehandle	dw	0	;handle of file containing voice data
hookedflag	dw	0	;indicates whether the interrupt is hooked
countfor_18hz	dw	0	;counter to determine when to call BIOS

shiftcount	db	0	;current bit position within byte

;**********************************************************************
;*                                                                    *
;*                               Code                                 *
;*                                                                    *
;**********************************************************************

.CODE

bios_timer_routine	dd	0	;we keep this in the code segment
					; for access through the CS register
					; since the other segment registers
					; will contain unknown values when
					; this is needed

	assume	ds:DGROUP

;**********************************************************************
;*             Speed up the timer tick and set "goflag"               *
;**********************************************************************

startvoice	proc	near

	pushf
	cli
	mov	al,tcmode
	out	tcaddrc,al
	mov	ax,tccount
	out	tcaddrd,al
	mov	al,ah
	out	tcaddrd,al
	mov	shiftcount,0
	mov	countfor_18hz,countmax_18hz
	mov	goflag,1
	popf
	ret

startvoice	endp

;**********************************************************************
;*         Slow the timer tick to normal and clear "goflag"           *
;**********************************************************************

stopvoice	proc	near

	pushf
	cli
	mov	al,tcmode
	out	tcaddrc,al
	mov	al,0
	out	tcaddrd,al
	out	tcaddrd,al
	mov	goflag,0
	mov	fdoneflag,0
	popf
	ret

stopvoice	endp

;**********************************************************************
;*                   Interrupt service routine                        *
;**********************************************************************

timer_tick	proc	far

	push	ds
	push	bx
	push	es
	push	ax
	push	cx

	mov	ax,DGROUP
	mov	ds,ax

	cmp	goflag,0
	je	chain_exit

	les	bx,currentaddr
	mov	ah,es:[bx]
	mov	cl,shiftcount
	rol	ah,cl
	rol	ah,2
	and	ah,02h
	in	al,ppiaddr
	and	al,0FCh
	or	al,ah
	out	ppiaddr,al
	inc	cl
	and	cl,07h
	mov	shiftcount,cl
	jnz	exit_decide

	mov	ax,es
	inc	bx
	jnz	check_wrap
	add	ax,1000h
check_wrap:
	cmp	bx,word ptr blockend
	jne	check_end
	cmp	ax,word ptr blockend+2
	jne	check_end
	mov	bx,word ptr blockaddr
	mov	ax,word ptr blockaddr+2
check_end:
	cmp	bx,word ptr endaddr
	jne	save_cur_addr
	cmp	ax,word ptr endaddr+2
	jne	save_cur_addr
	call	stopvoice
	jmp	short exit_decide
save_cur_addr:
	mov	word ptr currentaddr,bx
	mov	word ptr currentaddr+2,ax
	add	word ptr datacount,1
	adc	word ptr datacount+2,0

exit_decide:
	dec	countfor_18hz
	jnz	nochain_exit
	mov	countfor_18hz,countmax_18hz

chain_exit:
	pop	cx
	pop	ax
	pop	es
	pop	bx
	pop	ds
	jmp	cs:bios_timer_routine

nochain_exit:
	mov	al,20h
	out	20h,al
	pop	cx
	pop	ax
	pop	es
	pop	bx
	pop	ds
	iret

timer_tick	endp

;**********************************************************************
;*                   Initialization procedure                         *
;**********************************************************************

;This routine should be called exactly one time before any of the other
; routines in this package are called. It takes no parameters, but returns
; a value indicating success or failure as follows:

;Return value        Meaning
;------------        -------
;     0              success
;     1              voice package already initialized
;     2              wrong CPU, won't run on 8088 or 8086

	public	PVOICE_INIT

PVOICE_INIT	proc	far

	push	sp
	pop	ax
	cmp	ax,sp
	je	vi_test
	mov	ax,2
	ret

vi_test:
	cmp	hookedflag,0
	je	vi_hook
	mov	ax,1
	ret

vi_hook:
	enter	0,0
	push	si
	push	di

	push	ds
	lea	dx,PVOICE_CBREAK
	mov	ax,seg PVOICE_CBREAK
	mov	ds,ax
	mov	ax,2523h
	int	21h
	pop	ds

	push	ds
	mov	ax,3508h
	int	21h
	mov	word ptr cs:bios_timer_routine,bx
	mov	word ptr cs:bios_timer_routine+2,es
	lea	dx,timer_tick
	mov	ax,seg timer_tick
	mov	ds,ax
	mov	ax,2508h
	int	21h
	pop	ds

	pop	di
	pop	si
	mov	hookedflag,1
	sub	ax,ax
	leave
	ret

PVOICE_INIT	endp

;**********************************************************************
;*                      Cleanup procedure                             *
;**********************************************************************

;This will restore the interrupt 8 vector to its original state. This
; routine MUST be called before the main program exits to DOS, unless:
; (a) the program has never called PVOICE_INIT, or (b) the program is
; becoming memory-resident.

;There are no parameters. The return values are 0 for success or 1 if
; the interrupt vector was not in fact hooked.

	public	PVOICE_CLEANUP

PVOICE_CLEANUP	proc	far

	cmp	hookedflag,1
	je	vc_unhook
	mov	ax,1
	ret

vc_unhook:
	enter	0,0
	push	si
	push	di
	call	stopvoice
	push	ds
	lds	dx,cs:bios_timer_routine
	mov	ax,2508h
	int	21h
	pop	ds
	pop	di
	pop	si
	mov	hookedflag,0
	sub	ax,ax
	leave
	ret

PVOICE_CLEANUP	endp

;**********************************************************************
;*                Vector restore for Control-Break                    *
;**********************************************************************

	public	PVOICE_CBREAK

PVOICE_CBREAK	proc	far

	pusha
	push	ds
	push	es

	mov	ax,DGROUP
	mov	ds,ax
	call	stopvoice
	call	PVOICE_CLEANUP

	pop	es
	pop	ds
	popa
	stc
	ret

PVOICE_CBREAK	endp

;**********************************************************************
;*                "File read catch-up" procedure                      *
;**********************************************************************

;This must be called frequently from the main program if file reading is
; being used and the length of the data to be played is longer than the
; length of the memory block. The routine checks the progress of the
; address pointer in use by the interrupt service routine and fills in
; more data from the file if necessary. When all the data has been read,
; then "endaddr" is set accordingly.

;There are no parameters. The return values are 0 for success or 1 if
; an error occurred while reading the file.

vcat_temp1l		equ	[bp-4]	;current address from ISR
vcat_temp1h		equ	[bp-2]

;the four stopping points:

vcat_temp2l		equ	[bp-8]	;data length - fill count
vcat_temp2h		equ	[bp-6]
vcat_temp3l		equ	[bp-12]	;current address - fill address
vcat_temp3h		equ	[bp-10]
vcat_temp4l		equ	[bp-16]	;block end - fill address
vcat_temp4h		equ	[bp-14]
vcat_temp5l		equ	[bp-20]	;(boundary) - fill address
vcat_temp5h		equ	[bp-18]

vcat_templength		equ	20

	public	PVOICE_CATCHUP

PVOICE_CATCHUP	proc	far

	enter	vcat_templength,0

;should we even be doing this?

	cmp	fileflag,0
	ljz	vcat_exit0
	cmp	fdoneflag,0
	ljz	vcat_exit0

;grab a stable value from currentaddr
; since it is changing all the time

	cli
	mov	ax,word ptr currentaddr
	mov	vcat_temp1l,ax
	mov	ax,word ptr currentaddr+2
	mov	vcat_temp1h,ax
	sti

;calculate the forward distance to each of the
; four possible stopping points

vcat_getlengths:
	mov	ax,word ptr datalength
	sub	ax,word ptr fillcount
	mov	vcat_temp2l,ax
	mov	ax,word ptr datalength+2
	sbb	ax,word ptr fillcount+2
	mov	vcat_temp2h,ax
	or	ax,vcat_temp2l
	jnz	vcat_getgap

	;if datalength = fillcount then all
	; data has been read

	cli
	mov	ax,word ptr filladdr
	mov	word ptr endaddr,ax
	mov	ax,word ptr filladdr+2
	mov	word ptr endaddr+2,ax
	sti
	mov	fdoneflag,0

	jmp	vcat_exit0

vcat_getgap:
	mov	ax,vcat_temp1l
	sub	ax,word ptr filladdr
	mov	vcat_temp3l,ax
	pushf
	mov	ax,vcat_temp1h
	sub	ax,word ptr filladdr+2
	sar	ax,12
	popf
	sbb	ax,0
	mov	vcat_temp3h,ax
	or	ax,vcat_temp3l
	jnz	vcat_getclearance
	mov	vcat_temp3l,0FFFFh
	mov	vcat_temp3h,0FFFFh

vcat_getclearance:
	mov	ax,word ptr blockend
	sub	ax,word ptr filladdr
	mov	vcat_temp4l,ax
	pushf
	mov	ax,word ptr blockend+2
	sub	ax,word ptr filladdr+2
	sar	ax,12
	popf
	sbb	ax,0
	mov	vcat_temp4h,ax

	mov	ax,word ptr filladdr
	neg	ax
	mov	vcat_temp5l,ax
	mov	word ptr vcat_temp5h,0
	jnz	vcat_cmp1
	inc	word ptr vcat_temp5h

;select the stopping point which will be
; encountered soonest

;Since we are not interested in the negative
; values, doing unsigned compares in a search
; for the smallest number will give the
; desired result.

vcat_cmp1:
	mov	ax,vcat_temp2h
	mov	cx,vcat_temp2l

	cmp	ax,vcat_temp3h
	jb	vcat_cmp3
	ja	vcat_cmp2
	cmp	cx,vcat_temp3l
	jb	vcat_cmp3

vcat_cmp2:
	mov	ax,vcat_temp3h
	mov	cx,vcat_temp3l


vcat_cmp3:
	cmp	ax,vcat_temp4h
	jb	vcat_cmp5
	ja	vcat_cmp4
	cmp	cx,vcat_temp4l
	jb	vcat_cmp5

vcat_cmp4:
	mov	ax,vcat_temp4h
	mov	cx,vcat_temp4l


vcat_cmp5:
	cmp	ax,vcat_temp5h
	jb	vcat_cmp7
	ja	vcat_cmp6
	cmp	cx,vcat_temp5l
	jb	vcat_cmp7

vcat_cmp6:
	mov	ax,vcat_temp5h
	mov	cx,vcat_temp5l


vcat_cmp7:

;Now the smallest number is in AX:CX.
; However, AX:CX may be 65536 or 0FFFFh
; (illegal), so we must test for these
; possibilities.

	cmp	ax,1
	je	vcat_fixcx
	cmp	cx,0FFFFh
	jne	vcat_readfile

vcat_fixcx:
	mov	cx,0FE00h
	sub	ax,ax

vcat_readfile:
	mov	bx,filehandle
	push	ds
	lds	dx,filladdr
	mov	ah,3Fh
	int	21h
	pop	ds
	jc	vcat_error1
	cmp	ax,cx
	jne	vcat_error1

;update global varibles to show the current
; situation

	add	word ptr fillcount,cx
	adc	word ptr fillcount+2,0

	add	word ptr filladdr,cx
	jnc	vcat_fillwrap
	add	word ptr filladdr+2,1000h

vcat_fillwrap:
	mov	bx,word ptr blockend
	cmp	bx,word ptr filladdr
	jne	vcat_exit0
	mov	ax,word ptr blockend+2
	cmp	ax,word ptr filladdr+2
	jne	vcat_exit0
	mov	bx,word ptr blockaddr
	mov	ax,word ptr blockaddr+2
	mov	word ptr filladdr,bx
	mov	word ptr filladdr+2,ax

	jmp	short vcat_exit0

;error exit point

vcat_error1:
	mov	ax,1
	jmp	short vcat_exit

;normal exit point

vcat_exit0:

	;don't leave until at least one bit has been played
	; or goflag becomes zero

	mov	ax,word ptr currentaddr
	cmp	vcat_temp1l,ax
	jne	vcat_exitloop
	cmp	goflag,0
	jne	vcat_exit0
vcat_exitloop:
	sub	ax,ax
vcat_exit:
	leave
	ret

PVOICE_CATCHUP	endp

;**********************************************************************
;*                   "Start playback" procedure                       *
;**********************************************************************

;Accepts the following parameters with PASCAL parameter-passing convention:

;Position   Size   Description
;--------   ----   -----------
;   1         4    A far pointer to the memory block used for voice data.
;   2         4    A dword indicating the length of the memory block.
;   3         2    A flag word. If this is 1, then data will be read from
;                   a file. If it is 0, then the data is assumed to be
;                   already present in the memory block.
;   4         2    An open file handle. This is ignored if the flag word
;                   is 0.
;   5         4    A dword indicating the number of bytes of voice data
;                   to be played. If the flag word is 0, then this number
;                   must not be greater than the length of the memory
;                   block.
;   6         4    A dowrd indicating the offset within the memory block
;                   where playback is to begin. This number must be less
;                   than the block length.

;Return value        Meaning
;------------        -------
;     0              success
;     1              data length greater than block size but no file read
;     2              block size is too small (datalength > blocksize and
;                     blocksize < 8192 and file read = yes)
;     3              unable to read from file
;     4              voice playback is already in progress
;     5              starting offset is not less than block length

min_blocksize	equ	8192

vs_parm1	equ	[bp+22]	;length = 4
vs_parm2	equ	[bp+18]	;length = 4
vs_parm3	equ	[bp+16]	;length = 2
vs_parm4	equ	[bp+14]	;length = 2
vs_parm5	equ	[bp+10]	;length = 4
vs_parm6	equ	[bp+6]	;length = 4

vs_parmlength	equ	20

	public	PVOICE_START

PVOICE_START	proc	far

	cmp	goflag,0
	je	vs_begin
	mov	ax,4
	ret	vs_parmlength

vs_begin:
	enter	0,0
	mov	word ptr datacount,0
	mov	word ptr datacount+2,0
	mov	word ptr fillcount,0
	mov	word ptr fillcount+2,0

	les	bx,dword ptr vs_parm1
	mov	word ptr blockaddr,bx
	mov	word ptr blockaddr+2,es
	les	bx,dword ptr vs_parm6
	mov	ax,es
	add	bx,word ptr blockaddr
	adc	ax,0
	shl	ax,12
	add	ax,word ptr blockaddr+2
	mov	word ptr currentaddr,bx
	mov	word ptr currentaddr+2,ax
	mov	word ptr filladdr,bx
	mov	word ptr filladdr+2,ax
	les	bx,dword ptr vs_parm2
	mov	word ptr blocksize,bx
	mov	word ptr blocksize+2,es
	mov	ax,es
	add	bx,word ptr blockaddr
	adc	ax,0
	shl	ax,12
	add	ax,word ptr blockaddr+2
	mov	word ptr blockend,bx
	mov	word ptr blockend+2,ax
	cmp	ax,word ptr currentaddr+2
	ljb	vs_error5
	ja	vs_getdatalen
	cmp	bx,word ptr currentaddr
	ljbe	vs_error5

vs_getdatalen:
	les	bx,dword ptr vs_parm5
	mov	word ptr datalength,bx
	mov	word ptr datalength+2,es
	mov	ax,vs_parm4
	mov	filehandle,ax
	mov	ax,vs_parm3
	mov	fileflag,ax

	or	ax,ax
	jz	vs_check

	mov	fdoneflag,1
	mov	word ptr endaddr,0
	mov	word ptr endaddr+2,0
	mov	ax,word ptr blocksize
	sub	ax,word ptr datalength
	mov	ax,word ptr blocksize+2
	sbb	ax,word ptr datalength+2
	jnc	vs_catch
	mov	ax,word ptr blocksize
	sub	ax,min_blocksize
	mov	ax,word ptr blocksize+2
	sbb	ax,0
	jc	vs_error2

vs_catch:
	call	PVOICE_CATCHUP
	or	ax,ax
	jnz	vs_error3
	jmp	short vs_exit0

vs_check:
	mov	ax,word ptr datalength+2
	cmp	ax,word ptr blocksize+2
	ja	vs_error1
	jb	vs_setend
	mov	ax,word ptr datalength
	cmp	ax,word ptr blocksize
	ja	vs_error1
	jb	vs_setend

	mov	ax,word ptr blockaddr
	mov	word ptr endaddr,ax
	mov	ax,word ptr blockaddr+2
	mov	word ptr endaddr+2,ax
	jmp	short vs_exit0

vs_setend:
	les	bx,dword ptr vs_parm5
	mov	ax,es
	add	bx,word ptr blockaddr
	adc	ax,0
	shl	ax,12
	add	ax,word ptr blockaddr+2
	mov	word ptr endaddr,bx
	mov	word ptr endaddr+2,ax
	jmp	short vs_exit0

vs_error1:
	mov	ax,1
	jmp	short vs_exit

vs_error2:
	mov	ax,2
	jmp	short vs_exit

vs_error3:
	mov	ax,3
	jmp	short vs_exit

vs_error5:
	mov	ax,5
	jmp	short vs_exit

vs_exit0:
	call	startvoice
	sub	ax,ax
vs_exit:
	leave
	ret	vs_parmlength

PVOICE_START	endp

;**********************************************************************
;*                   "Stop playback" procedure                        *
;**********************************************************************

;This is used when it is necessary to stop the playback before all
; data has been output to the speaker.

;This routine accepts no parameters and has no return information.

	public	PVOICE_STOP

PVOICE_STOP	proc	far

	call	stopvoice
	sub	ax,ax
	ret

PVOICE_STOP	endp

;**********************************************************************
;*                     "Get status" procedure                         *
;**********************************************************************

;This will supply certain status information about the operation of the
; voice playback routines.

;The first parameter is a far pointer to a dword which will receive the
; number of bytes which have already been output to the speaker.

;The second parameter is a far pointer to a dword which will receive the
; offset within the memory block of the byte that will be played next.

;The return value is a flag word in which the lowest two bits are
; significant:

;Bit #      Meaning if 1
;-----      ------------
;  0        playback is still in progress
;  1        unread data remains in file


vst_parm1	equ	[bp+10]	;length = 4
vst_parm2	equ	[bp+6]	;length = 4

vst_parmlength	equ	8

	public	PVOICE_STATUS

PVOICE_STATUS	proc	far

	enter	0,0

	les	bx,vst_parm1
	cli
	mov	ax,word ptr datacount
	mov	dx,word ptr datacount+2
	mov	es:[bx],ax
	mov	es:[bx+2],dx
	les	bx,vst_parm2
	mov	ax,word ptr currentaddr
	mov	dx,word ptr currentaddr+2
	sti
	shr	dx,12	;we can cheat here because we
			; know it's incremented by 1000h units
	mov	cx,word ptr blockaddr+2
	shr	cx,12
	sub	ax,word ptr blockaddr
	sbb	dx,cx
	mov	es:[bx],ax
	mov	es:[bx+2],dx

	cli
	mov	ax,fdoneflag
	shl	ax,1
	or	ax,goflag
	sti

	leave
	ret	vst_parmlength

PVOICE_STATUS	endp

	end
