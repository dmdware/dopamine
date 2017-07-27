

#include "vpwrap.h"
#include "../gui.h"
#include "../draw2d.h"

int g_fpi[4] = { 1,2,3,4 };

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

	drawsq(0.7f, 0.7f, 0.7f, 0.5f, 20, 20, 500, 500, crop);

	endsh();

	flatview(w, h, 1, 1, 1, 1);
	s = g_shader + g_cursh;

	endsh();
}

void szfp(wg* w)
{
	w->pos[0] = 30;
	w->pos[1] = 30;
	w->pos[2] = 200;
	w->pos[3] = 100;
	//cenlab((bwg*)w);
}
