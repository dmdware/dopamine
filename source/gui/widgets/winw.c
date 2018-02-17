










#include "../gui.h"
#include "../../sys/texture.h"
#include "winw.h"
#include "../../sys/debug.h"
#include "../font.h"
#include "../richtext.h"

void wwinit(ww *b, wg* parent, const char* name,
	const char* title, char f, void(*reframef)(wg* w),
	dbool start)
{
	wg *bw;
	int length;
	char i;

	bw = (wg*)b;
	wginit(bw);

	bw->parent = parent;
	bw->type = WG_WIN;
	strcpy(bw->name, name);
	pstrset(&b->title, title);
	b->font = f;
	b->over = dfalse;
	b->ldown = dfalse;
	bw->reframef = reframef;
	bw->hidden = !start;
	wgreframe(bw);
}

void wwfree(wg* bw)
{
	ww *w = (ww*)bw;

	free(w->title);
}

#define WMX	6
#define WMY	22

void wwnewsz(wg *bw)
{
	bw->pos[0] = g_width / 2 - 100;
	bw->pos[1] = g_height / 2 - 100;
	bw->pos[2] = g_width / 2 + 100;
	bw->pos[3] = g_height / 2 + 100;
}

void wwsz(wg* bw)
{
	ww *w;
	w = (ww*)bw;
	w->opos[0] = bw->pos[0] - WMX;
	w->opos[1] = bw->pos[1] - WMY;
	w->opos[2] = bw->pos[2] + WMX;
	w->opos[3] = bw->pos[3] + WMX;
	w->tpos[0] = w->opos[0];
	w->tpos[1] = w->opos[1];
	w->tpos[2] = w->opos[2] - WMY * 3;
	w->tpos[3] = bw->pos[1];
}

void wwin(wg *bw, inev* ie)
{
	ww *b;

	b = (ww*)bw;

	if (ie->type == IE_MOUSEUP && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		if (b->over && b->ldown)
		{
			b->ldown = dfalse;

			ie->intercepted = dtrue;

			return;	// intercept mouse event
		}

		if (b->ldown)
		{
			b->ldown = dfalse;
			ie->intercepted = dtrue;
			return;
		}

		b->over = dfalse;
	}
	else if (ie->type == IE_MOUSEDOWN && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		if (b->over)
		{
			b->ldown = dtrue;
			ie->intercepted = dtrue;
			return;	// intercept mouse event
		}
	}
	else if (ie->type == IE_MOUSEMOVE)
	{
		if (g_mouse.x >= b->opos[0] && g_mouse.x < b->opos[2] && g_mouse.y >= b->opos[1] && g_mouse.y < b->opos[3])
		{
		}
		else
		{
			b->over = dfalse;
		}

		if (!ie->intercepted)
		{
			if (g_mouse.x >= b->opos[0] && g_mouse.x < b->opos[2] && g_mouse.y >= b->opos[1] && g_mouse.y < b->opos[3])
			{
				b->over = dtrue;

				ie->intercepted = dtrue;
				return;
			}
		}
	}
}

