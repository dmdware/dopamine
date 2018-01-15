



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
	bw->type = WG_BUTTON;
	strcpy(bw->name, name);
	b->style = style;
	pstrset(&b->tooltip, tooltip);
	pstrset(&b->label, label);
	b->font = f;

	length = endx(b->tooltip, richlen(b->tooltip), b->font, bw->pos, dfalse);
	b->over = dfalse;
	b->ldown = dfalse;

	if(style == BUST_LEIM)
		createtex(&b->texi, filepath, dtrue, dfalse, dfalse);

	b->bgtex = 0;
	b->bgovtex = 0;

	bw->reframef = reframef;
	b->clickf = click;
	b->clickf2 = click2;
	b->overf = overf;
	b->overf2 = overf2;
	b->outf = out;
	b->param = parm;
	b->clickf3 = click3;
	b->ldown = dfalse;
	wgreframe(bw);
}

void bwgfree(wg* w)
{
	bwg *b = (bwg*)w;

	free(b->label);
	free(b->tooltip);
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

			if(b->clickf2 != NULL)
				b->clickf2(b->param);

			if(b->clickf3 != NULL)
				b->clickf3(bw);

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
			if(b->over && b->outf != NULL)
				b->outf();

			b->over = dfalse;
		}

		if(!ie->intercepted)
		{
			if(g_mouse.x >= bw->pos[0] && g_mouse.x <= bw->pos[2] && g_mouse.y >= bw->pos[1] && g_mouse.y <= bw->pos[3])
			{
				if(b->overf != NULL)
					b->overf();
				if(b->overf2 != NULL)
					b->overf2(b->param);

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
	float mc[] = {MCR,MCG,MCB,MCA};
	float lc[] = {LCR,LCG,LCB,LCA};
	float dc[] = {DCR,DCG,DCB,DCA};
	float *tc = TC;
	char i;
	float w;
	float h;
	float minsz;
	float gheight;
	float texttop;
	float textleft;
	font *f;
	gltex *tex, *bgtex, *bgovtex;

	b = (bwg*)bw;
	f = g_font+b->font;
	tex = g_tex+b->texi;
	bgovtex = g_tex+b->bgovtex;
	bgtex = g_tex+b->bgtex;

	w = bw->pos[2]-bw->pos[0]-2;
	h = bw->pos[3]-bw->pos[1]-2;
	minsz = ((w<h)?w:h);

	/* TODO all to font and lines/quads */

	if(b->style == BUST_LEIM)
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
				mc[i] = 0.8f;
				lc[i] = 0.9f;
				dc[i] = 0.6f;
				tc[i] = 1.0f;
			}
		}

		drawsq(mc[0], mc[1], mc[2], mc[3], bw->pos[0], bw->pos[1], bw->pos[2], bw->pos[3], bw->crop);

		drawl(lc[0], lc[1], lc[2], lc[3], bw->pos[0], bw->pos[1], bw->pos[0], bw->pos[3]-1, bw->crop);
		drawl(lc[0], lc[1], lc[2], lc[3], bw->pos[0], bw->pos[1], bw->pos[2]-1, bw->pos[1], bw->crop);

		drawl(dc[0], dc[1], dc[2], dc[3], bw->pos[0]+1, bw->pos[3], bw->pos[2], bw->pos[3], bw->crop);
		drawl(dc[0], dc[1], dc[2], dc[3], bw->pos[2], bw->pos[1]+1, bw->pos[2], bw->pos[3], bw->crop);

		endsh();
		CHECKGL();
		flatview(g_currw, g_currh, 1, 1, 1, 1);

		drawim(tex->texname, bw->pos[0]+1, bw->pos[1]+1, bw->pos[0]+minsz, bw->pos[1]+minsz, 0,0,1,1, bw->crop);

		gheight = f->gheight;
		texttop = bw->pos[1] + h/2.0f - gheight/2.0f;
		textleft = bw->pos[0]+minsz+1;

		//TODO rewrite font.cpp/h to better deal with cropping
		drawt(b->font, b->tpos, bw->crop, b->label, tc, 0, -1, dtrue, dfalse);
	}
	else if(b->style == BUST_LINE)
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
				mc[i] = 0.8f;
				lc[i] = 0.9f;
				dc[i] = 0.6f;
			}
		}

		drawsq(mc[0], mc[1], mc[2], mc[3], bw->pos[0], bw->pos[1], bw->pos[2], bw->pos[3], bw->crop);

		drawl(lc[0], lc[1], lc[2], lc[3], bw->pos[0], bw->pos[1], bw->pos[0], bw->pos[3]-1, bw->crop);
		drawl(lc[0], lc[1], lc[2], lc[3], bw->pos[0], bw->pos[1], bw->pos[2]-1, bw->pos[1], bw->crop);

		drawl(dc[0], dc[1], dc[2], dc[3], bw->pos[0]+1, bw->pos[3], bw->pos[2], bw->pos[3], bw->crop);
		drawl(dc[0], dc[1], dc[2], dc[3], bw->pos[2], bw->pos[1]+1, bw->pos[2], bw->pos[3], bw->crop);

		endsh();
		CHECKGL();
		flatview(g_currw, g_currh, 1, 1, 1, 1);

		//TODO fix resolution change on settings reload on mobile

		drawt(b->font, b->tpos, bw->crop, b->label, tc, 0, -1, dtrue, dfalse);
	}
}

void bwgdrawov(wg *bw)
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

void cenlab(bwg *b, char *label, char fi, float *pos, float *tpos)
{
	font *f;
	int texwidth;
	wg *bw;

	bw = (wg*)b;
	f = g_font+fi;

	texwidth = textw(fi, pos, label);

	tpos[0] = (pos[2]+pos[0])/2 - texwidth/2;
	tpos[1] = (pos[3]+pos[1])/2 - f->gheight/2;
	tpos[2] = pos[2];
	tpos[3] = pos[3];
}