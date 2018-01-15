




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

	if (ie->type == INEV_MOUSEUP && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		//mousemove();

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
	else if (ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_LEFT)
	{
		//mousemove();

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
	else if (ie->type == INEV_MOUSEMOVE)
	{
		if (g_mouse.x >= bw->pos[0] && g_mouse.x <= bw->pos[2] && g_mouse.y >= bw->pos[1] && g_mouse.y <= bw->pos[3])
		{
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

#define SQSP	2

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
	drawsq(mc[0], mc[1], mc[2], mc[3], tp[0], tp[1], tp[2], tp[3], gb->crop);
	drawl(lc[0], lc[1], lc[2], lc[3], tp[0], tp[1], tp[0], tp[3] - 1, gb->crop);
	drawl(lc[0], lc[1], lc[2], lc[3], tp[0], tp[1], tp[2] - 1, tp[1], gb->crop);
	drawl(dc[0], dc[1], dc[2], dc[3], tp[2], tp[1] + 1, tp[2], tp[3], gb->crop);
	drawl(dc[0], dc[1], dc[2], dc[3], tp[0] + 1, tp[3], tp[2], tp[3], gb->crop);

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
		drawt(d->font, tp, gb->crop, d->options[i], tc[0], 0, -1, dtrue, dfalse);
	}
}


#if 0
#include "../widget.h"
#include "barbutton.h"
#include "button.h"
#include "checkbox.h"
#include "editbox.h"
#include "droplist.h"
#include "image.h"
#include "insdraw.h"
#include "link.h"
#include "listbox.h"
#include "text.h"
#include "textarea.h"
#include "textblock.h"
#include "touchlistener.h"
#include "../gui.h"

DropList::DropList() : Widget()
{
	parent = NULL;
	type = WIDGET_DROPLIST;
	name = "";
	font = MAINFONT8;
	opened = ecfalse;
	selected = 0;
	scroll[1] = 0;
	mousescroll = ecfalse;
	ldown = ecfalse;
	changefunc = NULL;
	reframefunc = NULL;
	CreateTex(frametex, "gui/frame.jpg", ectrue, ecfalse);
	CreateTex(filledtex, "gui/filled.jpg", ectrue, ecfalse);
	CreateTex(uptex, "gui/up.jpg", ectrue, ecfalse);
	//CreateTex(downtex, "gui/down.jpg", ectrue, ecfalse);
	reframe();
}

DropList::DropList(Widget* parent, const char* n, int f, void(*reframef)(Widget* w), void(*change)()) : Widget()
{
	parent = parent;
	type = WIDGET_DROPLIST;
	name = n;
	font = f;
	opened = ecfalse;
	selected = 0;
	scroll[1] = 0;
	mousescroll = ecfalse;
	ldown = ecfalse;
	changefunc = change;
	reframefunc = reframef;
	CreateTex(frametex, "gui/frame.jpg", ectrue, ecfalse);
	CreateTex(filledtex, "gui/filled.jpg", ectrue, ecfalse);
	CreateTex(uptex, "gui/up.jpg", ectrue, ecfalse);
	//CreateTex(downtex, "gui/down.jpg", ectrue, ecfalse);
	reframe();
}

void DropList::erase(int which)
{
	options.erase(options.begin() + which);
	if (selected == which)
		selected = -1;

	if (scroll[1] + rowsshown() > options.size())
		scroll[1] = options.size() - (float)rowsshown();

	if (scroll[1] < 0)
		scroll[1] = 0;
}

int DropList::rowsshown()
{
	int rows = MAX_OPTIONS_SHOWN;

	if (rows > options.size())
		rows = options.size();

	return rows;
}

int DropList::square()
{
	return (int)g_font[font].gheight;
}

float DropList::scrollspace()
{
	return g_font[font].gheight*(rowsshown()) - square();
}

//TODO draw using lines

void DropList::draw()
{
	//glColor4f(1, 1, 1, 1);
	glUniform4f(g_sh[SHADER_ORTHO].slot[SSLOT_COLOR], 1, 1, 1, 1);

	DrawImage(g_texture[frametex].texname, pos[0], pos[1], pos[2], pos[3], 0, 0, 1, 1, crop);

	if (!opened)
		DrawImage(g_texture[uptex].texname, pos[2] - square(), pos[1], pos[2], pos[1] + square(), 0, 1, 1, 0, crop);

	if (options.size() <= 0)
		return;

	if (selected < 0)
	{
		//if(options.size() <= 0)
		//	return;

		//DrawShadowedText(font, pos[0]+3, pos[1], options[0].c_str());

		return;
	}

	if (selected >= (int)options.size())
		return;

	DrawShadowedTextF(font, pos[0] + 3, pos[1], crop[0], crop[1], crop[2], crop[3], &options[selected], NULL, -1);
}

void DropList::drawover()
{
	if (!opened)
		return;

	//glColor4f(1, 1, 1, 1);
	glUniform4f(g_sh[SHADER_ORTHO].slot[SSLOT_COLOR], 1, 1, 1, 1);

	Font* f = &g_font[font];

	float drop[4];
	float dropsign;
	int dropedge;

	float drfy[2];	//drop frame box y
	float scary[2];	//scroll area y
	float bary[2];
	float upy[2];
	float downy[2];

	//if(opened)
	{
		if (downdrop)
		{
			drop[0] = pos[0];
			drop[1] = pos[1];
			drop[2] = pos[2];
			drop[3] = pos[3] + rowsshown() * f->gheight;
			dropsign = 1;
			dropedge = 3;

			drfy[0] = drop[1] + f->gheight;
			drfy[1] = drop[3];
			scary[0] = drop[1] + f->gheight;
			scary[1] = drop[3];
			upy[0] = drop[1];
			upy[1] = drop[1] + square();
			downy[0] = drop[3] - square();
			downy[1] = drop[3];
			bary[0] = drop[1] + f->gheight + scrollspace()*topratio();
			bary[1] = drop[1] + f->gheight + scrollspace()*bottomratio();
		}
		else
		{
			drop[0] = pos[0];
			drop[1] = pos[1] - rowsshown() * f->gheight;
			drop[2] = pos[2];
			drop[3] = pos[3];
			dropsign = -1;
			dropedge = 1;

			drfy[0] = drop[1];
			drfy[1] = drop[3] - f->gheight;
			scary[0] = drop[1] + f->gheight;
			scary[1] = drop[3];
			upy[0] = drop[1];
			upy[1] = drop[1] + square();
			downy[0] = drop[3] - square();
			downy[1] = drop[3];
			bary[0] = drop[1] + f->gheight + scrollspace()*topratio();
			bary[1] = drop[1] + f->gheight + scrollspace()*bottomratio();
		}
	}

	//downdrop = ectrue;

	//if(downdrop)
	{
		//dropdown frame box
		DrawImage(g_texture[frametex].texname,
			pos[0], drfy[0], pos[2], drfy[1],
			0, 0, 1, 1, g_gui.crop);

		//scroll area box
		DrawImage(g_texture[frametex].texname,
			pos[2] - square(), scary[0], pos[2], scary[1],
			0, 0, 1, 1, g_gui.crop);

		//up scroll button
		DrawImage(g_texture[uptex].texname,
			pos[2] - square(), upy[0], pos[2], upy[1],
			0, 0, 1, 1, g_gui.crop);

		//down scroll button
		DrawImage(g_texture[uptex].texname,
			pos[2] - square(), downy[0], pos[2], downy[1],
			0, 1, 1, 0, g_gui.crop);

#if 1
		//scroll bar
		DrawImage(g_texture[filledtex].texname,
			pos[2] - square(), bary[0], pos[2], bary[1],
			0, 0, 1, 1, g_gui.crop);

		for (int i = (int)scroll[1]; i < (int)scroll[1] + rowsshown(); i++)
			//DrawShadowedText(font, pos[0]+3, pos[3]+f->gheight*(i-(int)scroll[1]), &options[i]);
			DrawShadowedTextF(font, pos[0] + 3, drfy[0] + f->gheight*(i - (int)scroll[1]), pos[0], drop[1], pos[2], drop[3], &options[i]);
#endif
	}
}

void DropList::inev(InEv* ie)
{
	Font* f = &g_font[font];

	float drop[4];
	float dropsign;
	int dropedge;

	float drfy[2];	//drop frame box y
	float scary[2];	//scroll area y
	float bary[2];
	float upy[2];
	float downy[2];

	if (opened)
	{
		if (downdrop)
		{
			drop[0] = pos[0];
			drop[1] = pos[1];
			drop[2] = pos[2];
			drop[3] = pos[3] + rowsshown() * f->gheight;
			dropsign = 1;
			dropedge = 3;

			drfy[0] = drop[1] + f->gheight;
			drfy[1] = drop[3];
			scary[0] = drop[1] + f->gheight;
			scary[1] = drop[3];
			upy[0] = drop[1];
			upy[1] = drop[1] + square();
			downy[0] = drop[3] - square();
			downy[1] = drop[3];
			bary[0] = drop[1] + f->gheight + scrollspace()*topratio();
			bary[1] = drop[1] + f->gheight + scrollspace()*bottomratio();
		}
		else
		{
			drop[0] = pos[0];
			drop[1] = pos[1] - rowsshown() * f->gheight;
			drop[2] = pos[2];
			drop[3] = pos[3];
			dropsign = -1;
			dropedge = 1;

			drfy[0] = drop[1];
			drfy[1] = drop[3] - f->gheight;
			scary[0] = drop[1] + f->gheight;
			scary[1] = drop[3];
			upy[0] = drop[1];
			upy[1] = drop[1] + square();
			downy[0] = drop[3] - square();
			downy[1] = drop[3];
			bary[0] = drop[1] + f->gheight + scrollspace()*topratio();
			bary[1] = drop[1] + f->gheight + scrollspace()*bottomratio();
		}
	}

	if (ie->type == INEV_MOUSEWHEEL && !ie->intercepted)
	{
		if (opened)
		{
			ie->intercepted = ectrue;
			return;	// intercept mouse event
		}
	}
	// corpd fix
	else if (ie->type == INEV_MOUSEMOVE && (!ie->intercepted || mousescroll))
	{
		if (g_mouse.x >= pos[0] && g_mouse.x <= pos[2] && g_mouse.y >= pos[1] && g_mouse.y <= pos[3])
		{
			g_mouseoveraction = ectrue;
			//windows/msvs2012 still allowed droplist drop down even when the following line wasn't there wtf
			//but mac didn't
			over = ectrue;
		}

#if 0
		if (opened)
		{
			Font* f = &g_font[font];

			//on dropped list?
			if (g_mouse.x >= pos[0] && g_mouse.x <= pos[2] && g_mouse.y >= pos[1] && g_mouse.y <= pos[1] + f->gheight*rowsshown())
			{
				g_mouseoveraction = ectrue;
				over = ectrue;
				ie->intercepted = ectrue;
			}
		}
#endif

		//corpd fix corpc fix
		if (opened)
		{
			for (int i = (int)scroll[1]; i < (int)scroll[1] + rowsshown(); i++)
			{
				// std::list item?
				if (g_mouse.x >= pos[0] &&
					g_mouse.x <= pos[2] - square() &&
					g_mouse.y >= drfy[0] + f->gheight*(i - (int)scroll[1]) &&
					g_mouse.y <= drfy[0] + f->gheight*(i - (int)scroll[1] + 1))
				{
					ie->intercepted = ectrue;
				}
			}

			// scroll bar?
			if (g_mouse.x >= pos[2] - square() &&
				g_mouse.y >= bary[0] &&
				g_mouse.x <= pos[2] &&
				g_mouse.y <= bary[1])
			{
				ie->intercepted = ectrue;
			}

			// up button?
			if (g_mouse.x >= pos[2] - square() &&
				g_mouse.y >= upy[0] &&
				g_mouse.x <= pos[2] &&
				g_mouse.y <= upy[1])
			{
				ie->intercepted = ectrue;
			}

			// down button?
			if (g_mouse.x >= pos[2] - square() &&
				g_mouse.y >= downy[0] &&
				g_mouse.x <= pos[2] &&
				g_mouse.y <= downy[1])
			{
				ie->intercepted = ectrue;
			}
		}
		if (g_mouse.x >= pos[0] && g_mouse.y >= pos[1] && g_mouse.x <= pos[2] && g_mouse.y <= pos[3])
		{
			ie->intercepted = ectrue;
		}

		if (ldown)
			ie->intercepted = ectrue;

		if (!mousescroll)
			return;

		g_mouseoveraction = ectrue;

		int dy = g_mouse.y - mousedown[1];
		float topy = scary[dropedge / 2] + square() + scrollspace()*topratio();
		float newtopy = topy + dy;

		//topratio = (float)scroll / (float)(options.size());
		//topy = pos[3]+square+scrollspace*topratio
		//topy = pos[3]+square+scrollspace*((float)scroll / (float)(options.size()))
		//topy - pos[3] - square = scrollspace*(float)scroll / (float)(options.size())
		//(topy - pos[3] - square)*(float)(options.size())/scrollspace = scroll

		scroll[1] = (newtopy - scary[dropedge / 2] - square())*(float)(options.size()) / scrollspace();

		if (scroll[1] < 0)
		{
			scroll[1] = 0;
			ie->intercepted = ectrue;
			return;
		}
		else if (scroll[1] + rowsshown() > options.size())
		{
			scroll[1] = options.size() - (float)rowsshown();
			ie->intercepted = ectrue;
			return;
		}

		mousedown[1] = g_mouse.y;
		ie->intercepted = ectrue;
	}
	else if (ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_LEFT)
	{
		//InfoMess("dlld", "dlld");

#if 0
		if (over)
		{
			ldown = ectrue;
			ie->intercepted = ectrue;
		}
#endif

		if (opened)
		{
			for (int i = (int)scroll[1]; i < (int)scroll[1] + rowsshown(); i++)
			{
				// std::list item?
				if (g_mouse.x >= pos[0] &&
					g_mouse.x <= pos[2] - square() &&
					g_mouse.y >= drfy[0] + f->gheight*(i - (int)scroll[1]) &&
					g_mouse.y <= drfy[0] + f->gheight*(i - (int)scroll[1] + 1))
				{
					ldown = ectrue;
					ie->intercepted = ectrue;
					return;	// intercept mouse event
				}
			}

			// scroll bar?
			if (g_mouse.x >= pos[2] - square() &&
				g_mouse.y >= bary[0] &&
				g_mouse.x <= pos[2] &&
				g_mouse.y <= bary[1])
			{
				ldown = ectrue;
				mousescroll = ectrue;
				mousedown[1] = g_mouse.y;
				ie->intercepted = ectrue;
				return;	// intercept mouse event
			}

			// up button?
			if (g_mouse.x >= pos[2] - square() &&
				g_mouse.y >= upy[0] &&
				g_mouse.x <= pos[2] &&
				g_mouse.y <= upy[1])
			{
				ldown = ectrue;
				ie->intercepted = ectrue;
				return;
			}

			// down button?
			if (g_mouse.x >= pos[2] - square() &&
				g_mouse.y >= downy[0] &&
				g_mouse.x <= pos[2] &&
				g_mouse.y <= downy[1])
			{
				ldown = ectrue;
				ie->intercepted = ectrue;
				return;
			}

			ie->intercepted = ectrue;
		}

		if (!ie->intercepted)
		{
			if (g_mouse.x >= pos[0] && g_mouse.y >= pos[1] && g_mouse.x <= pos[2] && g_mouse.y <= pos[3])
			{
				ldown = ectrue;
				ie->intercepted = ectrue;
				return;
			}
		}

		//corpd fix xp
		if (!ldown)
		{
			opened = ecfalse;

			return;
		}
	}
	else if (ie->type == INEV_MOUSEUP && ie->key == MOUSE_LEFT)
	{
		//InfoMess("dllu", "dllu");

#if 0
		if (over)
		{
			ie->intercepted = ectrue;
		}
#endif

		if (opened)
		{
#if 0	//wtf msvs still worked but mac didn't (mac version is correct)
			if (!ldown)
			{
				opened = ecfalse;
				return;
			}
#endif

			//corpd fix
			//did some other widget intercept?
			if (ie->intercepted &&
				ie->interceptor != this)
				opened = ecfalse;

			if (ldown)
			{
				ie->intercepted = ectrue;
				ie->interceptor = this;
				ldown = ecfalse;
			}

			if (mousescroll)
			{
				mousescroll = ecfalse;
				ie->intercepted = ectrue;
				ie->interceptor = this;
				return;	// intercept mouse event
			}

			for (int i = (int)scroll[1]; i < (int)scroll[1] + rowsshown(); i++)
			{
				// std::list item?
				if (g_mouse.x >= pos[0] &&
					g_mouse.x <= pos[2] - square() &&
					g_mouse.y >= drfy[0] + f->gheight*(i - (int)scroll[1]) &&
					g_mouse.y <= drfy[0] + f->gheight*(i - (int)scroll[1] + 1))
				{
					selected = i;
					opened = ecfalse;
					if (changefunc != NULL)
						changefunc();

					ie->intercepted = ectrue;
					ie->interceptor = this;
					return;	// intercept mouse event
				}
			}

			// up button?
			if (g_mouse.x >= pos[2] - square() &&
				g_mouse.y >= upy[0] &&
				g_mouse.x <= pos[2] &&
				g_mouse.y <= upy[1])
			{
				scroll[1]--;
				if (scroll[1] < 0)
					scroll[1] = 0;

				ie->intercepted = ectrue;
				ie->interceptor = this;
				return;
			}

			// down button?
			if (g_mouse.x >= pos[2] - square() &&
				g_mouse.y >= downy[0] &&
				g_mouse.x <= pos[2] &&
				g_mouse.y <= downy[1])
			{
				scroll[1]++;
				if (scroll[1] + rowsshown() > options.size())
					scroll[1] = options.size() - rowsshown();

				ie->intercepted = ectrue;
				ie->interceptor = this;
				return;
			}

			//corpd fix
			//was it outside of this widget?
			if (!ie->intercepted)
				opened = ecfalse;

			ie->intercepted = ectrue;	// intercept mouse event
			ie->interceptor = this;
		}
		//!opened
		else if (!ie->intercepted)
		{
			if (!ldown)
				return;

			ldown = ecfalse;

			if (g_mouse.x >= pos[2] - square() && g_mouse.y >= pos[1] && g_mouse.x <= pos[2] && g_mouse.y <= pos[1] + square())
			{
				opened = ectrue;
				ie->intercepted = ectrue;
				ie->interceptor = this;

				float* parf = g_gui.crop; //parent->pos;
				//see whether up or down has more room
				if (fabs(parf[1] - pos[1]) > fabs(parf[3] - pos[3]))
					downdrop = ecfalse;
				else
					downdrop = ectrue;

				//Need to bring tree to front so that drop-down list gets 
				//the mouse up event first instead of any item in the background.
				Widget* parw = parent;
				while (parw)
				{
					parw->tofront();
					parw = parw->parent;
				}
				tofront();

				return;
			}
		}
	}
}
#endif