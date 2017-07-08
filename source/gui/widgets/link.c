






#include "../widget.h"
#include "button.h"
#include "editbox.h"
#include "image.h"
#include "link.h"
#include "text.h"

void hplinit(hpl* hl, wg* parent, const char* n, const char* t, char f, void(*reframef)(wg* w), void(*click)())
{
	wg* bw;

	bw = (wg*)hl;

	wginit(bw);

	bw->parent = parent;
	bw->type = WIDGET_LINK;
	strcpy(bw->name, n);
	hl->over = ecfalse;
	hl->ldown = ecfalse;
	pstrset(&hl->text, t);
	hl->font = f;
	bw->reframefunc = reframef;
	hl->clickfunc = click;
	wgreframe(bw);
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

	drawt(hl->font, bw->pos, bw->crop, hl->text, color, 0, -1, ectrue, ecfalse);
	s = g_shader+g_cursh;
	glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);
}

void hplinev(wg* bw, inev* ie)
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
			if(hl->clickfunc != NULL)
				hl->clickfunc();

			hl->over = ecfalse;
			hl->ldown = ecfalse;

			ie->intercepted = ectrue;
			return;	// intercept mouse event
		}

		hl->over = ecfalse;
		hl->ldown = ecfalse;
	}
	else if(ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		//mousemove();

		if(hl->over)
		{
			hl->ldown = ectrue;
			ie->intercepted = ectrue;
			return;	// intercept mouse event
		}
	}
	else if(ie->type == INEV_MOUSEMOVE)
	{
		x = endx(hl->text, richlen(hl->text), hl->font, bw->pos, ecfalse);
		if(g_mouse.x >= bw->pos[0] && g_mouse.y >= bw->pos[1] &&
		                g_mouse.x <= x &&	//
		                g_mouse.y <= bw->pos[1]+f->gheight)
		{
		}
		else
		{
			hl->over = ecfalse;
		}

		if(!ie->intercepted)
		{
			if(g_mouse.x >= bw->pos[0] && g_mouse.y >= bw->pos[1] &&
			g_mouse.x <= x &&	//
			                g_mouse.y <= bw->pos[1]+f->gheight)
			{
				hl->over = ectrue;

				ie->intercepted = ectrue;
			}
			else
			{
				hl->over = ecfalse;
			}
		}
	}
}