void wwdraw(wg *bw)
{
	ww *b;
	glshader *s;
	float mc[] = { MCR,MCG,MCB,MCA };
	float lc[] = { LCR,LCG,LCB,LCA };
	float dc[] = { DCR,DCG,DCB,DCA };
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
	float *fcrop;
	wg *gui;
	float fs;
	float cb[4];

	b = (ww*)bw;
	f = g_font + b->font;
	gui = (wg*)&g_gui;
	fcrop = gui->crop;

	w = bw->pos[2] - bw->pos[0] - 2;
	h = bw->pos[3] - bw->pos[1] - 2;
	minsz = ((w < h) ? w : h);
	fs = bw->pos[1] - b->opos[1];

	cb[0] = b->opos[2] - fs - fs + 1;
	cb[1] = b->opos[1] + 1;
	cb[2] = b->opos[2] - fs - 1;
	cb[3] = bw->pos[1] - 1;

	/* TODO all to font and lines/quads */

	endsh();

	usesh(SH_COLOR2D);
	s = g_shader + g_cursh;
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_currw);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_currh);

	drawsq(mc[0], mc[1], mc[2], mc[3], b->opos[0], b->opos[1], b->opos[2], b->opos[3], fcrop);

	drawl(lc[0], lc[1], lc[2], lc[3], b->opos[0], b->opos[1], b->opos[0], b->opos[3] - 1, fcrop);
	drawl(lc[0], lc[1], lc[2], lc[3], b->opos[0], b->opos[1], b->opos[2] - 1, b->opos[1], fcrop);
	drawl(dc[0], dc[1], dc[2], dc[3], b->opos[0] + 1, b->opos[3], b->opos[2], b->opos[3], fcrop);
	drawl(dc[0], dc[1], dc[2], dc[3], b->opos[2], b->opos[1] + 1, b->opos[2], b->opos[3], fcrop);

	drawl(dc[0], dc[1], dc[2], dc[3], bw->pos[0], bw->pos[1], bw->pos[0], bw->pos[3] - 1, fcrop);
	drawl(dc[0], dc[1], dc[2], dc[3], bw->pos[0], bw->pos[1], bw->pos[2] - 1, bw->pos[1], fcrop);
	drawl(lc[0], lc[1], lc[2], lc[3], bw->pos[0] + 1, bw->pos[3], bw->pos[2], bw->pos[3], fcrop);
	drawl(lc[0], lc[1], lc[2], lc[3], bw->pos[2], bw->pos[1] + 1, bw->pos[2], bw->pos[3], fcrop);

	drawsq(mc[0], mc[1], mc[2], mc[3], cb[0], cb[1], cb[2], cb[3], fcrop);
	drawl(dc[0], dc[1], dc[2], dc[3], cb[0], cb[3], cb[2], cb[3], fcrop);
	drawl(dc[0], dc[1], dc[2], dc[3], cb[2], cb[1], cb[2], cb[3], fcrop);
	drawl(lc[0], lc[1], lc[2], lc[3], cb[0], cb[1], cb[2], cb[1], fcrop);
	drawl(lc[0], lc[1], lc[2], lc[3], cb[0], cb[1], cb[0], cb[3], fcrop);

	drawl(lc[0], lc[1], lc[2], lc[3], cb[0] + 3, cb[1] + 3, cb[2] - 3, cb[3] - 3, fcrop);
	drawl(lc[0], lc[1], lc[2], lc[3], cb[0] + 3, cb[3] - 3, cb[2] - 3, cb[1] + 3, fcrop);

	endsh();
	CHECKGL();
	flatview(g_currw, g_currh, 1, 1, 1, 1);

	//TODO fix resolution change on settings reload on mobile

	drawt(b->font, b->tpos, fcrop, b->title, tc, 0, -1, dtrue, dfalse);
}

void wwdrawov(wg *bw)
{
	float tpos[4];
	ww *b;
	b = (ww*)bw;
}

#if 0

void Click_WinClose(Widget* thisw)
{
	thisw->m_parent->hide();
}

void Click_WinSize(Widget* thisw)
{
	Win* win = (Win*)thisw->m_parent;
	win->fullsize();
}

Win::Win() : Widget()
{
	m_type = WIDGET_WINDOW;
	m_parent = NULL;
	m_opened = false;
}

void Win::fullsize()
{
	float* parf = m_parent->m_pos;	//parent frame
	float outpos[4];
	fillout(outpos);

	if(outpos[0] == parf[0] &&
		outpos[1] == parf[1] &&
		outpos[2] == parf[2] &&
		outpos[3] == parf[3])
	{
		//already fullscreen, make smaller
		memcpy(m_pos, prevpos, sizeof(float)*4);
	}
	else
	{
		//currently not fullscreen, make fullscreen
		memcpy(prevpos, m_pos, sizeof(float)*4);

		float border[4];
		border[0] = m_pos[0] - outpos[0];
		border[1] = m_pos[1] - outpos[1];
		border[2] = outpos[2] - m_pos[2];
		border[3] = outpos[3] - m_pos[3];
		
		m_pos[0] = border[0] + parf[0];
		m_pos[1] = border[1] + parf[1];
		m_pos[2] = parf[2] - border[2];
		m_pos[3] = parf[3] - border[3];
	}

	reframe();
}

void Win::chcall(Widget* ch, int32_t type, void* data)
{
	if(ch == &m_vscroll)
	{

	}
	//else if(ch == &hscroll)
	{
	}
}

