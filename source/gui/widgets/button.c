



#include "../wg.h"
#include "button.h"
#include "../icon.h"
#include "../wg.h"
#include "../gui.h"
#include "../font.h"
#include "../richtext.h"

void bwginit(bwg *b, wg* parent, const char* name, const char* filepath, 
				 const char* label, const char* tooltip, char f, char style,
				 void (*reframef)(wg* w), void (*click)(), void (*click2)(int p), 
				 void (*overf)(), void (*overf2)(int p), void (*out)(), int parm, 
				 void (*click3)(wg* w))
{
	wg *bw;
	int length;

	bw = (wg*)b;
	wginit(bw);

	bw->parent = parent;
	bw->type = WIDGET_BUTTON;
	strcpy(bw->name, name);
	b->style = style;
	pstrset(&b->tooltip, tooltip);
	pstrset(&b->label, label);
	b->font = f;

	length = endx(b->tooltip, richlen(b->tooltip), b->font, bw->pos, dfalse);
	b->over = dfalse;
	b->ldown = dfalse;

	if(style == BUST_LEFTIMAGE)
		createtex(&b->texi, filepath, dtrue, dfalse, dfalse);

	b->bgtex = 0;
	b->bgovertex = 0;

	bw->reframef = reframef;
	b->clickf = click;
	b->clickfunc2 = click2;
	b->overfunc = overf;
	b->overfunc2 = overf2;
	b->outfunc = out;
	b->param = parm;
	b->clickfunc3 = click3;
	b->ldown = dfalse;
	wgreframe(bw);
}

void bwgin(wg *bw, inev* ie)
{
	bwg *b;

	b = (bwg*)bw;

	if(ie->type == INEV_MOUSEUP && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		//mousemove();

		if(b->over && b->ldown)
		{
			if(b->clickf != NULL)
				b->clickf();

			if(b->clickfunc2 != NULL)
				b->clickfunc2(b->param);

			if(b->clickfunc3 != NULL)
				b->clickfunc3(bw);

			//over = dfalse;
			b->ldown = dfalse;

			ie->intercepted = dtrue;
			
			return;	// intercept mouse event
		}

		if(b->ldown)
		{
			b->ldown = dfalse;
			ie->intercepted = dtrue;
			return;
		}

		b->over = dfalse;
	}
	else if(ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		//mousemove();

		if(b->over)
		{
			b->ldown = dtrue;
			ie->intercepted = dtrue;
			return;	// intercept mouse event
		}
	}
	else if(ie->type == INEV_MOUSEMOVE)
	{
		if(g_mouse.x >= bw->pos[0] && g_mouse.x <= bw->pos[2] && g_mouse.y >= bw->pos[1] && g_mouse.y <= bw->pos[3])
		{
		}
		else
		{
			if(b->over && b->outfunc != NULL)
				b->outfunc();

			b->over = dfalse;
		}

		if(!ie->intercepted)
		{
			if(g_mouse.x >= bw->pos[0] && g_mouse.x <= bw->pos[2] && g_mouse.y >= bw->pos[1] && g_mouse.y <= bw->pos[3])
			{
				if(b->overfunc != NULL)
					b->overfunc();
				if(b->overfunc2 != NULL)
					b->overfunc2(b->param);

				b->over = dtrue;

				ie->intercepted = dtrue;
				return;
			}
		}
	}
}

