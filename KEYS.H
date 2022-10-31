#ifndef H_KEYS
#define H_KEYS

#include <dos.h>

	void interrupt NewInt9(...);
	void chkey(unsigned char butt);
extern	void far (*mainkey)(unsigned char);

class Keys {

public:
	unsigned char butt;
	void interrupt (*oldfunc)(...);
	void interrupt (*newfunc)(...);

	Keys(void) {
		oldfunc=getvect(0x9);
		mainkey=chkey;
		//mainkey=prog;
		setvect(0x09, newfunc=NewInt9); /* уст вектор прер */
	}

	Keys(void far (*prog)(unsigned char)) {
		oldfunc=getvect(0x9);
		mainkey=prog;
		setvect(0x09, newfunc=NewInt9); /* уст вектор прер */
	}

	~Keys(void) {
		setvect(0x09, oldfunc); /* уст первый вектор прер */
	}

	//void init(void far (*prog)(char));
	void on(void);
	void off(void);
	void set(void far (*prog)(unsigned char));
};

#endif