Win::Win(Widget* parent, const char* n, void (*reframef)(Widget* w)) : Widget()
{
	m_type = WIDGET_WINDOW;
	reframefunc = reframef;
	m_parent = parent;
	m_name = n;
	m_scroll[0] = 0;
	m_scroll[1] = 0;
	//m_hidden = true;
	m_hidden = false;

	const float alpha = 0.9f;

	CHECKGLERROR();

	bg_logo_image = Image(this, "", "gui/centerp/pcsgray.png", true, NULL, 1, 1, 1, alpha/8.0f,		0, 0, 1, 1);

	m_vscroll = VScroll(this, "m_vscroll");

	title_text = Text(this, "title", RichText(""), FONT_EUROSTILE16, NULL, true, 1, 1, 1, 1.0f);
	
	m_trclose = Button(this, "close", "gui/cancel.png", RichText(), RichText(), MAINFONT16, BUST_LEFTIMAGE, NULL, NULL, NULL, NULL, NULL, NULL, -1, Click_WinClose);
	m_trfull = Button(this, "size", "gui/fullsize.png", RichText(), RichText(), MAINFONT16, BUST_LEFTIMAGE, NULL, NULL, NULL, NULL, NULL, NULL, -1, Click_WinSize);

	// only call this when window is created
	if(reframefunc)
		reframefunc(this);
	else //default size and position
	{
		// to do
	}

	reframe();
}

void Win::reframe()
{
#if 1
	if(m_parent)
	{
		SubCrop(m_parent->m_crop, m_pos, m_crop);
	}
	else
	{
		m_crop[0] = 0;
		m_crop[1] = 0;
		m_crop[2] = (float)g_width-1;
		m_crop[3] = (float)g_height-1;
	}
#endif

	m_scar[0] = m_pos[0];
	m_scar[1] = m_pos[1];
	m_scar[2] = m_pos[2];
	m_scar[3] = m_pos[3];

	for(std::list<Widget*>::iterator i=m_subwidg.begin(); i!=m_subwidg.end(); i++)
		(*i)->reframe();

	//m_crop[0] = 0;
	//m_crop[1] = 0;
	//m_crop[2] = (float)g_width-1;
	//m_crop[3] = (float)g_height-1;
	//fillout(m_crop);

	//m_vscroll.m_pos[0] = innerright - 3 - 10;
	//m_vscroll.m_pos[1] = innertop + 27;
	//m_vscroll.m_pos[2] = innerright - 3;
	//m_vscroll.m_pos[3] = innerbottom - 27 - 10;
	m_vscroll.reframe();

	//title_text.m_pos[0] = inner_top_mid_image.m_pos[0];
	//title_text.m_pos[1] = inner_top_mid_image.m_pos[1];
	//title_text.m_pos[2] = inner_top_mid_image.m_pos[2];
	//title_text.m_pos[3] = inner_top_mid_image.m_pos[1] + 32;
	title_text.reframe();

	float outpos[4];
	fillout(outpos);

	m_trclose.m_pos[0] = outpos[2] - (m_pos[1]-outpos[1]) * 2;
	m_trclose.m_pos[1] = outpos[1];
	m_trclose.m_pos[2] = outpos[2] - (m_pos[1]-outpos[1]) * 1;
	m_trclose.m_pos[3] = m_pos[1];
	m_trclose.reframe();
	m_trclose.m_crop[0] = outpos[0];
	m_trclose.m_crop[1] = outpos[1];
	m_trclose.m_crop[2] = outpos[2];
	m_trclose.m_crop[3] = outpos[3];

	m_trfull.m_pos[0] = outpos[2] - (m_pos[1]-outpos[1]) * 3;
	m_trfull.m_pos[1] = outpos[1];
	m_trfull.m_pos[2] = outpos[2] - (m_pos[1]-outpos[1]) * 2;
	m_trfull.m_pos[3] = m_pos[1];
	m_trfull.reframe();
	m_trfull.m_crop[0] = outpos[0];
	m_trfull.m_crop[1] = outpos[1];
	m_trfull.m_crop[2] = outpos[2];
	m_trfull.m_crop[3] = outpos[3];

	float minsz = fmin((m_pos[2]-m_pos[0]),(m_pos[3]-m_pos[1]));

	bg_logo_image.m_pos[0] = (m_pos[0]+m_pos[2])/2.0f - minsz/2.0f;
	bg_logo_image.m_pos[1] = (m_pos[3]+m_pos[1])/2.0f - minsz/2.0f;
	bg_logo_image.m_pos[2] = (m_pos[0]+m_pos[2])/2.0f + minsz/2.0f;
	bg_logo_image.m_pos[3] = (m_pos[3]+m_pos[1])/2.0f + minsz/2.0f;
	bg_logo_image.reframe();

	m_scar[2] = m_scar[0];
	m_scar[3] = m_scar[1];
	for(std::list<Widget*>::iterator i=m_subwidg.begin(); i!=m_subwidg.end(); i++)
	{
		Widget* w = *i;
		m_scar[2] = fmax(m_scar[2], w->m_pos[2]);
		m_scar[3] = fmax(m_scar[3], w->m_pos[3]);
	}
}

