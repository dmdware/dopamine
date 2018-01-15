










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
	char *label;
	void(*clickf)();
	float scroll;
	dbool uover;
	dbool dover;
	dbool ldownd;
	dbool ldownu;
	int ldowna;
	int oover;
};

typedef struct dlw dlw;

void dlwinit(dlw *b, wg* parent, const char* name,
	const char* label, char f, void *e,
	void(*reframef)(wg* w), void(*click)());
void dlwsz(wg *bw);
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
