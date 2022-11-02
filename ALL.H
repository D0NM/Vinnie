#ifndef H_ALL
#define H_ALL

//#define EDITOR 1
#ifndef EDITOR
//!!! #define DEMO 1
#endif

#define wclock 1

#define wmyrg 18*1 //кол во мырганий мужика

#define f_def files[0]
#define f_level files[1]
#define f_blevel files[2]
#define f_bmonstr files[3]
#define f_dmonstr files[4]
#define f_models files[5]
#define f_demo files[6]
#define f_fon files[7]
#define f_bckg files[8]
#define f_dstrip files[9]

//положение меню
#define xsp 296
#define ysp 3

#define maxsiz_level (unsigned long)60*500

#define maxdemo (unsigned long)1000

//режимы
enum {s_go,s_down,s_jump,s_fly,s_lift,s_ouch,s_crash,s_over,s_end};

//модели
enum {m_lift0,m_lift,m_door,m_button,m_switch,m_keyhole,m_hole,m_brick,
m_warp,m_shop,m_touch,m_flag,m_copy,m_item};

//типы уровней (разгон и инерция и т.п.)
enum {t_normal,t_ice,t_water,t_air,t_cave};

//--опис прогр
void splitcopy0(screen dest, int sx, int sy, screen src);
void putup(void);
int yesno(char *);
void put_curr(void); //вывод тьекущих объектов редактирования
void f_quit(void); //выход в дос
void f_disk(void); //работа с диском
void f_size(void); //изменить размеры уровня
void f_clear(void); //очистка уровня
void f_game(void); //проба игры
void f_gamedemo(void);
void f_poohxy(void);
void paramlev(void);
void readlev();
void savelev();
void makepass(int);
int testpass(void);
void far mm(unsigned char); //обработчик прерываний от клавиатуры
void far mz(unsigned char); //обработчик для заставки
void far md(unsigned char); //обработчик для демонстр

void far get_joy(void); //обработчик джойстика

void f_map(void); //вывод карты уровня

void but_up(void); //сдвиг при редактировании на 1 клетку экрана
void but_down(void);
void but_left(void);
void but_right(void);

void choose_fon(void); //выбор фона
void choose_trase(void);
void choose_items(void);
void choose_monstr(void);
void choose_model(void); //установка лифтов и др. механизмов
void switch_fon(void);
void switch_trase(void);
void switch_items(void);
void switch_monstr(void);
void switch_model(void);
void show_model(int n);
void edmodel(int n);
void domodel(void);
void actmodel(void);
char check(register int x_,register int y_); //проверка на пустоту
char checkdw(register int x_,register int y_); //проверка на пустоту

void ris_lab(int,int); //вывод экрана для редакт
void rislab(unsigned int,unsigned int); //вывод экрана в игр
//!!!char check(unsigned int,unsigned int); //проверка на пустоту
//!!!char checkdw(unsigned int,unsigned int); //проверка на пустоту
void getitem(int, int); //взятие вещи по координатам

void addboom(int x, int y, int lx, int ly, char sx, char sy, char typ, block buf);
#define b_boom 0	//взрыв на месте
#define b_buh 1		//нет смены фаз
#define b_crash 2	//разлет кусков
#define b_fall 3	//опадение кусков

void addmess(char *m,unsigned char w); //!!! w=32
void doboom(); //обработка взрыврв
int addbullet(int x, int y, int sx, int sy, char typ, block buf);
void doshop(int);
void dobullet(void);
void put_score(void);
void put_additems(void);
void open_exit(void);
void doman(void); //обработка героя
void readmonstr(void); //считать описание монстров
void makemonstr(int i, int n, unsigned int x,unsigned int y);
int addmonstr(int n, unsigned int x,unsigned int y);
void doobj(void); //обработка объектов

void mult(char *); //показ мульта
void readstrip(void);
//------- структуры

//!!!
#pragma pack(push, 1)

typedef struct {
	char name[16];
	unsigned short lx,ly;	//размер уровня
	unsigned short nx,ny;	//начальн коорд героя
//!!!	unsigned int lx,ly; //размер уровня
//!!!	unsigned nx,ny;	//начальн коорд героя
	unsigned char fonR,fonG,fonB; //окраска заднего фона
	char end; //последний ли уровень?
	char bckgname[13],fonname[13]; //назв задн план, фон
	char mname0[13],mname1[13]; //назв м-ф в нач и конце уровня
	char typ; //тип уровня
	char musicname[13]; //назв музыки на уровень
} levdef;

//враги
#define maxobj 64