void Win::fillout(float* outpos)
{
	BmpFont* f = &g_font[MAINFONT16];
	outpos[0] = m_pos[0] - 7;
	outpos[1] = m_pos[1] - 7 - f->gheight;
	outpos[2] = m_pos[2] + 7;
	outpos[3] = m_pos[3] + 7;
}

void Win::draw()
{
	EndS();
	UseS(SHADER_COLOR2D);
	Shader* s = &g_shader[g_curS];
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_width);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_height);
	glUniform4f(s->slot[SSLOT_COLOR], 1,1,1,1);

	float outpos[4];

	fillout(outpos);

	DrawSquare(0.5f, 0.5f, 0.5f, 0.9f, outpos[0], outpos[1], outpos[2], outpos[3], outpos);
	
	DrawLine(0.6f, 0.6f, 0.6f, 0.9f, outpos[0], outpos[1], outpos[2], outpos[1], outpos);	//top line outer
	DrawLine(0.6f, 0.6f, 0.6f, 0.9f, outpos[0], outpos[1], outpos[0], outpos[3], outpos);	//left line outer
	DrawLine(0.4f, 0.4f, 0.4f, 0.9f, outpos[0], outpos[3], outpos[2], outpos[3], outpos);	//bottom line outer
	DrawLine(0.4f, 0.4f, 0.4f, 0.9f, outpos[2], outpos[1], outpos[2], outpos[3], outpos);	//right line outer

	DrawLine(0.4f, 0.4f, 0.4f, 0.9f, m_pos[0]-1, m_pos[1]-1, m_pos[2]+1, m_pos[1]-1, outpos);	//top line inner
	DrawLine(0.4f, 0.4f, 0.4f, 0.9f, m_pos[0]-1, m_pos[1]-1, m_pos[0]-1, m_pos[3]+1, outpos);	//left line inner
	DrawLine(0.6f, 0.6f, 0.6f, 0.9f, m_pos[0]-1, m_pos[3]+1, m_pos[2]+1, m_pos[3]+1, outpos);	//bottom line inner
	DrawLine(0.6f, 0.6f, 0.6f, 0.9f, m_pos[2]+1, m_pos[1]-1, m_pos[2]+1, m_pos[3]+1, outpos);	//right line inner

	EndS();
	Ortho(g_width, g_height, 1, 1, 1, 1);

	//bg_logo_image.draw();

	//m_vscroll.draw();
	title_text.draw();
	m_trclose.draw();
	m_trfull.draw();
	
	Widget::draw();
}

void Win::drawover()
{
	//m_vscroll.drawover();

	Widget::drawover();
}

void Win::show()
{
	Widget::show();
	
	//necessary for window widgets:
	tofront();	//can't break list iterator, might shift
}

