






#include "../wg.h"
#include "vp.h"

void vpinit(vp* v, wg* parent, const char* n, void(*reframef)(wg* w),
	void(*drawf)(void *e, int x, int y, int w, int h),
	dbool(*ldownf)(void *e, int relx, int rely, int w, int h),
	dbool(*lupf)(void *e, int relx, int rely, int w, int h),
	dbool(*mmovef)(void *e, int relx, int rely, int w, int h),
	dbool(*rdownf)(void *e, int relx, int rely, int w, int h),
	dbool(*rupf)(void *e, int relx, int rely, int w, int h),
	dbool(*mousewf)(void *e, int d),
	dbool(*mdownf)(void *e, int relx, int rely, int w, int h),
	dbool(*mupf)(void *e, int relx, int rely, int w, int h),
	void *e, void(*freef)(wg* w))
{
	wg *bw;

	bw = (wg*)v;
	wginit(bw);

	bw->parent = parent;
	bw->type = WIDGET_VIEWPORT;
	bw->extra = e;
	bw->reframef = reframef;
	v->drawf = drawf;
	v->ldownf = ldownf;
	v->lupf = lupf;
	v->mmovef = mmovef;
	v->rdownf = rdownf;
	v->rupf = rupf;
	v->mousewf = mousewf;
	v->mdownf = mdownf;
	v->mupf = mupf;
	v->freef = freef;
	v->ldown = dfalse;
	v->over = dfalse;
	wgreframe(bw);
}

void vpfree(wg* w)
{
	vp *v = (vp*)w;

	if (v->freef)
		v->freef(v);
}

void vpin(wg *w, inev *ie)
{
	vp* v = (vp*)w;

	if (ie->type == INEV_MOUSEMOVE)
	{
		if (g_mouse.x >= w->pos[0] && g_mouse.x <= w->pos[2] && g_mouse.y >= w->pos[1] && g_mouse.y <= w->pos[3])
		{
			if (!ie->intercepted)
				v->over = dtrue;
		}
		else
			v->over = dfalse;
	}

	if (ie->type == INEV_MOUSEMOVE && !ie->intercepted)
	{
		if (g_mouse.x >= w->pos[0] && g_mouse.x <= w->pos[2] && g_mouse.y >= w->pos[1] && g_mouse.y <= w->pos[3])
			v->over = dtrue;
		else
			v->over = dfalse;

		if (v->over && v->mmovef != NULL)
		{
			int relx = g_mouse.x - (int)w->pos[0];
			int rely = g_mouse.y - (int)w->pos[1];
			int wd = (int)(w->pos[2] - w->pos[0]);
			int ht = (int)(w->pos[3] - w->pos[1]);
			/* ie->intercepted = v->*/ v->mmovef(w->extra, relx, rely, wd, ht);
		}

		return;
	}
	else if (ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		if (!v->over)
			return;

		if (v->ldownf != NULL)
		{
			int relx = g_mouse.x - (int)w->pos[0];
			int rely = g_mouse.y - (int)w->pos[1];
			int wd = (int)(w->pos[2] - w->pos[0]);
			int ht = (int)(w->pos[3] - w->pos[1]);
			ie->intercepted = v->ldownf(w->extra, relx, rely, wd, ht);
		}
	}
	else if (ie->type == INEV_MOUSEUP && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		if (!v->over)
			return;

		if (v->lupf != NULL)
		{
			int relx = g_mouse.x - (int)w->pos[0];
			int rely = g_mouse.y - (int)w->pos[1];
			int wd = (int)(w->pos[2] - w->pos[0]);
			int ht = (int)(w->pos[3] - w->pos[1]);
			ie->intercepted = v->lupf(w->extra, relx, rely, wd, ht);
		}
	}
	else if (ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_RIGHT && !ie->intercepted)
	{
		if (!v->over)
			return;

		if (v->rdownf != NULL)
		{
			int relx = g_mouse.x - (int)w->pos[0];
			int rely = g_mouse.y - (int)w->pos[1];
			int wd = (int)(w->pos[2] - w->pos[0]);
			int ht = (int)(w->pos[3] - w->pos[1]);
			ie->intercepted = v->rdownf(w->extra, relx, rely, wd, ht);
		}
	}
	else if (ie->type == INEV_MOUSEUP && ie->key == MOUSE_RIGHT && !ie->intercepted)
	{
		if (!v->over)
			return;

		if (v->rupf != NULL)
		{
			int relx = g_mouse.x - (int)w->pos[0];
			int rely = g_mouse.y - (int)w->pos[1];
			int wd = (int)(w->pos[2] - w->pos[0]);
			int ht = (int)(w->pos[3] - w->pos[1]);
			ie->intercepted = v->rupf(w->extra, relx, rely, wd, ht);
		}
	}
	else if (ie->type == INEV_MOUSEWHEEL && !ie->intercepted)
	{
		if (!v->over)
			return;

		if (v->mousewf != NULL)
		{
			ie->intercepted = v->mousewf(w->extra, ie->amount);
		}
	}
}

void vpdraw(wg *w)
{
	int wd, ht;
	int viewport[4];
	glshader* s;
	vp* v = (vp*)w;

	wd = (int)(w->pos[2] - w->pos[0]);
	ht = (int)(w->pos[3] - w->pos[1]);

	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(w->pos[0], (int)(g_height - w->pos[3]), wd, ht);
	s = g_shader + g_cursh;
	glUniform1f(s->slot[SSLOT_WIDTH], (float)wd);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)ht);
	endsh();

	if (v->drawf)
		v->drawf(w->extra, w->pos[0], w->pos[1], w->pos[2] - w->pos[0], w->pos[3] - w->pos[1]);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	flatview(g_width, g_height, 1, 1, 1, 1);
}
