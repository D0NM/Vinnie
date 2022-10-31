#include <dos.h>
#include "famegraf.h"
#include "ems.h"

int EmsError=-1;	//ошибка-сост EMS дпайвера
int EmsPresent=0;	//ошибка-сост EMS дпайвера

int EmsInstalled(void) {
	//есть ли EMS 4.0?
	static char *EmmName="EMMXXXX0";
	block EmmPtr;
	_AX=0x3567;
	geninterrupt(0x21);
	EmmPtr=(block)MK_FP(_ES,0x0a);
	while( *EmmName ) {
		if( *EmmName++ != *EmmPtr++ )
			return (EmsError=NOTINSTALLED);
	}
	EmsPresent=1;
	return (EmsError=NOERROR);
}

int EmsVersion(void) {
	//опред N версии EMS
	if( EmsPresent==0 ) {
		return 0;
	}
	_AH=0x46;
	geninterrupt(0x67);
	if( _AH ) {
		EmsError=_AH;
		return 0;
	}
	EmsError=0;
	return _AL;
}

int EmsPagesAvail(void) {
	//выдает кол-во свободных логич страниц
	if( EmsPresent==0 ) {
		return -1;
	}
	_AH=0x42;
	geninterrupt(0x67);
	if( _AH ) {
		EmsError=_AH;
		return -1;
	}
	EmsError=0;
	return _BX;
}

int EmsAlloc(int Pages) {
	//восст map тек handle во вн. обл EMS
	if( EmsPresent==0 ) {
		return -1;
	}
	_BX=Pages;
	_AH=0x43;
	geninterrupt(0x67);
	if( _AH ) {
		EmsError=_AH;
		return -1;
	}
	EmsError=0;
	return (_DX);
}

block EmsFrameAdr(void) {
	//восст map тек handle во вн. обл EMS
	if( EmsPresent==0 ) {
		return NULL;
	}
	_AH=0x41;
	geninterrupt(0x67);
	if( _AH ) {
		EmsError=_AH;
		return (block)0;
	}
	EmsError=0;
	return (block)MK_FP(_BX,0);
}


int EmsMap(int Handle, int Pages[]) {
	//отображ логич страниц с n из pages[]
	register int i;
	if( EmsPresent==0 ) {
		return -1;
	}
	//постр карты отображ для 4х страниц
	for( i=0; i<4; ++i ) {
		_BX=Pages[i];
		_DX=Handle;
		_AH=0x44;
		_AL=(char)i;
		geninterrupt(0x67);
		if( _AH ) {
			EmsError=_AH;
			return -1;
		}
	}
	EmsError=0;
	return 0;
}

int EmsFree(int Handle) {
	//освободить логич стран. за handle
	if( EmsPresent==0 ) {
		return -1;
	}
	_DX=Handle;
	_AH=0x45;
	geninterrupt(0x67);
	if( _AH ) {
		EmsError=_AH;
		return -1;
	}
	EmsError=0;
	return 0;
}

int EmsSave(int Handle) {
	//сохр тек map тек handle во вн. обл EMS
	if( EmsPresent==0 ) {
		return -1;
	}
	_DX=Handle;
	_AH=0x47;
	geninterrupt(0x67);
	if( _AH ) {
		EmsError=_AH;
		return -1;
	}
	EmsError=0;
	return 0;

}

int EmsRestore(int Handle) {
	//восст map тек handle во вн. обл EMS
	if( EmsPresent==0 ) {
		return -1;
	}
	_DX=Handle;
	_AH=0x48;
	geninterrupt(0x67);
	if( _AH ) {
		EmsError=_AH;
		return -1;
	}
	EmsError=0;
	return 0;

}

char *	EMMerror[]=
{
	"Internal error in EMM driver",
	"EMS memory board hardware malfunction",
	"EMM is busy",
	"Invalid handle",
	"Undefined function requested",
	"No more EMM handles available",
	"Error in save or restore of mapping context",
	"Allocation request was larger than total EMS memory",
	"Allocation request was larger than available pages",
	"You can't open a handle with a page count of 0",
	"Your handle doesn't own that many pages",
	"Illegal mapping.  Only physical pages 0-3 are defined",
	"Mapping context save area is full",
	"You can save context only once per handle",
	"You can't restore context without a prior save",
	"Subfunction parameter in AL not defined",
	/* EMS 4.0 new error codes */
	"Attribute type undefined",
	"Non-volatility not supported",
	"Source and dest overlap in EMS RAM (warning, not error)",
	"Destination area in handle too small",
	"Conventional memory overlaps EMS RAM",
	"Offset too large in block move",
	"Block size too large; ie, > 1M",
	"Source and dest are in same handle and overlap",
	"Source or dest memory type invalid (in offset 4 of packet)",
	"Alternate map register set in not supported",
	"All alternate map/DMA register sets are all allocated",
	"All alternate map/DMA register sets are not supported",
	"Specified alternate map/DMA register set invalid or in use",
	"Dedicated DMA channels are not supported",
	"Specified DMA channel is not supported",
	"No handle matches specified name",
	"The specified name already exists",
	"Source offset+region length > 1M (attempted to wrap)",
	"Contents of specified data packet are corrupted or invalid",
	"Access to this function has been denied (bad access key)"
};

char *getEMMerror(int errnum){
	if(!errnum) return "Нет ошибок";
	if(errnum<0x80 || errnum>0xA4) return "Неизвестная ошибка";
	return EMMerror[errnum-0x80];
}
