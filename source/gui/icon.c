#include "icon.h"
#include "../sys/texture.h"

icot g_icon[ICONS];

void icot_init(icot *i)
{
	i->tag[0] = 0;
}

void icot_free(icot *i)
{
	i->tag[0] = 0;
}

void defi(int type, char *relative, char *tag)
{
	icot *i;
	gltex *t;
	
	i = &g_icon[type];

	strcpy(i->tag, tag);
	createtex(&i->texi, relative, dtrue, dfalse, dfalse);
	t = &g_tex[i->texi];
	i->w = t->width;
	i->h = t->height;
}
