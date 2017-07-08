









#include "../../app/appmain.h"
#include "../../gui/gui.h"
#include "../../gui/widgets/image.h"
#include "../../gui/widgets/button.h"
#include "../../gui/viewlayer.h"

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
	
	minsz = fmin((float)g_width-1, (float)g_height-1);

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


void makewg()
{
	wg *gui;
	vw *logo, *loading, *menu;
	bwg *but;

	gui = (wg*)&g_gui;
	wgginit((wgg*)gui);

	logo = (vw*)malloc(sizeof(vw));
	loading = (vw*)malloc(sizeof(vw));
	menu = (vw*)malloc(sizeof(vw));
	but = (bwg*)malloc(sizeof(bwg));

	vwinit(logo, "logo", gui);
	vwinit(loading, "loading", gui);
	vwinit(menu, "menu", gui);
	bwginit(but, menu, "",
		"", "label", "tooltip",
		MAINFONT8, BUST_LINEBASED,
		szbut, NULL, NULL,
		NULL, NULL,
		NULL, -1, NULL);

	wgadd(gui, (wg*)logo);
	wgadd(gui, (wg*)loading);
	wgadd(gui, (wg*)menu);
	wgadd(menu, (wg*)but);

	wghideall(gui);
	wgshow((wg*)logo);
}