typedef struct { //описание объекта
	//char name[8];
	short x,y; //координаты тек кв-та
//!!!	int x,y; //координаты тек кв-та
	char sx,sy; //ускорение
	char dsx,dsy; //торможение по осям
	unsigned char lx,ly; //высота ширина
	char lfx,lfy; //коорд точек опоры
	char rtx,rty; //коорд точек опоры
	char upx,upy; //коорд точек опоры
	char dwx,dwy; //коорд точек опоры и центра
	char typ;	//тип объекта
	char n;	 //номер в массиве номеров монстров
	char s;	//состояние

	block buf; //ссылка на текущую фазу

	char f,f0,napr,myrg; //фаза скорость смены фазы и направление

	char on; //вкл/выкл 1..x, 0 жизненн сила
} object;

typedef struct {
	short x,y; //координаты тек
//!!!	int x,y; //координаты тек
	char n;	//номер типа монстра-объекта
} strkobj;

typedef struct {	//описание героя (глобальное)
	char name[8];
	short x,y; //координаты тек кв-та
//!!!	int x,y; //координаты тек кв-та
	char sx,sy; //ускорение
	char dsx1,dsx2,dsy; //торможение по осям
	unsigned char lx,ly; //высота ширина
	char lfx,lfy1,lfy2; //коорд точек опоры
	char rtx,rty1,rty2; //коорд точек опоры
	char upx1,upx2,upy; //коорд точек опоры
	char dwx1,dwx2,dwy; //коорд точек опоры и центра
	char typ;	//тип объекта
	char namespr[12]; //название спрайта
	block gospr; //ссылка на начало массива с фазами
	block jmspr;
	block stspr;
	block specspr;
	char maxgo; //мах кол-во фаз движения фаза
	char maxjm;
	char maxst;
	char maxspec;
	char s;	//состояние

	block buf; //ссылка на текущую фазу

	char f,f0,fw,napr,myrg; //фаза скорость смены фазы и направление

	char on; //вкл/выкл 1..x, 0 жизненн сила
} hero;

//типы монстров
#define maxmonstr 16

typedef struct { //описание монстров
	char name[9];
	unsigned char lx,ly; //высота, ширина
	char lfx,lfy;	//коорд точек опоры
	char rtx,rty;	//коорд точек опоры
	char upx,upy;	//коорд точек опоры
	char dwx,dwy;	//коорд точек опоры и центра
	char typ;	//тип поведения объекта
	char speed;
	char lives;
	char namespr[13]; //название файла спрайта
	block gospr; //ссылка на начало массива с фазами
	block jmspr;
	block stspr;
	block specspr;
	char maxgo; //мах кол-во фаз движения фаза
	char maxjm;
	char maxst;
	char maxspec;
	//только для монстра
	char _t_;
} monstr;

//макс кол-во лифтов и прочих моделей
#define maxmodel 48
typedef struct {
	char name[8]; //свое имя
	char name0[8]; //что включить
	short x,y; //координаты тек кв-та
//!!!	int x,y; //координаты тек кв-та
	char sx,sy; //ускорение
	unsigned char lx,ly; //высота ширина
	short x1,y1; //коорд начального положения
	short x2,y2; //коорд конечного положения
//!!!	int x1,y1; //коорд начального положения
//!!!	int x2,y2; //коорд конечного положения
	char typ;	//тип модели
	block buf; //ссылка на текущую фазу
	char on,on0; //вкл/выкл 1..x, 0 жизненн сила
} strmodels;

//магазин
typedef struct {	//описание предмета из магазина
	short price; //цена
//!!!	int price; //цена
	char name[16];
	unsigned char lx,ly; //высота ширина
	block *buf; //ссылка на фазу
	short sm;
//!!!	int sm;
	char mf;	//кол-во фаз
} sshop;

//макс кол-во взрывов
#define maxboom 32
typedef struct { //координаты взрывов
	short x,y; //координаты тек кв-та
//!!!	int x,y; //координаты тек кв-та
	unsigned char lx,ly; //высота ширина
	char sx,sy; //высота ширина
	char typ;	//тип объекта
	block buf; //ссылка на фазу
	char f; //фаза
} boom;

//пули - шишки
#define maxbullet 20
typedef struct {
	short x,y; //координаты тек кв-та
//!!!	int x,y; //координаты тек кв-та
	char sx,sy; //ускорение
	char typ;	//тип объекта
	char f;		//фаза
	block buf; //ссылка на текущую фазу
} strbullets;

//для вывод элементов в лабиринте сверху
#define maxnup (8*10)
typedef struct {
	block adr; //ссылка на адрес экрана
	block buf; //ссылка на текущую фазу
} strup;

//!!!
#pragma pack(pop)

#endif
