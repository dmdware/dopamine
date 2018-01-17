

#include "vpwrap.h"
#include "../gui.h"
#include "../draw2d.h"
#include "../../render/toxy.h"

int g_fpi[4] = { 0,1,2,3 };

void drawgrid(void *e, int x, int y, int w, int h, float *crop)
{
	glshader *s;
	v3f vmin, vmax;
	float extentx = PROJ_RIGHT*((float)w/h) / 1;
	float extenty = PROJ_RIGHT / 1;
	v3f ex = { extentx, extenty, extentx }, ey = { extentx, extenty, extentx };
	v3fadd(&vmin, g_camf.view, ex);
	v3fadd(&vmax, g_camf.view, ey);
	v3f l[2];

	s = g_shader + g_cursh;

	l[0].x = -0.2f;
	l[0].y = 0.0f;
	l[0].z = 0.3f;
	l[1].x = 0.3f;
	l[1].y = 0.1f;
	l[1].z = 0.33f;

	l[0] = toclip(l[0]);
	l[1] = toclip(l[1]);

	glUniform4f(s->slot[SSLOT_COLOR], 0.0f, 1.0f, 0.0f, 1.0f);
	glVertexPointer(3, GL_FLOAT, 0, l);
	glDrawArrays(GL_LINES, 0, 2);

#if 0
	//draw only one layer of grid dots ...
	if (v->m_type == VIEWPORT_FRONT)	// ... on the x and y axises
	{
		vmin.z = 0;
		vmax.z = 0;
	}
	else if (v->m_type == VIEWPORT_LEFT)	// ... on the y and z axises
	{
		vmin.x = 0;
		vmax.x = 0;
	}
	else if (v->m_type == VIEWPORT_TOP)	// ... on the x and z axises
	{
		vmin.y = 0;
		vmax.y = 0;
	}
#endif

#if 0
	Shader* s = &g_shader[g_curS];
	glUniform4f(g_shader[g_curS].m_slot[SSLOT_COLOR], 0.5f, 0.5f, 1.0f, 1.0f);

	// draw x axis
	float xaxisverts[] = { vmin.x, 0.0f, 0.0f, vmax.x, 0.0f, 0.0f };
	//glVertexAttribPointer(s->m_slot[SSLOT_POSITION], 3, GL_FLOAT, GL_FALSE, 0, xaxisverts);
	glVertexPointer(3, GL_FLOAT, 0, xaxisverts);
#ifdef DEBUG
	CHECKGLERROR();
#endif
	glDrawArrays(GL_LINES, 0, 2);

	// draw y axis
	float yaxisverts[] = { 0.0f, vmin.y, 0.0f, 0.0f, vmax.y, 0.0f };
	//glVertexAttribPointer(s->m_slot[SSLOT_POSITION], 3, GL_FLOAT, GL_FALSE, 0, yaxisverts);
	glVertexPointer(3, GL_FLOAT, 0, yaxisverts);
#ifdef DEBUG
	CHECKGLERROR();
#endif
	glDrawArrays(GL_LINES, 0, 2);

	// draw z axis
	float zaxisverts[] = { 0.0f, 0.0f, vmin.z, 0.0f, 0.0f, vmax.z };
	//glVertexAttribPointer(s->m_slot[SSLOT_POSITION], 3, GL_FLOAT, GL_FALSE, 0, zaxisverts);
	glVertexPointer(3, GL_FLOAT, 0, zaxisverts);
#ifdef DEBUG
	CHECKGLERROR();
#endif
	glDrawArrays(GL_LINES, 0, 2);

	//float interval = (10.0f/g_zoom);
	//float interval = log10(g_zoom * 100.0f) * 10.0f;
	//float interval = (int)(1.0f/g_zoom/10.0f)*10.0f;

	//float base = PROJ_RIGHT/20*2;
	//float invzoom = base/g_zoom;
	//int power = log(invzoom)/log(base);
	//float interval = pow(base, power);

	//STOREY_HEIGHT=250
	//float baseinterval = STOREY_HEIGHT / 5.0f;
	//float screenfit = PROJ_RIGHT*2/g_zoom;
	//int scale =

	//-  float invzoom = 2.0f/g_zoom;
	//-  int tenpower = log(invzoom)/log(2);
	//-  float interval = pow(2, tenpower);
	//+
	//+  float base = PROJ_RIGHT/15*2;
	//+  float invzoom = base/g_zoom;
	//+  int power = log(invzoom)/log(base);
	//+  float interval = pow(base, power);

	float base = 50.0f;
	//float invzoom = base/g_zoom;
	//int power = log(invzoom)/log(base);
	//float interval = pow(base, power);

	float interval;

	// zoom 1 -> interval 50
	// zoom 0.5 -> interval = 100
	// zoom 0.25 -> interval = 200
	// zoom 0.125 -> interval = 400
	// zoom 0.0625 -> interval 800

	//if(power == 0)
	//if(g_zoom > 1.0f)
	{
		//	interval = invzoom;
		//interval = base / pow(2, (int)g_zoom-1);
		int power2 = log(g_zoom) / log(2.0f);
		interval = base / pow(2.0f, (float)power2);
	}

	// zoom 1 -> interval 50 = 50 / 1 = 50 / 2^0
	// zoom 2 -> interval 25 = 50 / 2 = 50 / 2^1
	// zoom 3 -> interval 12.5 = 50 / 4 = 50 / 2^2
	// zoom 4 -> interval 6.25 = 50 / 8 = 50 / 2^3
	// zoom 5 -> interval 3.125 = 50 / 16 = 50 / 2^4
	// wrong


	Vec3f start = Vec3f((int)(vmin.x / interval)*interval, (int)(vmin.y / interval)*interval, (int)(vmin.z / interval)*interval);
	Vec3f end = Vec3f((int)(vmax.x / interval)*interval, (int)(vmax.y / interval)*interval, (int)(vmax.z / interval)*interval);

	//dots
	glUniform4f(g_shader[g_curS].m_slot[SSLOT_COLOR], 0.3f, 0.3f, 0.3f, 1.0f);
	for (float x = start.x; x <= end.x; x += interval)
		for (float y = start.y; y <= end.y; y += interval)
			for (float z = start.z; z <= end.z; z += interval)
			{
				float point[] = { x, y, z };
				//glVertexAttribPointer(s->m_slot[SSLOT_POSITION], 3, GL_FLOAT, GL_FALSE, 0, point);
				glVertexPointer(3, GL_FLOAT, 0, point);
				glDrawArrays(GL_POINTS, 0, 1);
			}

	// crosses more spaced out
	float interval2 = interval * 5.0f;

	start = Vec3f((int)(vmin.x / interval2)*interval2, (int)(vmin.y / interval2)*interval2, (int)(vmin.z / interval2)*interval2);
	end = Vec3f((int)(vmax.x / interval2)*interval2, (int)(vmax.y / interval2)*interval2, (int)(vmax.z / interval2)*interval2);

	for (float x = start.x; x <= end.x; x += interval2)
		for (float y = start.y; y <= end.y; y += interval2)
			for (float z = start.z; z <= end.z; z += interval2)
			{
				float xline[] = { x - interval / 2.0f, y, z, x + interval / 2.0f, y, z };
				//glVertexAttribPointer(s->m_slot[SSLOT_POSITION], 3, GL_FLOAT, GL_FALSE, 0, xline);
				glVertexPointer(3, GL_FLOAT, 0, xline);
				glDrawArrays(GL_LINES, 0, 2);

				float yline[] = { x, y - interval / 2.0f, z, x, y + interval / 2.0f, z };
				//glVertexAttribPointer(s->m_slot[SSLOT_POSITION], 3, GL_FLOAT, GL_FALSE, 0, yline);
				glVertexPointer(3, GL_FLOAT, 0, yline);
				glDrawArrays(GL_LINES, 0, 2);

				float zline[] = { x, y, z - interval / 2.0f, x, y, z + interval / 2.0f };
				//glVertexAttribPointer(s->m_slot[SSLOT_POSITION], 3, GL_FLOAT, GL_FALSE, 0, zline);
				glVertexPointer(3, GL_FLOAT, 0, zline);
				glDrawArrays(GL_LINES, 0, 2);
			}
#endif
}

