










#include "gui.h"
#include "../sys/texture.h"
#include "vl.h"

void vwinit(vl *v, const char* n, wg* parent)
{
	wg *bw;
	bw = (wg*)v;
	wginit(bw);
	strcpy(bw->name, n);
	bw->type = WG_VIEWLAYER;
	bw->parent = parent;
	bw->hidden = dtrue;
	bw->reframef = szfs;
	wgreframe(bw);
}