void bwgdraw(wg *bw)
{
	bwg *b;
	glshader *s;
	float midcolor[] = {0.7f,0.7f,0.7f,0.8f};
	float lightcolor[] = {0.8f,0.8f,0.8f,0.8f};
	float darkcolor[] = {0.5f,0.5f,0.5f,0.8f};
	char i;
	float w;
	float h;
	float minsz;
	float gheight;
	float texttop;
	float textleft;
	font *f;
	gltex *tex, *bgtex, *bgovertex;

	b = (bwg*)bw;
	f = g_font+b->font;
	tex = g_tex+b->texi;
	bgovertex = g_tex+b->bgovertex;
	bgtex = g_tex+b->bgtex;

	w = bw->pos[2]-bw->pos[0]-2;
	h = bw->pos[3]-bw->pos[1]-2;
	minsz = ((w<h)?w:h);

	/* TODO all to font and lines/quads */

	if(b->style == BUST_LEFTIMAGE)
	{
		endsh();

		usesh(SH_COLOR2D);
		s = g_shader+g_cursh;
		glUniform1f(s->slot[SSLOT_WIDTH], (float)g_currw);
		glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_currh);

		if(b->over)
		{
			for(i=0; i<3; ++i)
			{
				midcolor[i] = 0.8f;
				lightcolor[i] = 0.9f;
				darkcolor[i] = 0.6f;
			}
		}

		drawsq(midcolor[0], midcolor[1], midcolor[2], midcolor[3], bw->pos[0], bw->pos[1], bw->pos[2], bw->pos[3], bw->crop);

		drawl(lightcolor[0], lightcolor[1], lightcolor[2], lightcolor[3], bw->pos[2], bw->pos[1], bw->pos[2], bw->pos[3]-1, bw->crop);
		drawl(lightcolor[0], lightcolor[1], lightcolor[2], lightcolor[3], bw->pos[0], bw->pos[1], bw->pos[2]-1, bw->pos[1], bw->crop);

		drawl(darkcolor[0], darkcolor[1], darkcolor[2], darkcolor[3], bw->pos[0]+1, bw->pos[3], bw->pos[2], bw->pos[3], bw->crop);
		drawl(darkcolor[0], darkcolor[1], darkcolor[2], darkcolor[3], bw->pos[2], bw->pos[1]+1, bw->pos[2], bw->pos[3], bw->crop);

		endsh();
		CHECKGL();
		flatview(g_currw, g_currh, 1, 1, 1, 1);

		drawim(tex->texname, bw->pos[0]+1, bw->pos[1]+1, bw->pos[0]+minsz, bw->pos[1]+minsz, 0,0,1,1, bw->crop);

		gheight = f->gheight;
		texttop = bw->pos[1] + h/2.0f - gheight/2.0f;
		textleft = bw->pos[0]+minsz+1;

		//TODO rewrite font.cpp/h to better deal with cropping
		drawt(b->font, b->tpos, bw->crop, b->label, NULL, 0, -1, dtrue, dfalse);
	}
	else if(b->style == BUST_LINEBASED)
	{
		endsh();

		usesh(SH_COLOR2D);
		s = g_shader+g_cursh;
		glUniform1f(s->slot[SSLOT_WIDTH], (float)g_currw);
		glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_currh);

		if(b->over)
		{
			for(i=0; i<3; i++)
			{
				midcolor[i] = 0.8f;
				lightcolor[i] = 0.9f;
				darkcolor[i] = 0.6f;
			}
		}

		drawsq(midcolor[0], midcolor[1], midcolor[2], midcolor[3], bw->pos[0], bw->pos[1], bw->pos[2], bw->pos[3], bw->crop);

		drawl(lightcolor[0], lightcolor[1], lightcolor[2], lightcolor[3], bw->pos[2], bw->pos[1], bw->pos[2], bw->pos[3]-1, bw->crop);
		drawl(lightcolor[0], lightcolor[1], lightcolor[2], lightcolor[3], bw->pos[0], bw->pos[1], bw->pos[2]-1, bw->pos[1], bw->crop);

		drawl(darkcolor[0], darkcolor[1], darkcolor[2], darkcolor[3], bw->pos[0]+1, bw->pos[3], bw->pos[2], bw->pos[3], bw->crop);
		drawl(darkcolor[0], darkcolor[1], darkcolor[2], darkcolor[3], bw->pos[2], bw->pos[1]+1, bw->pos[2], bw->pos[3], bw->crop);

		endsh();
		CHECKGL();
		flatview(g_currw, g_currh, 1, 1, 1, 1);

		//TODO fix resolution change on settings reload on mobile

		drawt(b->font, b->tpos, bw->crop, b->label, NULL, 0, -1, dtrue, dfalse);
	}
}

void bwgdrawover(wg *bw)
{
	float tpos[4];
	bwg *b;

	b = (bwg*)bw;

	if(b->over)
	{
		tpos[0] = (float)g_mouse.x;
		tpos[1] = (float)g_mouse.y;
		tpos[2] = tpos[0] + 256;
		tpos[3] = tpos[1] + 53;

		drawt(b->font, tpos, bw->crop, b->tooltip, NULL, 0, -1, dtrue, dfalse);
	}
}

void cenlab(bwg *b)
{
	font* f;
	int texwidth;
	wg *bw;

	bw = (wg*)b;
	f = g_font+b->font;

	texwidth = textw(b->font, bw->pos, b->label);

	b->tpos[0] = (bw->pos[2]+bw->pos[0])/2 - texwidth/2;
	b->tpos[1] = (bw->pos[3]+bw->pos[1])/2 - f->gheight/2;
	b->tpos[2] = bw->pos[2];
	b->tpos[3] = bw->pos[3];
}