//four ports
void fpdraw(void *e, int x, int y, int w, int h)
{
	glshader* s;
	float crop[4];

	crop[0] = 0;
	crop[1] = 0;
	crop[2] = w;
	crop[3] = h;

	usesh(SH_COLOR2D);
	s = g_shader + g_cursh;
	glUniform1f(s->slot[SSLOT_WIDTH], (float)w);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)h);

	drawsq(0.7f, 0.7f, 0.7f, 0.5f, 0, 0, w, h, crop);

	endsh();

	flatview(w, h, 1, 1, 1, 1);
	s = g_shader + g_cursh;

	endsh();

	usesh(SH_GRID);
	s = g_shader + g_cursh;
	glUniform1f(s->slot[SSLOT_WIDTH], (float)w);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)h);

	drawgrid(e, x, y, w, h, crop);

	endsh();
}

void szfp(wg* w)
{
	int fpi = *(int*)w->extra;
	int x = fpi % 2;
	int y = fpi / 2;

	w->pos[0] = 100 + x * (g_width - 100) / 2;
	w->pos[1] = 30 + y * (g_height - 30) / 2;
	w->pos[2] = 100 + (x+1) * (g_width - 100) / 2 - 1;
	w->pos[3] = 30 + (y+1) * (g_height - 30) / 2 - 1;
	//cenlab((bwg*)w);
}
