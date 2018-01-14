










#ifndef DROPLIST_H
#define DROPLIST_H

#include "../wg.h"

struct dlw
{
	wg base;

	float tpos[4];	//text pos
	float dpos[4];
	char font;

	char **options;
	int noptions;
	int active;
	dbool over;
	dbool ldown;	//was the left mouse button pressed while over this (i.e. drag)?
	dbool opened;
	int param;
	char *label;
	void(*clickf)();
	void(*clickf2)(int p);
	void(*clickf3)(wg* w);
	float scroll;
};

typedef struct dlw dlw;

void dlwinit(dlw *b, wg* parent, const char* name,
	const char* label, char f, int parm,
	void(*reframef)(wg* w), void(*click)(),
	void(*click2)(int p), void(*click3)(wg* w),
	dlw *prev, dlw *next);
void dlwcen(dlw *d);
void dlwadd(dlw *d, char *label);
void dlwfree(wg* w);
void dlwin(wg *bw, inev* ie);
void dlwdraw(wg *bw);
void dlwdrawov(wg *bw);

#if 0
struct DropList : public Widget
{
public:

	int mousedown[2];

	ecbool downdrop;

	DropList();
	DropList(Widget* parent, const char* n, int f, void (*reframef)(Widget* w), void (*change)());

	virtual void draw();
	virtual void drawover();
	virtual void inev(InEv* ie);

	int rowsshown();
	int square();
	void erase(int which);

	float topratio()
	{
		return scroll[1] / (float)(options.size());
	}

	float bottomratio()
	{
		return (scroll[1]+rowsshown()) / (float)(options.size());
	}

	float scrollspace();
};
#endif

#endif
