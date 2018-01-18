

#include "vpwrap.h"
#include "../gui.h"
#include "../draw2d.h"
#include "../../render/toxy.h"
#include "../../math/camf.h"
#include "../../math/3dmath.h"
#include "../../app/appmain.h"

void drawgrid(void *e, int xs, int ys, int ws, int hs, float *crop, v3f ax)
{
	glshader *s;
	int fpi = (int)e;
	v3f *v = &g_camf.view;
	float extx = PROJ_RIGHT*((float)ws / hs) / 1;
	float exty = PROJ_RIGHT / 1;
	v3f ex = { extx, exty, extx }, ey = { extx, exty, extx };
	v3f bx = { 1.0f - fabs(ax.x), 1.0f - fabs(ax.y), 1.0f - fabs(ax.z) };
	v3f vmin = {-ex.x * bx.x + v->x, -ex.y * bx.y + v->y, -ex.z * bx.z + v->z},
		vmax = { ey.x * bx.x + v->x, ey.y * bx.y + v->y, ey.z * bx.z + v->z };
	v3f l[2];
	float x, y, z;
	float base = 50.0f;
	int pow2 = log(g_zoom) / log(2.0f);
	float i = base / pow(2.0f, (float)pow2), j = i * 5.0f;
	v3f vs = { (int)(vmin.x / i)*i, (int)(vmin.y / i)*i, (int)(vmin.z / i)*i },
		ve = { (int)(vmax.x / i)*i, (int)(vmax.y / i)*i, (int)(vmax.z / i)*i };
	v3f vs2 = { (int)(vmin.x / j)*j, (int)(vmin.y / j)*j, (int)(vmin.z / j)*j },
		ve2 = { (int)(vmax.x / j)*j, (int)(vmax.y / j)*j, (int)(vmax.z / j)*j };
	v3f pos = { v->x - ax.x, v->y - ax.y, v->z - ax.z };
	v3f up = {(fabs(ax.y)==1.0f),(fabs(ax.y) != 1.0f),0.0f };
	v3f strafe = norm3f(cross3f(up,ax));
	float d[12];
	ofrust(extx, exty, *v, pos, up, strafe, MAX_DISTANCE, MIN_DISTANCE, d, gpv, gpl, gpld, gsa);

	s = g_shader + g_cursh;

	glUniform4f(s->slot[SSLOT_COLOR], 0.1f, 0.1f, 0.1f, 0.6f);
	glVertexPointer(3, GL_FLOAT, 0, l);

	for (x = vs.x; x <= ve.x; x += i)
	{
		for (y = vs.y; y <= ve.y; y += i)
		{
			for (z = vs.z; z <= ve.z; z += i)
			{
				l[0].x = x;
				l[0].y = y;
				l[0].z = z;
				l[0] = toclip(l[0]);
				glDrawArrays(GL_POINTS, 0, 1);
			}
		}
	}

	for (x = vs2.x; x <= ve2.x; x += j)
	{
		for (y = vs2.y; y <= ve2.y; y += j)
		{
			for (z = vs2.z; z <= ve2.z; z += j)
			{
				l[0].x = x - i/2.0f;
				l[0].y = y;
				l[0].z = z;
				l[0] = toclip(l[0]);
				l[1].x = x + i/2.0f;
				l[1].y = y;
				l[1].z = z;
				l[1] = toclip(l[1]);
				glDrawArrays(GL_LINES, 0, 2);

				l[0].x = x;
				l[0].y = y - i / 2.0f;
				l[0].z = z;
				l[0] = toclip(l[0]);
				l[1].x = x;
				l[1].y = y + i / 2.0f;
				l[1].z = z;
				l[1] = toclip(l[1]);
				glDrawArrays(GL_LINES, 0, 2);

				l[0].x = x;
				l[0].y = y;
				l[0].z = z - i / 2.0f;
				l[0] = toclip(l[0]);
				l[1].x = x;
				l[1].y = y;
				l[1].z = z + i / 2.0f;
				l[1] = toclip(l[1]);
				glDrawArrays(GL_LINES, 0, 2);
			}
		}
	}

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
	int fpi = (int)e;
	v3f ax = {fpi==0, fpi==1, fpi==2};

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
	switch (fpi)
	{
	case 0:
	case 1:
	case 2:
		drawgrid(e, x, y, w, h, crop, ax);
		break;
	case 3:
		break;
	}
	endsh();

	usesh(SH_COLOR3D);
	s = g_shader + g_cursh;
	switch (fpi)
	{
	case 0:
	case 1:
	case 2:
		break;
	case 3:
		drawscene();
		break;
	}
	endsh();

}

void szfp(wg* w)
{
	int fpi = (int)w->extra;
	int x = fpi % 2;
	int y = fpi / 2;

	w->pos[0] = 100 + x * (g_width - 100) / 2;
	w->pos[1] = 30 + y * (g_height - 30) / 2;
	w->pos[2] = 100 + (x+1) * (g_width - 100) / 2 - 1;
	w->pos[3] = 30 + (y+1) * (g_height - 30) / 2 - 1;
	//cenlab((bwg*)w);
}
