#ifndef H_MOUSE
#define H_MOUSE

struct TMButton {
	int x;
	int y;
	int press;
};

class Mouse {
	char mask[64];
	int init(void);
public:
	int x;
	int y;
	int present;

	TMButton left;
	TMButton right;
	TMButton center;

	Mouse(void) {
		present=init();
	}
	~Mouse(void) {};


	void on(void);
	void off(void);
	void image(int,int,char*);
	void status(void);
	void set(int,int);
	void pressed(void);
	void released(void);
	void wait(void);
};

#endif
