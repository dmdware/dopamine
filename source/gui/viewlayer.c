










#include "gui.h"
#include "../sys/texture.h"
#include "viewlayer.h"

void vwinit(vw *v, const char* n, wg* parent)
{
	wg *bw;
	bw = (wg*)v;
	wginit(bw);
	strcpy(bw->name, n);
	bw->opened = ecfalse;
	bw->type = WIDGET_VIEWLAYER;
	bw->parent = parent;
	bw->hidden = ectrue;
	bw->reframef = szfs;
	wgreframe(bw);
}
