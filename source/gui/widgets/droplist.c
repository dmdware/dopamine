




#include "droplist.h"
#include "../gui.h"

void dlwinit(dlw *d, wg* parent, const char* name,
	const char* label, char f, void *e,
	void(*reframef)(wg* w), void(*click)())
{
	wg *bw;
	int length;

	bw = (wg*)d;
	wginit(bw);

	bw->parent = parent;
	bw->type = WG_DROPLIST;
	strcpy(bw->name, name);
	pstrset(&d->label, label);
	d->font = f;
	d->options = NULL;
	d->noptions = 0;
	d->active = -1;
	d->over = dfalse;
	d->ldown = dfalse;
	bw->extra = e;
	d->opened = dfalse;
	d->clickf = click;
	d->scroll = 0.0f;
	d->dover = dfalse;
	d->uover = dfalse;
	d->oover = -1;
	d->ldownu = dfalse;
	d->ldownd = dfalse;
	d->ldowna = -1;
	d->ldownb = dfalse;
	d->bover = dfalse;
	bw->reframef = reframef;
	wgreframe(bw);
}

void dlwsz(wg *bw)
{
	dlw *d;
	d = (dlw*)bw;
	memcpy(d->tpos, bw->pos, sizeof(float) * 4);
	memcpy(d->dpos, bw->pos, sizeof(float) * 4);
	d->tpos[2] = bw->pos[0] + (bw->pos[2] - bw->pos[0]) / 2.0f;
	d->dpos[0] = d->tpos[2];
}

void dlwadd(dlw *d, char *label)
{
	if (!d->options)
		d->options = (char**)malloc(sizeof(char*));
	else
		d->options = (char**)realloc(d->options, sizeof(char*)*(d->noptions + 1));
	pstrset(&d->options[d->noptions], label);
	++d->noptions;
}

void dlwfree(wg* w)
{
	int i;
	dlw *d = (dlw*)w;
	for (i = 0; i < d->noptions; ++i)
		free(d->options[i]);
	free(d->options);
	free(d->label);
}

void dlwin(wg *bw, inev* ie)
{
	dlw *d;
	dlw *pd;
	wg *pw;
	float tp[4];
	int nops;
	float h;
	int i;
	float bp[4];

	d = (dlw*)bw;
	pw = bw->parent;
	pd = (dlw*)pw;

	if (d->opened)
	{
#define WGOPS	7
		h = d->dpos[3] - d->dpos[1];
		nops = mini(WGOPS, d->noptions);

		tp[0] = d->dpos[0];
		tp[2] = d->dpos[2] - h;

#define SQSP	2

		bp[0] = d->dpos[2] - h + SQSP;
		bp[1] = d->dpos[3] + (d->scroll / (float)d->noptions) * ((nops-1) * h);
		bp[2] = d->dpos[2] - SQSP;
		bp[3] = d->dpos[3] + ((d->scroll + nops) / (float)d->noptions) * ((nops - 1) * h);

		if (g_mouse.x >= tp[0] && g_mouse.x <= tp[2])
		{
			i = (g_mouse.y - d->dpos[3]) / h;
			if (i >= 0 && i < nops)
			{
				i += (int)d->scroll;
				goto c;
			}
		}
	}

	i = -1;

	c:

	if (ie->type == IE_MOUSEUP && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		//mousemove();

		if (d->ldownb)
		{
			d->ldownb = dfalse;
			ie->intercepted = dtrue;
			return;
		}

		if (d->ldowna == i && i >= 0)
		{
			d->active = i;
			ie->intercepted = dtrue;
			d->ldown = dfalse;
			d->over = dfalse;
			d->opened = dfalse;
			d->ldowna = -1;
			return;
		}

		d->ldowna = -1;

		if (d->over && d->ldown)
		{
			ie->intercepted = dtrue;

			return;	// intercept mouse event
		}

		d->ldown = dfalse;
		d->over = dfalse;
	}
	else if (ie->type == IE_MOUSEDOWN && ie->key == MOUSE_LEFT)
	{
		//mousemove();

		if (d->bover)
		{
			d->ldownb = dtrue;
			ie->intercepted = dtrue;
			return;
		}

		if (i >= 0 && i < d->noptions && !ie->intercepted)
		{
			d->ldowna = i;
			ie->intercepted = dtrue;
			return;
		}

		if (d->over && !ie->intercepted)
		{
			d->ldown = dtrue;
			ie->intercepted = dtrue;
			d->opened = dtrue;
			wgtop(bw);
			return;	// intercept mouse event
		}
		else if (!d->over)
		{
			d->opened = dfalse;
		}
	}
	else if (ie->type == IE_MOUSEMOVE)
	{
		if (g_mouse.x >= bw->pos[0] && g_mouse.x < bw->pos[2] && g_mouse.y >= bw->pos[1] && g_mouse.y < bw->pos[3])
		{
		}
		else
		{
			d->over = dfalse;
		}

		if (g_mouse.x >= bp[0] && g_mouse.x < bp[2] && g_mouse.y >= bp[1] && g_mouse.y < bp[3])
		{
		}
		else
		{
			d->bover = dfalse;
		}

		if (d->ldownb)
		{
			d->scroll += ie->dy * d->noptions / (float)((nops-1) * h);
			d->scroll = minf(d->noptions - nops, d->scroll);
			d->scroll = maxf(0, d->scroll);
			ie->intercepted = dtrue;
			return;
		}

		if (!ie->intercepted)
		{
			if (g_mouse.x >= bp[0] && g_mouse.x < bp[2] && g_mouse.y >= bp[1] && g_mouse.y < bp[3])
			{
				d->bover = dtrue;
				ie->intercepted = dtrue;
				return;
			}

			if (g_mouse.x >= bw->pos[0] && g_mouse.x < bw->pos[2] && g_mouse.y >= bw->pos[1] && g_mouse.y < bw->pos[3])
			{
				d->over = dtrue;
				ie->intercepted = dtrue;
				return;
			}
			if (i >= 0 && i < d->noptions)
			{
				ie->intercepted = dtrue;
				return;
			}
		}
	}
}


