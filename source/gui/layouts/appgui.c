









#include "../../app/appmain.h"
#include "../../gui/gui.h"
#include "../../gui/widgets/image.h"
#include "../../gui/widgets/button.h"
#include "../../gui/viewlayer.h"
#include "../../gui/widgets/link.h"

char g_lastsave[DMD_MAX_PATH+1];

static char g_restage = 0;

void skiplogo()
{
	wg *gui, *load;

	if(g_appmode != APPMODE_LOGO)
		return;

	g_appmode = APPMODE_LOADING;return;
	gui = (wg*)&g_gui;

	wghide( gui );
	load = wgget( gui, "loading" );
	wgshow( load );
}

void uplogo()
{
	wg *gui, *logo;
	imw *logo2;
	float a;
	static int stage = 0;

	skiplogo();
	return;

	gui = (wg*)&g_gui;

	if(stage < 60)
	{
		a = (float)stage / 60.0f;
		logo = wgget( gui, "logo");
		logo2 = (imw*)wgget( logo, "logo");
		logo2->rgba[3] = a;
	}
	else if(stage < 120)
	{
		a = 1.0f - (float)(stage-60) / 60.0f;
		logo = wgget( gui, "logo");
		logo2 = (imw*)wgget( logo, "logo");
		logo2->rgba[3] = a;
	}
	else
		skiplogo();

	stage++;
}

void szfs(wg* w)
{
	w->pos[0] = 0;
	w->pos[1] = 0;
	w->pos[2] = (float)g_width;
	w->pos[3] = (float)g_height;
}

void szfssq(wg* w)
{
	float minsz;
	
	minsz = minf((float)g_width-1, (float)g_height-1);

	w->pos[0] = g_width/2.0f - minsz/2.0f;
	w->pos[1] = g_height/2.0f - minsz/2.0f;
	w->pos[2] = g_width/2.0f + minsz/2.0f;
	w->pos[3] = g_height/2.0f + minsz/2.0f;
}

void szapplogo(wg* w)
{
	w->pos[0] = 30;
	w->pos[1] = 30;
	w->pos[2] = 200;
	w->pos[3] = 200;
}

void szapptit(wg* w)
{
	w->pos[0] = 30;
	w->pos[1] = 30;
	w->pos[2] = (float)g_width-1;
	w->pos[3] = 100;
}

void szbut(wg* w)
{
	w->pos[0] = 30;
	w->pos[1] = 30;
	w->pos[2] = 200;
	w->pos[3] = 100;
	cenlab((bwg*)w);
}

void cng()
{
}

void clg()
{
}

void cop()
{
}

void ccs()
{
}

void cqu()
{
	g_quit = ectrue;
}

void ced()
{
}

void esz(wg* bw)
{
	bw->pos[0] = g_width / 6 - 10;
	bw->pos[1] = g_height / 2 - 120 + *(int*)bw->extra * 22;
	bw->pos[2] = bw->pos[0] + 100;
	bw->pos[3] = bw->pos[1] + 22;
}

void efree(wg* bw)
{
	free(bw->extra);
	bw->extra = NULL;
}

void makewg()
{
	wg *gui;
	vw *logo, *loading, *menu;
	bwg *but;
	imw *bg;
	hpl *ng, *lg, *op, *cs, *qu, *ed;
	int *i[6], j;

	for (j = 0; j < 6; ++j)
	{
		i[j] = (int*)malloc(sizeof(int));
		*(i[j]) = j;
	}
	gui = (wg*)&g_gui;
	wgginit((wgg*)gui);

	logo = (vw*)malloc(sizeof(vw));
	loading = (vw*)malloc(sizeof(vw));
	menu = (vw*)malloc(sizeof(vw));
	but = (bwg*)malloc(sizeof(bwg));
	bg = (imw*)malloc(sizeof(imw));
	ng = (hpl*)malloc(sizeof(hpl));
	lg = (hpl*)malloc(sizeof(hpl));
	op = (hpl*)malloc(sizeof(hpl));
	cs = (hpl*)malloc(sizeof(hpl));
	qu = (hpl*)malloc(sizeof(hpl));
	ed = (hpl*)malloc(sizeof(hpl));

	vwinit(logo, "logo", gui);
	vwinit(loading, "loading", gui);
	vwinit(menu, "menu", gui);
	imwinit(bg, menu, "",
		"gui/bg.jpg", ectrue,
		szfs, 
		1, 1, 1, 1,
		0, 0, 1, 1);
	bwginit(but, menu, "",
		"", "label", "tooltip",
		MAINFONT16, BUST_LINEBASED,
		szbut, NULL, NULL,
		NULL, NULL,
		NULL, -1, NULL);
	hplinit(ng, menu, "", "new game", MAINFONT16, esz, cng, i[0], efree);
	hplinit(lg, menu, "", "load game", MAINFONT16, esz, clg, i[1], efree);
	hplinit(op, menu, "", "options", MAINFONT16, esz, cop, i[2], efree);
	hplinit(cs, menu, "", "credits", MAINFONT16, esz, ccs, i[3], efree);
	hplinit(qu, menu, "", "quit", MAINFONT16, esz, cqu, i[4], efree);
	hplinit(ed, menu, "", "editor", MAINFONT16, esz, ced, i[5], efree);

	wgadd(gui, (wg*)logo);
	wgadd(gui, (wg*)loading);
	wgadd(gui, (wg*)menu);
	wgadd(menu, (wg*)bg);
	wgadd(menu, (wg*)but);
	wgadd(menu, (wg*)ng);
	wgadd(menu, (wg*)lg);
	wgadd(menu, (wg*)op);
	wgadd(menu, (wg*)cs);
	wgadd(menu, (wg*)qu);
	wgadd(menu, (wg*)ed);

	wghide((wg*)but);

	wghideall(gui);
	wgshow((wg*)logo);
}