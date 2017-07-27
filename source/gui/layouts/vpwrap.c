

#include "vpwrap.h"
#include "../gui.h"
#include "../draw2d.h"

int g_fpi[4] = { 0,1,2,3 };

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
}

void szfp(wg* w)
{
	int fpi = *(int*)w->extra;
	int x = fpi % 2;
	int y = fpi / 2;

	w->pos[0] = 100 + x * (g_width - 100) / 2 + 1;
	w->pos[1] = 30 + y * (g_height - 30) / 2 + 1;
	w->pos[2] = 100 + (x+1) * (g_width - 100) / 2 - 1;
	w->pos[3] = 30 + (y+1) * (g_height - 30) / 2 - 1;
	//cenlab((bwg*)w);
}
