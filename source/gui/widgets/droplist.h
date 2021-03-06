










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
	dbool ldownb;
	dbool bover;
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

#endif

