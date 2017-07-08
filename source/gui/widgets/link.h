










#ifndef LINK_H
#define LINK_H

#include "../widget.h"

//hyperlink
struct hpl
{
	wg base;
	char* text;
	char font;
	dbool over;
	dbool ldown;	//was the left mouse button pressed while over this (i.e. drag)?
	void(*clickf)();
	void(*freef)(wg* w);
};

typedef struct hpl hpl;

void hplinit(hpl* hl, wg* parent, const char* n, const char* t, char f, void(*reframef)(wg* w), void(*click)(), void *e, void(*freef)(wg* w));
void hpldraw(wg* bw);
void hplinev(wg* bw, inev* ie);

#endif