void dlwdraw(wg *bw)
{
	dlw *d;
	glshader *s;
	float *mc = MC;
	float *lc = LC;
	float *dc = DC;
	float *mco = MCO;
	float *lco = LCO;
	float *dco = DCO;
	float *tc[2] = { TC,TCO };
	float *ulc = lc;
	float *umc = mc;
	float *udc = dc;
	float *dlc = lc;
	float *dmc = mc;
	float *ddc = dc;
	float *cmc = mc;
	float *cdc = dc;
	float *clc = lc;
	font *f;
	char i;
	dlw *pd;
	wg *pw;
	float ua[3 * 2];
	float da[3 * 2];
	float h;
	float us[4];
	float ds[4];
	float tp[4];
	wg* gb;

	gb = (wg*)&g_gui;
	d = (dlw*)bw;
	pw = bw->parent;
	pd = (dlw*)pw;

	f = g_font + d->font;
	h = d->dpos[3] - d->dpos[1];

	ua[0 * 2 + 0] = d->dpos[2] - h + h*0.3f;
	ua[0 * 2 + 1] = d->dpos[3] - h*0.3f;
	ua[1 * 2 + 0] = d->dpos[2] - h*0.3f;
	ua[1 * 2 + 1] = d->dpos[3] - h*0.3f;
	ua[2 * 2 + 0] = d->dpos[2] - h / 2.0f;
	ua[2 * 2 + 1] = d->dpos[1] + h*0.3f;
	memcpy(da, ua, sizeof(float) * 3 * 2);
	for (i = 0; i < 3; ++i)
		da[2 * i + 1] = h - (da[2 * i + 1] - d->dpos[1]) + d->dpos[1];

	us[0] = d->dpos[2] - h + SQSP;
	us[1] = d->dpos[1] + SQSP;
	us[2] = d->dpos[2] - SQSP;
	us[3] = d->dpos[3] - SQSP;

	endsh();

	usesh(SH_COLOR2D);
	s = g_shader + g_cursh;
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_currw);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_currh);

	if (d->over)
	{
		clc = lco;
		cmc = mco;
		cdc = dco;
	}

	drawsq(cmc[0], cmc[1], cmc[2], cmc[3], us[0], us[1], us[2], us[3], bw->crop);

	drawl(cdc[0], cdc[1], cdc[2], cdc[3], d->dpos[0], d->dpos[1], d->dpos[0], d->dpos[3] - 1, bw->crop);

	drawl(cdc[0], cdc[1], cdc[2], cdc[3], d->dpos[0], d->dpos[1], d->dpos[2] - 1, d->dpos[1], bw->crop);

	drawl(clc[0], clc[1], clc[2], clc[3], d->dpos[0] + 1, d->dpos[3], d->dpos[2], d->dpos[3], bw->crop);

	drawl(clc[0], clc[1], clc[2], clc[3], d->dpos[2], d->dpos[1] + 1, d->dpos[2], d->dpos[3], bw->crop);

	if (d->opened)
	{
	}
	else
	{
		drawtri(lc[0], lc[1], lc[2], lc[3], da, bw->crop);
	}

	endsh();

#undef SQSP

	CHECKGL();
	flatview(g_currw, g_currh, 1, 1, 1, 1);

	//TODO rewrite font.cpp/h to better deal with cropping
	drawt(d->font, d->tpos, bw->crop, d->label, tc[d->over], 0, -1, dtrue, dfalse);

	if (d->active >= 0)
	{
		drawt(d->font, d->dpos, bw->crop, d->options[d->active], tc[d->over], 0, -1, dtrue, dfalse);

	}
}

