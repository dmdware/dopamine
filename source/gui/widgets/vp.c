






#include "../wg.h"
#include "vp.h"

void vpinit(wg* parent, vp* v, const char* n, void(*reframef)(wg* w),
	void(*drawf)(int p, int x, int y, int w, int h),
	dbool(*ldownf)(int p, int relx, int rely, int w, int h),
	dbool(*lupf)(int p, int relx, int rely, int w, int h),
	dbool(*mmovef)(int p, int relx, int rely, int w, int h),
	dbool(*rdownf)(int p, int relx, int rely, int w, int h),
	dbool(*rupf)(int p, int relx, int rely, int w, int h),
	dbool(*mousewf)(int p, int d),
	dbool(*mdownf)(int p, int relx, int rely, int w, int h),
	dbool(*mupf)(int p, int relx, int rely, int w, int h),
	void *e, void(*freef)(wg* w))
{
	wg *bw;

	bw = (wg*)v;
	wginit(bw);

	bw->parent = parent;
	bw->type = WIDGET_VIEWLAYER;
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
}

void vpdraw(wg *w)
{
}

#if 0
Viewport::Viewport()
{
	m_parent = NULL;
	m_type = WIDGET_VIEWPORT;
	m_name = "";
	reframefunc = NULL;
	m_ldown = false;
	m_param = -1;
	drawf = NULL;
	ldownf = NULL;
	lupf = NULL;
	mmovef = NULL;
	rdownf = NULL;
	rupf = NULL;
	mousewf = NULL;
	//reframe();
}


Viewport::Viewport(Widget* parent, const char* n, void (*reframef)(Widget* thisw),
                     void (*drawf)(int p, int x, int y, int w, int h),
                     dbool (*ldownf)(int p, int x, int y, int w, int h),
                     dbool (*lupf)(int p, int x, int y, int w, int h),
                     dbool (*mmovef)(int p, int x, int y, int w, int h),
                     dbool (*rdownf)(int p, int relx, int rely, int w, int h),
                     dbool (*rupf)(int p, int relx, int rely, int w, int h),
                     dbool (*mousewf)(int p, int d),
                     dbool (*mdownf)(int p, int relx, int rely, int w, int h),
                     dbool (*mupf)(int p, int relx, int rely, int w, int h),
                     int parm)
{
    m_parent = parent;
    m_type = WIDGET_VIEWPORT;
    m_name = n;
    reframefunc = reframef;
    m_ldown = false;
    m_param = parm;
    drawf = drawf;
    ldownf = ldownf;
    lupf = lupf;
    mmovef = mmovef;
    rdownf = rdownf;
    rupf = rupf;
    mousewf = mousewf;
    mdownf = mdownf;
    mupf = mupf;
    reframe();
}

void Viewport::draw()
{
	//Log(m_pos[0]<<","<<m_pos[1]<<","<<m_pos[2]<<","<<m_pos[3]);

	int w = (int)( m_pos[2] - m_pos[0] );
	int h = (int)( m_pos[3] - m_pos[1] );

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(m_pos[0], (int)(g_height-m_pos[3]), w, h);
	glUniform1f(g_shader[SHADER_ORTHO].m_slot[SSLOT_WIDTH], (float)w);
	glUniform1f(g_shader[SHADER_ORTHO].m_slot[SSLOT_HEIGHT], (float)h);


	EndS();
	
#if 1
	CHECKGLERROR();

	if(drawf != NULL)
		drawf(m_param, m_pos[0], m_pos[1], w, h);

	CHECKGLERROR();
#endif

	//glViewport(0, 0, g_width, g_height);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	//glUniform1f(g_shader[SHADER_ORTHO].m_slot[SSLOT_WIDTH], (float)g_width);
	//glUniform1f(g_shader[SHADER_ORTHO].m_slot[SSLOT_HEIGHT], (float)g_height);

	CHECKGLERROR();
	Ortho(g_width, g_height, 1, 1, 1, 1);
}

void Viewport::in(InEv* ie)
{
	if(ie->type == INEV_MOUSEMOVE)
	{
		if(g_mouse.x >= m_pos[0] && g_mouse.x <= m_pos[2] && g_mouse.y >= m_pos[1] && g_mouse.y <= m_pos[3])
		{
			if(!ie->intercepted)
				m_over = true;
		}
		else
			m_over = false;
	}
	
	if(ie->type == INEV_MOUSEMOVE && !ie->intercepted)
	{
		if(g_mouse.x >= m_pos[0] && g_mouse.x <= m_pos[2] && g_mouse.y >= m_pos[1] && g_mouse.y <= m_pos[3])
			m_over = true;
		else
			m_over = false;

		if(m_over && mmovef != NULL)
		{
			int relx = g_mouse.x - (int)m_pos[0];
			int rely = g_mouse.y - (int)m_pos[1];
			int w = (int)( m_pos[2] - m_pos[0] );
			int h = (int)( m_pos[3] - m_pos[1] );
			/* ie->intercepted = */ mmovef(m_param, relx, rely, w, h);
		}

		return;
	}
	else if(ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		//InfoMess("vpld", "vpld");

		if(!m_over)
			return;

		if(ldownf != NULL)
		{
			int relx = g_mouse.x - (int)m_pos[0];
			int rely = g_mouse.y - (int)m_pos[1];
			int w = (int)( m_pos[2] - m_pos[0] );
			int h = (int)( m_pos[3] - m_pos[1] );
			ie->intercepted = ldownf(m_param, relx, rely, w, h);
		}
	}
	else if(ie->type == INEV_MOUSEUP && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		//InfoMess("vplu", "vplu");

		if(!m_over)
			return;

		if(lupf != NULL)
		{
			int relx = g_mouse.x - (int)m_pos[0];
			int rely = g_mouse.y - (int)m_pos[1];
			int w = (int)( m_pos[2] - m_pos[0] );
			int h = (int)( m_pos[3] - m_pos[1] );
			ie->intercepted = lupf(m_param, relx, rely, w, h);
		}
	}
	else if(ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_RIGHT && !ie->intercepted)
	{
		if(!m_over)
			return;

		if(rdownf != NULL)
		{
			int relx = g_mouse.x - (int)m_pos[0];
			int rely = g_mouse.y - (int)m_pos[1];
			int w = (int)( m_pos[2] - m_pos[0] );
			int h = (int)( m_pos[3] - m_pos[1] );
			ie->intercepted = rdownf(m_param, relx, rely, w, h);
		}
	}
	else if(ie->type == INEV_MOUSEUP && ie->key == MOUSE_RIGHT && !ie->intercepted)
	{
		if(!m_over)
			return;

		if(rupf != NULL)
		{
			int relx = g_mouse.x - (int)m_pos[0];
			int rely = g_mouse.y - (int)m_pos[1];
			int w = (int)( m_pos[2] - m_pos[0] );
			int h = (int)( m_pos[3] - m_pos[1] );
			ie->intercepted = rupf(m_param, relx, rely, w, h);
		}
	}
	else if(ie->type == INEV_MOUSEWHEEL && !ie->intercepted)
	{
		if(!m_over)
			return;

		if(mousewf != NULL)
		{
			ie->intercepted = mousewf(m_param, ie->amount);
		}
	}
}
#endif