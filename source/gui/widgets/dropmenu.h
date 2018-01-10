
#ifndef DROPMENU_H
#define DROPMENU_H

#include "../wg.h"

typedef struct dwi dwi;

struct dwg
{
	wg base;

	float tpos[4];	//text pos
	char font;

	char* label;
	dbool over;
	dbool ldown;	//was the left mouse button pressed while over this (i.e. drag)?

	void(*clickf)();
	void(*clickf2)(int p);
	void(*clickf3)(wg* w);

	int param;
};

typedef struct dwg dwg;

void dwginit(dwg *b, wg* parent, const char* name,
	const char* label, char f, int parm,
	void(*reframef)(wg* w), void(*click)(), 
	void(*click2)(int p), void(*click3)(wg* w));
void dwgfree(wg* w);
void dwgin(wg *bw, inev* ie);
void dwgdraw(wg *bw);
void dwgdrawov(wg *bw);
void dsz(wg *bw);

#endif