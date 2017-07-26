







#include "../wg.h"
#include "image.h"
#include "text.h"
#include "../../sys/debug.h"

void imwinit(imw *i,
				wg* parent, 
				const char* nm, 
				const char* filepath, 
				dbool clamp, 
				void (*reframef)(wg* w), 
				float r, 
				float g, 
				float b, 
				float a, 
				float texleft, 
				float textop, 
				float texright, 
				float texbottom)
{
	wg *bw;

	bw = (wg*)i;
	wginit(bw);

	bw->parent = parent;
	bw->type = WIDGET_IMAGE;
	strcpy(bw->name, nm);
	createtex(&i->texi, filepath, clamp, dfalse, dfalse);
	bw->reframef = reframef;
	i->texc[0] = texleft;
	i->texc[1] = textop;
	i->texc[2] = texright;
	i->texc[3] = texbottom;
	i->rgba[0] = r;
	i->rgba[1] = g;
	i->rgba[2] = b;
	i->rgba[3] = a;
	bw->pos[0] = 0;
	bw->pos[1] = 0;
	bw->pos[2] = 0;
	bw->pos[3] = 0;
	wgreframe(bw);
}

void imwdraw(wg *bw)
{
	glshader *s;
	imw *i;
	gltex *tex;

	i = (imw*)bw;
	s = g_shader+g_cursh;
	tex = g_tex+i->texi;

	glUniform4fv(s->slot[SSLOT_COLOR], 1, i->rgba);
	drawim(tex->texname,
		bw->pos[0], bw->pos[1], bw->pos[2], bw->pos[3],
		i->texc[0], i->texc[1], i->texc[2], i->texc[3], 
		bw->crop);
	glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);
}

