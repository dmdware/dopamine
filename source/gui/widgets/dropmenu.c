

#include "dropmenu.h"
#include "button.h"
#include "../draw2d.h"
#include "../gui.h"

void dwginit(dwg *d, wg* parent, const char* name,
	const char* label, char f, int parm,
	void(*reframef)(wg* w), void(*click)(), 
	void(*click2)(int p), void(*click3)(wg* w),
	dwg *prev, dwg *next)
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
	d->param = parm;
	d->opened = dfalse;
	d->prev = prev;
	d->next = next;
	wgreframe(bw);
	cenlab((dwg*)bw, d->label, d->font, bw->pos, d->tpos);
}

void dwgfree(wg* w)
{
	dwg *d = (dwg*)w;
	free(d->label);
}

void dwgclose2(wg *bw)
{
	lnode *n;
	dwg *d;
	d = (dwg*)bw;
	wg *wi;

	for(n=bw->sub.head; n; n=n->next)
	{
		wi = *(wg**)&n->data[0];
		d = (dwg*)wi;
		d->opened = dfalse;
		d->ldown = dfalse;
		d->over = dfalse;
		dwgclose2(wi);
	}
}

void dwgopen(wg *bw)
{
	dwg *d;
	lnode *n;
	wg *wi;
	dwg *di;
	d = (dwg*)bw;
	d->opened = dtrue;
	d->ldown = dtrue;

	di = d->prev;
	while (di)
	{
		dwgclose2((wg*)di);
		di->opened = dfalse;
		di->ldown = dtrue;
		di = di->prev;
	}

	di = d->next;
	while (di)
	{
		dwgclose2((wg*)di);
		di->opened = dfalse;
		di->ldown = dtrue;
		di = di->next;
	}

	for (n = bw->sub.head; n; n = n->next)
	{
		wi = *(wg**)&n->data[0];
		wgreframe(wi);

		if (wi->type == WG_DROPMENU)
		{
			d = (dwg*)wi;
			d->over = dfalse;
		}
	}

	wi = bw->parent;

	while (wi)
	{
		if (wi->type == WG_DROPMENU)
		{
			d = (dwg*)wi;
			d->opened = dtrue;
			d->ldown = dtrue;
			wi = wi->parent;
			continue;
		}
		break;
	}
}

void dwgclose(wg *bw)
{
	dwg *d;
	d = (dwg*)bw;
	
	while (bw->type == WG_DROPMENU)
	{
		d->opened = dfalse;
		d->ldown = dfalse;
		d->over = dfalse;
		bw = bw->parent;
		d = (dwg*)bw;
	}
}

void dwgin(wg *bw, inev* ie)
{
	dwg *d;
	dwg *pd;
	wg *pw;

	d = (dwg*)bw;
	pw = bw->parent;
	pd = (dwg*)pw;

	if (pw->type == WG_DROPMENU && !pd->opened)
		return;

	if (ie->type == IE_MOUSEUP && ie->key == MOUSE_LEFT)
	{
		//mousemove();

		if (d->over && (d->ldown || pw->type == WG_DROPMENU) && !ie->intercepted)
		{
			if (d->clickf != NULL)
				d->clickf();

			if (d->clickf2 != NULL)
				d->clickf2(d->param);

			if (d->clickf3 != NULL)
				d->clickf3(bw);

			dwgclose(d);

			ie->intercepted = dtrue;

			return;	// intercept mouse event
		}

		//if (d->ldown)
		{
			//if (!bw->sub.size)
			//if(!d->opened)
			{
				//ie->intercepted = dtrue;
				//dwgclose(d);
			}
			//return;
		}

		d->ldown = dfalse;
		d->opened = dfalse;
		d->over = dfalse;
	}
	else if (ie->type == IE_MOUSEDOWN && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		//mousemove();

		if (d->over)
		{
			d->ldown = dtrue;
			ie->intercepted = dtrue;
			dwgopen(bw);
			return;	// intercept mouse event
		}
		else if (!d->opened)
		{
			dwgclose(bw);
			return;
		}
	}
	else if (ie->type == IE_MOUSEMOVE)
	{
		if (g_mouse.x >= bw->pos[0] && g_mouse.x <= bw->pos[2] && g_mouse.y >= bw->pos[1] && g_mouse.y <= bw->pos[3])
		{
			if (pw->type == WG_DROPMENU || d->ldown)
			{
				dwgopen(bw);
			}
		}
		else
		{
			d->over = dfalse;
		}

		if (!ie->intercepted)
		{
			if (g_mouse.x >= bw->pos[0] && g_mouse.x <= bw->pos[2] && g_mouse.y >= bw->pos[1] && g_mouse.y <= bw->pos[3])
			{
				d->over = dtrue;
				ie->intercepted = dtrue;
				if (pw->type == WG_DROPMENU && pd->ldown)
				{
					d->opened = dtrue;
					d->ldown = dtrue;
					dwgopen(bw);
				}
				return;
			}
		}
	}
}


void dwgdraw(wg *bw)
{
	dwg *d;
	glshader *s;
	float *mc = MC;
	float *lc = LC;
	float *dc = DC;
	float *tc = TC;
	font *f;
	char i;
	dwg *pd;
	wg *pw;
	float *crop;

	d = (dwg*)bw;
	pw = bw->parent;
	pd = (dwg*)pw;

	if (pw->type == WG_DROPMENU && !pd->opened)
		return;

	if (pw->type == WG_DROPMENU)
		crop = bw->pos;
	else
		crop = bw->crop;

	f = g_font + d->font;

	endsh();

	usesh(SH_COLOR2D);
	s = g_shader + g_cursh;
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_currw);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_currh);

	if (d->over)
	{
		mc = MCO;
		lc = LCO;
		dc = DCO;
		tc = TCO;
	}

	drawsq(mc[0], mc[1], mc[2], mc[3], bw->pos[0], bw->pos[1], bw->pos[2], bw->pos[3], crop);

	drawl(lc[0], lc[1], lc[2], lc[3], bw->pos[0], bw->pos[1], bw->pos[0], bw->pos[3] - 1, crop);

	drawl(lc[0], lc[1], lc[2], lc[3], bw->pos[0], bw->pos[1], bw->pos[2] - 1, bw->pos[1], crop);

	drawl(dc[0], dc[1], dc[2], dc[3], bw->pos[0] + 1, bw->pos[3], bw->pos[2], bw->pos[3], crop);

	drawl(dc[0], dc[1], dc[2], dc[3], bw->pos[2], bw->pos[1] + 1, bw->pos[2], bw->pos[3], crop);

	endsh();

	CHECKGL();
	flatview(g_currw, g_currh, 1, 1, 1, 1);

	//TODO rewrite font.cpp/h to better deal with cropping
	drawt(d->font, d->tpos, crop, d->label, tc, 0, -1, dtrue, dfalse);
}

void dwgdrawov(wg *bw)
{
	dwg *d;

	d = (dwg*)bw;

	if (d->opened)
	{

	}
}

void dsz(wg *bw)
{
}