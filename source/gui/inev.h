










#ifndef IE_H
#define IE_H

#include "../sys/inc.h"
#include "../algo/bool.h"

#define IE_MOUSEMOVE		0
#define IE_MOUSEDOWN		1
#define IE_MOUSEUP		2
#define IE_KEYDOWN		3
#define IE_KEYUP			4
#define IE_CHARIN			5
#define IE_MOUSEWHEEL		6
#define IE_TEXTED			7
#define IE_TEXTIN			8
#define IE_COPY			9
#define IE_PASTE			10
#define IE_SELALL			11	//select all

#define MOUSE_LEFT			0
#define MOUSE_MIDDLE		1
#define MOUSE_RIGHT			2

struct wg;

struct inev
{
	int type;
	int x;
	int y;
	int dx;
	int dy;
	unsigned int key;
	int scancode;
	int amount;
	dbool intercepted;
	char text[32];	//UTF8
	int cursor;	//cursor pos in composition
	int sellen;	//selection length
	signed char curst;	//cursor state/type eg. drag, resize, normal
	struct wg* interceptor;	//corpd fix
};

typedef struct inev inev;

void ininit(inev* ie);

#endif