void dlwdrawov(wg *bw)
{
	dlw *d;
	glshader *s;
	float *mc = MC;
	float *lc = LC;
	float *dc = DC;
	float *mco = MCO;
	float *lco = LCO;
	float *dco = DCO;
	float *tc[2] = { TC,TCO };
	float *ulc = lc;
	float *umc = mc;
	float *udc = dc;
	float *dlc = lc;
	float *dmc = mc;
	float *ddc = dc;
	float *cmc = mc;
	float *cdc = dc;
	float *clc = lc;
	float *bmc = MC;
	float *bdc = DC;
	float *blc = LC;
	font *f;
	char i;
	dlw *pd;
	wg *pw;
	float ua[3 * 2];
	float da[3 * 2];
	float h;
	int nops;
	float us[4];
	float ds[4];
	float tp[4];
	wg* gb;

	d = (dlw*)bw;

	if (!d->opened)
		return;

	gb = (wg*)&g_gui;
	pw = bw->parent;
	pd = (dlw*)pw;

	if (d->bover)
	{
		blc = LCO;
		bdc = DCO;
		dmc = MCO;
	}

	f = g_font + d->font;
	h = d->dpos[3] - d->dpos[1];

	ua[0 * 2 + 0] = d->dpos[2] - h + h*0.3f;
	ua[0 * 2 + 1] = d->dpos[3] - h*0.3f;
	ua[1 * 2 + 0] = d->dpos[2] - h*0.3f;
	ua[1 * 2 + 1] = d->dpos[3] - h*0.3f;
	ua[2 * 2 + 0] = d->dpos[2] - h / 2.0f;
	ua[2 * 2 + 1] = d->dpos[1] + h*0.3f;
	memcpy(da, ua, sizeof(float) * 3 * 2);
	for (i = 0; i < 3; ++i)
		da[2 * i + 1] = d->dpos[1] - (da[2 * i + 1] - d->dpos[1]);

#define SQSP	2

	us[0] = d->dpos[2] - h + SQSP;
	us[1] = d->dpos[1] + SQSP;
	us[2] = d->dpos[2] - SQSP;
	us[3] = d->dpos[3] - SQSP;

	nops = mini(WGOPS, d->noptions);

	endsh();

	usesh(SH_COLOR2D);
	s = g_shader + g_cursh;
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_currw);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_currh);

	tp[0] = d->dpos[0];
	tp[1] = d->dpos[3];
	tp[2] = d->dpos[2];
	tp[3] = d->dpos[3] + nops * h;

	drawsq(mc[0], mc[1], mc[2], mc[3], tp[0], tp[1], tp[2], tp[3], gb->crop);

	tp[0] = d->dpos[2] - h + SQSP;
	tp[1] = d->dpos[3];
	tp[2] = d->dpos[2] - SQSP;
	tp[3] = d->dpos[3] + (nops - 1) * h;

	drawsq(dc[0], dc[1], dc[2], dc[3], tp[0], tp[1], tp[2], tp[3], gb->crop);

	ds[0] = d->dpos[2] - h + SQSP;
	ds[1] = d->dpos[3] + h * (nops - 1) + SQSP;
	ds[2] = d->dpos[2] - SQSP;
	ds[3] = d->dpos[3] + h * nops - SQSP;

	drawsq(cmc[0], cmc[1], cmc[2], cmc[3], ds[0], ds[1], ds[2], ds[3], gb->crop);

	drawtri(lc[0], lc[1], lc[2], lc[3], ua, gb->crop);
	for (i = 0; i < 3; ++i)
		da[2 * i + 1] = da[2 * i + 1] + (nops + 1) * h;
	drawtri(lc[0], lc[1], lc[2], lc[3], da, gb->crop);

	tp[1] = d->dpos[3] + d->scroll / d->noptions * ((nops - 1) * h);
	tp[3] = d->dpos[3] + (d->scroll + (float)nops) / d->noptions * ((nops - 1) * h);
	drawsq(bmc[0], bmc[1], bmc[2], bmc[3], tp[0], tp[1], tp[2], tp[3], gb->crop);
	drawl(blc[0], blc[1], blc[2], blc[3], tp[0], tp[1], tp[0], tp[3] - 1, gb->crop);
	drawl(blc[0], blc[1], blc[2], blc[3], tp[0], tp[1], tp[2] - 1, tp[1], gb->crop);
	drawl(bdc[0], bdc[1], bdc[2], bdc[3], tp[2], tp[1] + 1, tp[2], tp[3], gb->crop);
	drawl(bdc[0], bdc[1], bdc[2], bdc[3], tp[0] + 1, tp[3], tp[2], tp[3], gb->crop);

	CHECKGL();
	flatview(g_currw, g_currh, 1, 1, 1, 1);

	//TODO rewrite font.cpp/h to better deal with cropping
	drawt(d->font, d->tpos, bw->crop, d->label, tc[d->over], 0, -1, dtrue, dfalse);

	tp[0] = d->dpos[0];
	tp[2] = d->dpos[2] - h;

	for (i = 0; i < nops; ++i)
	{
		tp[1] = d->dpos[3] + i * h;
		tp[3] = d->dpos[3] + (i + 1) * h;
		drawt(d->font, tp, gb->crop, d->options[i+(int)d->scroll], tc[0], 0, -1, dtrue, dfalse);
	}
}

