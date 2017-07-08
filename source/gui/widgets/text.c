#include "../widget.h"
#include "button.h"
#include "text.h"

void twginit(twg *tw, wg *parent, const char *n, const char *t, int f, void (*reframef)(wg *w), dbool shdw, float r, float g, float b, float a, dbool multiline)
{
	wg *bw;
	bw = &tw->base;
	wginit(bw);
	bw->parent = parent;
	bw->type = WIDGET_TEXT;
	strcpy(bw->name, n);
	tw->text = NULL;
	pstrset(&tw->text, t);
	tw->font = f;
	bw->reframefunc = reframef;
	bw->ldown = ecfalse;
	tw->rgba[0] = r;
	tw->rgba[1] = g;
	tw->rgba[2] = b;
	tw->rgba[3] = a;
	tw->shadow = shdw;
	tw->multiline = multiline;
	wgreframe(bw);
}

void twgdraw(twg *tw)
{
	wg *bw;
	float cp[4];
	glshader *s;
	
	bw = &tw->base;

	subcrop(bw->pos, bw->crop, cp);
	drawt(tw->font, bw->pos, cp, tw->text, tw->rgba, 0, -1, tw->shadow, tw->multiline);
	
	s = &g_shader[g_cursh];
	glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);
}

