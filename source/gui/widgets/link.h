










#ifndef LINK_H
#define LINK_H

#include "../widget.h"

//hyperlink
struct hpl
{
	wg base;
	char* text;
	char font;
	void(*clickfunc)();
};

typedef struct hpl hpl;

void hplinit(hpl* hl, wg* parent, const char* n, const char* t, char f, void(*reframef)(wg* w), void(*click)());
void hpldraw(wg* bw);
void hplinev(wg* bw, inev* ie);

#endif