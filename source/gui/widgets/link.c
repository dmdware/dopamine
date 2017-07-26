






#include "../wg.h"
#include "button.h"
#include "editbox.h"
#include "image.h"
#include "link.h"
#include "text.h"

void hplinit(hpl* hl, wg* parent, const char* n, const char* t, char f, void(*reframef)(wg* w), void(*click)(), void *e, void(*freef)(wg* w))
{
	wg* bw;

	bw = (wg*)hl;

	wginit(bw);

	bw->parent = parent;
	bw->type = WIDGET_LINK;
	strcpy(bw->name, n);
	hl->over = dfalse;
	hl->ldown = dfalse;
	pstrset(&hl->text, t);
	hl->font = f;
	bw->reframef = reframef;
	hl->clickf = click;
	bw->extra = e;
	hl->freef = freef;
	wgreframe(bw);
}

void hplfree(wg* w)
{
	hpl *hl = (hpl*)w;

	if(hl->freef)
		hl->freef(hl);
}

void hpldraw(wg* bw)
{
	hpl *hl;
	float color[] = { 1, 1, 1, 1 };
	glshader *s;

	hl = (hpl*)bw;

	glDisable(GL_TEXTURE_2D);

	if(!hl->over)
	{
		color[0] = 0.8f;
		color[1] = 0.8f;
		color[2] = 0.8f;
	}

	drawt(hl->font, bw->pos, bw->crop, hl->text, color, 0, -1, dtrue, dfalse);
	s = g_shader+g_cursh;
	glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);
}

void hplin(wg* bw, inev* ie)
{
	hpl *hl;
	int x;
	font *f;

	hl = (hpl*)bw;
	f = g_font+hl->font;

	if(ie->type == INEV_MOUSEUP && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		//mousemove();
		
		if(hl->over && hl->ldown)
		{
			if(hl->clickf != NULL)
				hl->clickf();

			hl->over = dfalse;
			hl->ldown = dfalse;

			ie->intercepted = dtrue;
			return;	// intercept mouse event
		}

		hl->over = dfalse;
		hl->ldown = dfalse;
	}
	else if(ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		//mousemove();

		if(hl->over)
		{
			hl->ldown = dtrue;
			ie->intercepted = dtrue;
			return;	// intercept mouse event
		}
	}
	else if(ie->type == INEV_MOUSEMOVE)
	{
		x = endx(hl->text, richlen(hl->text), hl->font, bw->pos, dfalse);
		if(g_mouse.x >= bw->pos[0] && g_mouse.y >= bw->pos[1] &&
		                g_mouse.x <= x &&	//
		                g_mouse.y <= bw->pos[1]+f->gheight)
		{
		}
		else
		{
			hl->over = dfalse;
		}

		if(!ie->intercepted)
		{
			if(g_mouse.x >= bw->pos[0] && g_mouse.y >= bw->pos[1] &&
			g_mouse.x <= x &&	//
			                g_mouse.y <= bw->pos[1]+f->gheight)
			{
				hl->over = dtrue;

				ie->intercepted = dtrue;
			}
			else
			{
				hl->over = dfalse;
			}
		}
	}
}

