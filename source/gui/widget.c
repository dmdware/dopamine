










#include "widget.h"
#include "gui.h"
#include "font.h"
#include "../sys/syswin.h"
#include "icon.h"
#include "../sys/debug.h"
#include "widgets/button.h"
#include "widgets/image.h"
#include "widgets/text.h"

void (*wgsubdraw[WIDGETS]) (wg* bw);
void (*wgsubdrawover[WIDGETS]) (wg* bw);
void (*wgsubinev[WIDGETS]) (wg *bw, inev* ie);

void wginits()
{
	wgsubdraw[WIDGET_GUI] = NULL;
	wgsubdraw[WIDGET_IMAGE] = imwdraw;
	wgsubdraw[WIDGET_EDITBOX] = NULL;
	wgsubdraw[WIDGET_BUTTON] = bwgdraw;
	wgsubdraw[WIDGET_VIEWLAYER] = NULL;
	wgsubdraw[WIDGET_TEXT] = twgdraw;

	wgsubdrawover[WIDGET_GUI] = NULL;
	wgsubdrawover[WIDGET_IMAGE] = NULL;
	wgsubdrawover[WIDGET_EDITBOX] = NULL;
	wgsubdrawover[WIDGET_BUTTON] = bwgdrawover;
	wgsubdrawover[WIDGET_VIEWLAYER] = NULL;
	wgsubdrawover[WIDGET_TEXT] = NULL;

	wgsubinev[WIDGET_GUI] = wgginev;
	wgsubinev[WIDGET_IMAGE] = NULL;
	wgsubinev[WIDGET_EDITBOX] = NULL;
	wgsubinev[WIDGET_BUTTON] = bwginev;
	wgsubinev[WIDGET_VIEWLAYER] = NULL;
	wgsubinev[WIDGET_TEXT] = NULL;
}

void wginit(wg* w)
{
	w->parent = NULL;
	w->name[0] = 0;
	w->opened = ecfalse;
	w->ldown = ecfalse;
	w->reframefunc = NULL;
	w->hidden = ecfalse;
	w->extra = NULL;

	linit(&w->sub);
}

void wgfree(wg *w)
{
	wgfreech(w);
	lfree(&w->sub);
	free(w->extra);
	w->extra = NULL;

	switch(w->type)
	{
	//case WIDGET_EDITBOX:
		//editbox_free((editbox*)w);
	//	break;
	}
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

	if(w->reframefunc)
		w->reframefunc(w);

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

	if (wgsubdraw[w->type])
		wgsubdraw[w->type](w);

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

	if (wgsubdrawover[w->type])
		wgsubdrawover[w->type](w);

	for(i=w->sub.head; i; i=i->next)
	{
		iw = *(wg**)&i->data[0];

		if(iw->hidden)
			continue;

		wgdrawover(iw);
	}
}

void wginev(wg *w, inev* ie)
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
		
		wginev(iw, ie);
	}

	if (wgsubinev[w->type])
		wgsubinev[w->type](w, ie);
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
	w->hidden = ectrue;
	wglosefocus(w);
}

void wgshow(wg *w)
{
	w->hidden = ecfalse;
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
