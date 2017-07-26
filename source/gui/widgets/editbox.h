#ifndef EDITBOX_H
#define EDITBOX_H

#include "../wg.h"

#if 0
struct ebw
{
	wg base;
	int highl[2];	// highlighted (selected) text
	char *compos;	//composition for unicode text
	dbool multiline;
	void (*submitfunc)();
	void (*changefunc3)(unsigned int key, unsigned int scancode, dbool down, int parm);

};

typedef struct ebw ebw;

void ebwinit(ebw *eb, wg *parent, const char *n, const char *t, int f, void (*reframef)(wg *w), dbool pw, int maxl, void (*change3)(unsigned int key, unsigned int scancode, dbool down, int parm), void (*submitf)(), int parm, dbool multiline);
void ebwdraw(ebw *eb);
char *ebwdrawvalue(ebw *eb);
void ebwframeup(ebw *eb);
void ebwin(ebw *eb, inev *ie);
void ebwplacestr(ebw *eb, const char *str);
void ebwchval(ebw *eb, const char *str);
dbool ebwdelnext(ebw *eb);
dbool ebwdelprev(ebw *eb);
void ebwcopyval(ebw *eb);
void ebwpasteval(ebw *eb);
void ebwselectall(ebw *eb);
void ebwclose(ebw *eb);
void ebwgainfocus(ebw *eb);
void ebwlosefocus(ebw *eb);

#endif

#endif