void Win::inev(InEv* ie)
{
	Widget::inev(ie);

	//m_vscroll.inev(ie);
	
	m_trclose.inev(ie);
	m_trfull.inev(ie);
	
	float outpos[4];
	fillout(outpos);

	if(m_ldown)
	{
		if(ie->type == INEV_MOUSEMOVE ||
		                ( (ie->type == INEV_MOUSEDOWN || ie->type == INEV_MOUSEUP) && ie->key == MOUSE_LEFT) )
			ie->intercepted = true;

		if(ie->type == INEV_MOUSEUP && ie->key == MOUSE_LEFT)
			m_ldown = false;

		if(ie->type == INEV_MOUSEMOVE)
		{
			int32_t dx = g_mouse.x - m_mousedown[0];
			int32_t dy = g_mouse.y - m_mousedown[1];
			m_mousedown[0] = g_mouse.x;
			m_mousedown[1] = g_mouse.y;

			if(g_curst == CU_MOVE)
			{
				m_pos[0] += dx;
				m_pos[1] += dy;
				m_pos[2] += dx;
				m_pos[3] += dy;

				if(m_pos[0] < 0)
				{
					m_pos[2] -= m_pos[0] - (float)0;
					m_pos[0] = (float)0;
				}
				if(m_pos[2] > (float)g_width)
				{
					m_pos[0] -= m_pos[2] - (float)g_width;
					m_pos[2] = (float)(g_width);
				}
				if(m_pos[1] < 0)
				{
					m_pos[3] -= m_pos[1];
					m_pos[1] = (float)64;
				}
				if(m_pos[3] > g_height)
				{
					m_pos[1] -= m_pos[3] - g_height;
					m_pos[3] = (float)(g_height);
				}
				
				memcpy(prevpos, m_pos, sizeof(float)*4);

				reframe();
			}
			else if(g_curst == CU_RESZT)
			{
				int32_t newh = (int32_t)( m_pos[3]-m_pos[1]-dy );
				if(newh < 0) newh = 0;
				m_pos[1] = m_pos[3] - newh;
				if(m_pos[1] < 0) m_pos[1] = (float)0;
				if(m_pos[1] > g_height) m_pos[1] = (float)(g_height);
				memcpy(prevpos, m_pos, sizeof(float)*4);
				reframe();
			}
			else if(g_curst == CU_RESZB)
			{
				int32_t newh = (int32_t)( m_pos[3]-m_pos[1]+dy );
				if(newh < 0) newh = 0;
				m_pos[3] = m_pos[1] + newh;
				if(m_pos[3] < 0) m_pos[3] = (float)0;
				if(m_pos[3] > g_height) m_pos[3] = (float)(g_height);
				memcpy(prevpos, m_pos, sizeof(float)*4);
				reframe();
			}
			else if(g_curst == CU_RESZL)
			{
				int32_t neww = (int32_t)( m_pos[2]-m_pos[0]-dx );
				if(neww < 0) neww = 0;
				m_pos[0] = m_pos[2] - neww;
				if(m_pos[0] < 0) m_pos[0] = (float)0;
				if(m_pos[0] > g_width) m_pos[0] = (float)(g_width);
				memcpy(prevpos, m_pos, sizeof(float)*4);
				reframe();
			}
			else if(g_curst == CU_RESZR)
			{
				int32_t neww = (int32_t)( m_pos[2]-m_pos[0]+dx );
				if(neww < 0) neww = 0;
				m_pos[2] = m_pos[0] + neww;
				if(m_pos[2] < 0) m_pos[2] = (float)0;
				if(m_pos[2] > g_width) m_pos[2] = (float)(g_width);
				memcpy(prevpos, m_pos, sizeof(float)*4);
				reframe();
			}
			else if(g_curst == CU_RESZTL)
			{
				int32_t newh = (int32_t)( m_pos[3]-m_pos[1]-dy );
				if(newh < 0) newh = 0;
				m_pos[1] = m_pos[3] - newh;
				if(m_pos[1] < 0) m_pos[1] = (float)0;
				if(m_pos[1] > g_height) m_pos[1] = (float)(g_height);

				int32_t neww = (int32_t)( m_pos[2]-m_pos[0]-dx );
				if(neww < 0) neww = 0;
				m_pos[0] = m_pos[2] - neww;
				if(m_pos[0] < 0) m_pos[0] = (float)0;
				if(m_pos[0] > g_width) m_pos[0] = (float)(g_width);
				
				memcpy(prevpos, m_pos, sizeof(float)*4);
				reframe();
			}
			else if(g_curst == CU_RESZTR)
			{
				int32_t newh = (int32_t)( m_pos[3]-m_pos[1]-dy );
				if(newh < 0) newh = 0;
				m_pos[1] = m_pos[3] - newh;
				if(m_pos[1] < 0) m_pos[1] = (float)0;
				if(m_pos[1] > g_height) m_pos[1] = (float)(g_height);

				int32_t neww = (int32_t)( m_pos[2]-m_pos[0]+dx );
				if(neww < 0) neww = 0;
				m_pos[2] = (float)( m_pos[0] + neww );
				if(m_pos[2] < 0) m_pos[2] = (float)0;
				if(m_pos[2] > g_width) m_pos[2] = (float)(g_width);
				
				memcpy(prevpos, m_pos, sizeof(float)*4);
				reframe();
			}
			else if(g_curst == CU_RESZBL) 
			{
				int32_t newh = (int32_t)( m_pos[3]-m_pos[1]+dy );
				if(newh < 0) newh = 0;
				m_pos[3] = m_pos[1] + newh;
				if(m_pos[3] < 0) m_pos[3] = (float)0;
				if(m_pos[3] > g_height) m_pos[3] = (float)(g_height);

				int32_t neww = (int32_t)( m_pos[2]-m_pos[0]-dx );
				if(neww < 0) neww = 0;
				m_pos[0] = m_pos[2] - neww;
				if(m_pos[0] < 0) m_pos[0] = (float)0;
				if(m_pos[0] > g_width) m_pos[0] = (float)(g_width);
				
				memcpy(prevpos, m_pos, sizeof(float)*4);
				reframe();
			}
			else if(g_curst == CU_RESZBR)
			{
				int32_t newh = (int32_t)( m_pos[3]-m_pos[1]+dy );
				if(newh < 0) newh = 0;
				m_pos[3] = m_pos[1] + newh;
				if(m_pos[3] < 0) m_pos[3] = (float)0;
				if(m_pos[3] > g_height) m_pos[3] = (float)(g_height);

				int32_t neww = (int32_t)( m_pos[2]-m_pos[0]+dx );
				if(neww < 0) neww = 0;
				m_pos[2] = m_pos[0] + neww;
				if(m_pos[2] < 0) m_pos[2] = (float)0;
				if(m_pos[2] > g_width) m_pos[2] = (float)(g_width);
				
				memcpy(prevpos, m_pos, sizeof(float)*4);
				reframe();
			}
		}
	}

	if(m_over && ie->type == INEV_MOUSEDOWN && !ie->intercepted)
	{
		if(ie->key == MOUSE_LEFT)
		{
			//InfoMess("win ev", "WE");

			m_mousedown[0] = g_mouse.x;
			m_mousedown[1] = g_mouse.y;
			m_ldown = true;
			ie->intercepted = true;
			tofront();	//can't change list order, breaks iterators
		}
	}

	if(ie->type == INEV_MOUSEMOVE)
	{
		if(m_ldown)
		{
			ie->intercepted = true;
			ie->curst = g_curst;
			return;
		}

		if(!ie->intercepted)
		{
			if( g_mouse.x >= outpos[0] &&
			   g_mouse.y >= outpos[1] &&
			   g_mouse.x <= outpos[2] &&
			   g_mouse.y <= outpos[3])
			{
				m_over = true;
				
				//InfoMess("win evOVER", "OVER");
				
				if(g_mousekeys[MOUSE_MIDDLE])
					return;
				
				//ie->intercepted = true;
				
				if(g_curst == CU_DRAG)
					return;
				
				if(g_mouse.x <= m_pos[0])
				{
					if(g_mouse.y <= m_pos[1])
						ie->curst = CU_RESZTL;
					else if(g_mouse.y >= m_pos[3])
						ie->curst = CU_RESZBL;
					else
						ie->curst = CU_RESZL;
					
					ie->intercepted = true;
				}
				else if(g_mouse.x >= m_pos[2])
				{
					if(g_mouse.y <= m_pos[1])
						ie->curst = CU_RESZTR;
					else if(g_mouse.y >= m_pos[3])
						ie->curst = CU_RESZBR;
					else
						ie->curst = CU_RESZR;
					
					ie->intercepted = true;
				}
				else if(g_mouse.x >= outpos[0] &&
						g_mouse.x <= outpos[2])
				{
					if(g_mouse.y <= outpos[1]+4)
						ie->curst = CU_RESZT;
					else if(g_mouse.y >= m_pos[3])
						ie->curst = CU_RESZB;
					else if(g_mouse.x >= m_pos[0] &&
							g_mouse.y >= outpos[1]+4 &&
							g_mouse.x <= m_pos[2] &&
							g_mouse.y <= m_pos[1])
						ie->curst = CU_MOVE;
					else
						ie->curst = CU_DEFAULT;
					
					ie->intercepted = true;
				}
			}
			else
			{
				//cursor out of window area?
				if(!ie->intercepted)
				{
					if(m_over)
					{
						//ie->intercepted = true;
						//ie->curst = CU_DEFAULT;
					}
				}
				//event intercepted but cursor in window rectangle (maybe covered up by something else)?
				else
				{
					// to do: this will be replaced by code in other
					//widgets that will set the cursor
					//g_curst = CU_DEFAULT;
				}
				
				m_over = false;
			}
		}
	}
}

void Win::subframe(float* fr)
{
	memcpy(fr, m_pos, sizeof(float)*4);
}


#endif