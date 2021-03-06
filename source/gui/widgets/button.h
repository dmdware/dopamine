


#ifndef BUTTON_H
#define BUTTON_H

#include "../wg.h"

/* styles */
#define BUST_LINE		0
#define BUST_LEIM		1

struct bwg
{
	wg base;
	char style;
	
	float texc[4];	//texture coordinates
	float tpos[4];	//text pos
	unsigned int texi;
	unsigned int bgtex;
	unsigned int bgovtex;
	char* tooltip;
	char font;

	int param;
	float rgba[4];

	char* label;
	dbool over;
	dbool ldown;	//was the left mouse button pressed while over this (i.e. drag)?

	void (*clickf)();
	void (*clickf2)(int p);
	void (*overf)();
	void (*overf2)(int p);
	void (*outf)();
	void (*changef)();
	void (*changef2)(int p);
	void (*clickf3)(wg* w);
};

typedef struct bwg bwg;

void bwginit(bwg *b, wg* parent, 
				 const char* name, const char* filepath, const char* label, 
				 const char* tooltip, char f, char style, void (*reframef)(wg* w),
				 void (*click)(), void (*click2)(int p), void (*overf)(), 
				 void (*overf2)(int p), void (*out)(), int parm, void (*click3)(wg* w));
void bwgfree(wg* w);
void bwgdraw(wg *bw);
void bwgdrawov(wg *bw);
void bwgin(wg *bw, inev* ie);
void cenlab(bwg *b, char *label, char fi, float *pos, float *tpos, dbool hc);

#endif
