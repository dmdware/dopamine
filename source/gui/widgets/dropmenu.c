

#include "dropmenu.h"
#include "button.h"
#include "../draw2d.h"
#include "../gui.h"

void dwginit(dwg *d, wg* parent, const char* name,
	const char* label, char f,
	void(*reframef)(wg* w), void(*click)(), 
	void(*click2)(int p), void(*click3)(wg* w))
{
	wg *bw;
	int length;

	bw = (wg*)d;
	wginit(bw);

	bw->parent = parent;
	bw->type = WG_DROPMENU;
	strcpy(bw->name, name);
	pstrset(&d->label, label);
	d->font = f;

	d->over = dfalse;
	d->ldown = dfalse;

	bw->reframef = reframef;
	d->clickf = click;
	d->clickf2 = click2;
	d->clickf3 = click3;
	wgreframe(bw);
	cenlab((bwg*)bw, d->label, d->font, bw->pos, d->tpos);
}

void dwgfree(wg* w)
{
	dwg *d = (dwg*)w;
	free(d->label);
}

void dwgdraw(wg *bw)
{
	dwg *d;
	glshader *s;
	float midcolor[] = { 0.7f,0.7f,0.7f,0.8f };
	float lightcolor[] = { 0.8f,0.8f,0.8f,0.8f };
	float darkcolor[] = { 0.5f,0.5f,0.5f,0.8f };
	font *f;
	char i;

	d = (dwg*)bw;
	f = g_font + d->font;

	endsh();

	usesh(SH_COLOR2D);
	s = g_shader + g_cursh;
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_currw);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_currh);

	if (d->over)
	{
		for (i = 0; i<3; ++i)
		{
			midcolor[i] = 0.8f;
			lightcolor[i] = 0.9f;
			darkcolor[i] = 0.6f;
		}
	}

	drawsq(midcolor[0], midcolor[1], midcolor[2], midcolor[3], bw->pos[0], bw->pos[1], bw->pos[2], bw->pos[3], bw->crop);

	drawl(lightcolor[0], lightcolor[1], lightcolor[2], lightcolor[3], bw->pos[2], bw->pos[1], bw->pos[2], bw->pos[3] - 1, bw->crop);
	drawl(lightcolor[0], lightcolor[1], lightcolor[2], lightcolor[3], bw->pos[0], bw->pos[1], bw->pos[2] - 1, bw->pos[1], bw->crop);

	drawl(darkcolor[0], darkcolor[1], darkcolor[2], darkcolor[3], bw->pos[0] + 1, bw->pos[3], bw->pos[2], bw->pos[3], bw->crop);
	drawl(darkcolor[0], darkcolor[1], darkcolor[2], darkcolor[3], bw->pos[2], bw->pos[1] + 1, bw->pos[2], bw->pos[3], bw->crop);

	endsh();
	CHECKGL();
	flatview(g_currw, g_currh, 1, 1, 1, 1);

	//TODO rewrite font.cpp/h to better deal with cropping
	drawt(d->font, d->tpos, bw->crop, d->label, NULL, 0, -1, dtrue, dfalse);
}

void dwgdrawov(wg *bw)
{
	dwg *d;

	d = (dwg*)bw;

	if (bw->opened)
	{

	}
}

void dsz(wg *bw)
{
}