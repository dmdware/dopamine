#ifndef TEXT_H
#define TEXT_H

#include "../wg.h"

struct twg
{
	wg base;
	dbool multiline;
	char* text;
	unsigned char font;
	float rgba[4];
	dbool shadow;
};

typedef struct twg twg;

void twginit(twg *tw, wg *parent, const char *n, const char *t, int f, void (*reframef)(wg *w), dbool shdw, float r, float g, float b, float a, dbool multiline);
void twgdraw(wg *bw);
void twgchval(twg *tw, const char *newv);

#endif
