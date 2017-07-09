










#include "../sys/inc.h"
#include "../render/shader.h"
#include "draw2d.h"
#include "../sys/utils.h"
#include "../sys/syswin.h"

void drawim(unsigned int texi, float left, float top, float right, float bottom, float texleft, float textop, float texright, float texbottom, float *crop)
{
	glshader *s;
	float v[2*6], t[2*6], newleft, newtop, newright, newbottom, newtexleft, newtextop, newtexright, newtexbottom;

	if(crop[0] >= crop[2])
		return;
	if(crop[1] >= crop[3])
		return;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texi);
	s = &g_shader[g_cursh];
	glUniform1i(s->slot[SSLOT_TEXTURE0], 0);

	newleft = left;
	newtop = top;
	newright = right;
	newbottom = bottom;
	newtexleft = texleft;
	newtextop = textop;
	newtexright = texright;
	newtexbottom = texbottom;

	if(newleft < crop[0])
	{
		newtexleft = texleft+(crop[0]-left)*(texright-texleft)/(right-left);
		newleft = crop[0];
	}
	else if(newleft > crop[2])
		return;

	if(newright < crop[0])
		return;
	else if(newright > crop[2])
	{
		newtexright = texleft+(crop[2]-left)*(texright-texleft)/(right-left);
		newright = crop[2];
	}

	if(newtop < crop[1])
	{
		newtextop = textop+(crop[1]-top)*(texbottom-textop)/(bottom-top);
		newtop = crop[1];
	}
	else if(newtop > crop[3])
		return;

	if(newbottom < crop[1])
		return;
	else if(newbottom > crop[3])
	{
		newtexbottom = textop+(crop[3]-top)*(texbottom-textop)/(bottom-top);
		newbottom = crop[3];
	}

	v[0] = newleft;
	v[1] = newtop;
	v[2] = newright;
	v[3] = newtop;
	v[4] = newright;
	v[5] = newbottom;
	v[6] = newright;
	v[7] = newbottom;
	v[8] = newleft;
	v[9] = newbottom;
	v[10] = newleft;
	v[11] = newtop;

	t[0] = newtexleft;
	t[1] = newtextop;
	t[2] = newtexright;
	t[3] = newtextop;
	t[4] = newtexright;
	t[5] = newtexbottom;
	t[6] = newtexright;
	t[7] = newtexbottom;
	t[8] = newtexleft;
	t[9] = newtexbottom;
	t[10] = newtexleft;
	t[11] = newtextop;

#ifdef PLAT_GL14
	glVertexPointer(2, GL_FLOAT, 0, v);
	glTexCoordPointer(2, GL_FLOAT, 0, t);
#endif
	
#ifdef PLAT_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &v[0]);
	glVertexAttribPointer(s->slot[SSLOT_TEXCOORD0], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &t[0]);
#endif
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawsq(float r, float g, float b, float a, float left, float top, float right, float bottom, float *crop)
{
	glshader *s;
	float v[2*6], newleft, newtop, newright, newbottom;

	if(crop[0] >= crop[2])
		return;
	if(crop[1] >= crop[3])
		return;

	s = &g_shader[g_cursh];

	glUniform4f(s->slot[SSLOT_COLOR], r, g, b, a);

	newleft = left;
	newtop = top;
	newright = right;
	newbottom = bottom;

	if(newleft < crop[0])
		newleft = crop[0];
	else if(newleft > crop[2])
		return;

	if(newright < crop[0])
		return;
	else if(newright > crop[2])
		newright = crop[2];

	if(newtop < crop[1])
		newtop = crop[1];
	else if(newtop > crop[3])
		return;

	if(newbottom < crop[1])
		return;
	else if(newbottom > crop[3])
		newbottom = crop[3];

	v[0] = newleft;
	v[1] = newtop;
	v[2] = newright;
	v[3] = newtop;
	v[4] = newright;
	v[5] = newbottom;
	v[6] = newright;
	v[7] = newbottom;
	v[8] = newleft;
	v[9] = newbottom;
	v[10] = newleft;
	v[11] = newtop;

#ifdef PLAT_GL14
	glVertexPointer(2, GL_FLOAT, 0, v);
#endif
	
#ifdef PLAT_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &v[0]);
#endif
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

//2015/10/27 cropping now works correctly for lines
void drawl(float r, float g, float b, float a, float x1, float y1, float x2, float y2, float *crop)
{
	float v[2*2], dx, dy, slope;
	glshader *s;

	//ax+by+c=0

	if(crop[0] >= crop[2])
		return;
	if(crop[1] >= crop[3])
		return;

	dx = x2-x1;
	dy = y2-y1;
	slope = dy/dx;

	if(x1 < crop[0])
	{
		dx = crop[0] - x1;
		dy = dx * slope;
		x1 += dx;
		y1 += dy;
	}

	//TODO fix droplist closing on left mouse button release after using slider first time
	//TODO different currencies
	
	if(x2 < crop[0])
	{
		dx = crop[0] - x2;
		dy = dx * slope;
		x2 += dx;
		y2 += dy;
	}
	
	if(y1 < crop[1])
	{
		dy = crop[1] - y1;
		dx = dy / slope;
		x1 += dx;
		y1 += dy;
	}
	
	if(y2 < crop[1])
	{
		dy = crop[1] - y2;
		dx = dy / slope;
		x2 += dx;
		y2 += dy;
	}
	
	if(x1 > crop[2])
	{
		dx = x1 - crop[2];
		dy = dx * slope;
		x1 -= dx;
		y1 -= dy;
	}
	
	if(x2 > crop[2])
	{
		dx = x2 - crop[2];
		dy = dx * slope;
		x2 -= dx;
		y2 -= dy;
	}

	if(y1 > crop[3])
	{
		dy = y1 - crop[3];
		dx = dy / slope;
		x1 -= dx;
		y1 -= dy;
	}
	
	if(y2 > crop[3])
	{
		dy = y2 - crop[3];
		dx = dy / slope;
		x2 -= dx;
		y2 -= dy;
	}

	s = &g_shader[g_cursh];
	glUniform4f(s->slot[SSLOT_COLOR], r, g, b, a);

	v[0] = x1;
	v[1] = y1;
	v[2] = x2;
	v[3] = y2;

#ifdef PLAT_GL14
	glVertexPointer(2, GL_FLOAT, 0, v);
#endif
	
#ifdef PLAT_GLES20
	glVertexAttribPointer(s->slot[SSLOT_POSITION], 2, GL_FLOAT, GL_FALSE, sizeof(float)*0, &v[0]);
#endif
	
	glDrawArrays(GL_LINES, 0, 2);
}
