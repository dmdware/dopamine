










#include "wg.h"
#include "gui.h"
#include "font.h"
#include "../sys/syswin.h"
#include "icon.h"
#include "../sys/debug.h"
#include "widgets/button.h"
#include "widgets/image.h"
#include "widgets/text.h"
#include "widgets/link.h"
#include "widgets/vp.h"

void(*wgsubd[WIDGETS]) (wg* bw) = {NULL,imwdraw,NULL,bwgdraw,NULL,twgdraw,hpldraw,vpdraw};
void(*wgsubdo[WIDGETS]) (wg* bw) = { NULL,NULL,NULL,bwgdrawover,NULL,NULL,NULL,NULL};
void(*wgsubin[WIDGETS]) (wg *bw, inev* ie) = {wggin,NULL,NULL,bwgin,NULL,NULL,hplin,vpin};
void(*wgsubf[WIDGETS]) (wg *bw) = {NULL,NULL,NULL,NULL,NULL,NULL,hplfree,vpfree};

void wginit(wg* w)
{
	w->parent = NULL;
	w->name[0] = 0;
	w->opened = dfalse;
	w->reframef = NULL;
	w->hidden = dfalse;
	w->extra = NULL;

	linit(&w->sub);
}

void wgfree(wg *w)
{
	wgfreech(w);
	lfree(&w->sub);
	if (wgsubf[w->type])
		wgsubf[w->type](w);
}

void wgchcall(wg *w, wg* ch, char type, void* data)
{
	/* TODO window scroll-bars call */
}

void wghideall(wg *w)
{
	lnode *i;
	wg *iw;

	for(i=w->sub.head; i; i=i->next)
	{
		iw = *(wg**)&i->data[0];
		wghide(iw);
	}
}

void wgframeup(wg *w)
{
	lnode *i;
	wg *iw;

	for(i=w->sub.head; i; i=i->next)
	{
		iw = *(wg**)&i->data[0];
		wgframeup(iw);
	}
}

void wgreframe(wg *w)	//resized or moved
{
	lnode *i;
	wg *iw;

	if(w->reframef)
		w->reframef(w);
	else
	{
		w->pos[0] = 0;
		w->pos[1] = 0;
		w->pos[2] = (float)g_width - 1;
		w->pos[3] = (float)g_height - 1;
	}

	if(w->parent)
	{
		subcrop(w->parent->crop, w->pos, w->crop);
	}
	else
	{
		w->crop[0] = 0;
		w->crop[1] = 0;
		w->crop[2] = (float)g_width-1;
		w->crop[3] = (float)g_height-1;
	}

	for(i=w->sub.head; i; i=i->next)
	{
		iw = *(wg**)&i->data[0];
		wgreframe(iw);
	}
}

void wgdraw(wg *w)
{
	lnode *i;
	wg *iw;

	if (wgsubd[w->type])
		wgsubd[w->type](w);

	for(i=w->sub.head; i; i=i->next)
	{
		iw = *(wg**)&i->data[0];

		if(iw->hidden)
			continue;

		wgdraw(iw);
	}
	
	switch(w->type)
	{
	case WIDGET_GUI:
		wggdraw2((wgg*)w);
		break;
	}
}

void wgdrawover(wg *w)
{	
	lnode *i;
	wg *iw;

	if (wgsubdo[w->type])
		wgsubdo[w->type](w);

	for(i=w->sub.head; i; i=i->next)
	{
		iw = *(wg**)&i->data[0];

		if(iw->hidden)
			continue;

		wgdrawover(iw);
	}
}

void wgin(wg *w, inev* ie)
{
	lnode *i;
	wg *iw;
	dbool intercepted;
	
	intercepted = ie->intercepted;
	
	i = w->sub.tail;
	/* safe, may shift during call */
	while(i)
	{
		iw = *(wg**)&i->data[0];
		i = i->prev;	/* safe, may shift during call */

		if(iw->hidden)
			continue;
		
		wgin(iw, ie);
	}

	if (wgsubin[w->type])
		wgsubin[w->type](w, ie);
}

void wgtofront(wg *w)
{
	lnode *i;
	wg *iw;
	list *parsub;

	if(!w->parent)
		return;

	parsub = &w->parent->sub;

	for(i=parsub->head; i; i=i->next)
	{
		iw = *(wg**)&i->data[0];

		if(iw == w)
		{
			lerase(parsub, i);
			lpub2(parsub, sizeof(wg*), &iw);
			return;
		}
	}
}

wg* wgget(wg *w, const char* name)
{
	lnode *i;
	wg *iw;

	for(i=w->sub.head; i; i=i->next)
	{
		iw = *(wg**)&i->data[0];
		if(!strcmp(iw->name, name))
			return iw;
	}

	return NULL;
}

void wgadd(wg *w, wg *newg)
{
	NOMEM(newg);
	lpub2(&w->sub, sizeof(wg*), &newg);
}

void wggainfocus(wg *w)
{
}

void wglosefocus(wg *w)
{
	lnode *i;
	wg *iw;

	/* TODO lose focus win blview members edit boxes */

	for(i=w->sub.head; i; i=i->next)
	{
		iw = *(wg**)&i->data[0];
		wglosefocus(iw);
	}
}

void wghide(wg *w)
{
	w->hidden = dtrue;
	wglosefocus(w);
}

void wgshow(wg *w)
{
	w->hidden = dfalse;
	/* necessary for window wgs: */
	//tofront();	//can't break list iterator, might shift

	//for(std::list<wg*>::iterator i=sub.begin(); i!=sub.end(); i++)
	//	(*i)->show();
}

/* free subwg children */
void wgfreech(wg *w)
{
	lnode *i;
	wg *iw;

	i = w->sub.head;
	while(i)
	{
		iw = *(wg**)&i->data[0];
		wgfree(iw);
		free(iw);
		iw = NULL;
		i = i->next;
	}
	lfree(&w->sub);
}

void subcrop(float *src1, float *src2, float *dest)
{
	dest[0] = ((src1[0]>src2[0])?src1[0]:src2[0]);
	dest[1] = ((src1[1]>src2[1])?src1[1]:src2[1]);
	dest[2] = ((src1[2]<src2[2])?src1[2]:src2[2]);
	dest[3] = ((src1[3]<src2[3])?src1[3]:src2[3]);
}
