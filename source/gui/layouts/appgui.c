









#include "../../app/appmain.h"
#include "../gui.h"
#include "../widgets/image.h"
#include "../widgets/button.h"
#include "../vl.h"
#include "../widgets/link.h"
#include "vpwrap.h"
#include "../widgets/vp.h"
#include "../../math/3dmath.h"
#include "../widgets/dropmenu.h"
#include "../widgets/droplist.h"
#include "../../render/screenshot.h"

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
		logo = wgget(gui, "logo");
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
	bwg *b;
	b = (bwg*)w;
	w->pos[0] = 30;
	w->pos[1] = 30;
	w->pos[2] = 200;
	w->pos[3] = 100;
	cenlab(b, b->label, b->font, w->pos, b->tpos, dfalse);
}

void cng()
{
	wg *gui;
	g_appmode = APPMODE_PLAY;
	gui = &g_gui;
	wghideall(gui);
	c3finit(&g_camf);
}

void clg()
{
}

void cop()
{
	wg* gui;
	wg* menu;
	wg* ops;

	gui = (wg*)&g_gui;
	menu = wgget(gui, "menu");
	ops = wgget(gui, "ops");

	wghide(menu);
	wgshow(ops);
}

void ccs()
{
}

void cqu()
{
	g_quit = dtrue;
}

void ced()
{
	wg* gui;
	wg* menu;
	wg* edm;

	gui = (wg*)&g_gui;
	menu = wgget(gui, "menu");
	edm = wgget(gui, "ed");

	wghide(menu);
	wgshow(edm);

	g_appmode = APPMODE_EDITOR;

	c3finit(&g_camf);
}

void cjg()
{
}

void chg()
{
}

void sop()
{
	wg* gui;
	wg* ops;
	dlw *r, *b, *f;

	gui = (wg*)&g_gui;
	ops = wgget(gui, "ops");
	r = (dlw*)wgget(ops, "r");
	b = (dlw*)wgget(ops, "b");
	f = (dlw*)wgget(ops, "f");

	if (r->active >= 0)
	{
		sscanf(r->options[r->active], "%dx%d", &g_width, &g_height);
		g_selres.x = g_width;
		g_selres.y = g_height;
	}
	if (b->active >= 0)
	{
		sscanf(b->options[b->active], "%d", &g_bpp);
	}
	if (f->active >= 0)
	{
		g_fs = f->active;
	}

	g_appmode = APPMODE_RELOADING;
	savecfg();
}

void ces()
{
	wg* gui;
	wg* menu;
	wg* edm;
	wg* bgv;
	wg* ops;
	switch (g_appmode)
	{
	case APPMODE_PLAY:
		gui = (wg*)&g_gui;
		menu = wgget(gui, "menu");
		bgv = wgget(gui, "bg");
		wgshow(bgv);
		wgshow(menu);
		g_appmode = APPMODE_MENU;
		break;
	case APPMODE_EDITOR:
		gui = (wg*)&g_gui;
		menu = wgget(gui, "menu");
		edm = wgget(gui, "ed");
		bgv = wgget(gui, "bg");

		wghide(edm);
		wgshow(bgv);
		wgshow(menu);

		g_appmode = APPMODE_MENU;
		break;
	case APPMODE_MENU:
		gui = (wg*)&g_gui;
		menu = wgget(gui, "menu");

		if (menu->hidden)
		{
			edm = wgget(gui, "ed");
			ops = wgget(gui, "ops");
			wghide(edm);
			wghide(ops);
			wgshow(menu);
		}
		else
		{
			g_quit = dtrue;
		}
		break;
	}
}

void esz(wg* bw)
{
	bw->pos[0] = g_width / 6 - 10;
	bw->pos[1] = g_height / 2 - 120 + (int)bw->extra * 22;
	bw->pos[2] = bw->pos[0] + 200;
	bw->pos[3] = bw->pos[1] + 22;
}

void mm(inev* ie)
{
	if (g_appmode != APPMODE_PLAY)
		return;

	v3f v, u;
	v3f nv, nu, nr;

	v3fsub(&v, g_camf.view, g_camf.pos);
	v = norm3f(v);
	u = cross3f(g_camf.strafe, v);

	nv = rot3f(v, ie->dy / 20.0f, g_camf.strafe.x, g_camf.strafe.y, g_camf.strafe.z);
	nv = rot3f(nv, -ie->dx / 20.0f, u.x, u.y, u.z);

	nu = rot3f(u, ie->dy / 20.0f, g_camf.strafe.x, g_camf.strafe.y, g_camf.strafe.z);

	nr = rot3f(g_camf.strafe, -ie->dx / 20.0f, u.x, u.y, u.z);

	g_camf.strafe = nr;
	//g_camf.up = nu;
	g_camf.up = norm3f( cross3f(nr, nv) );
	nv = norm3f(cross3f(nu, nr));
	v3fadd(&g_camf.view, nv, g_camf.pos);
	g_camf.strafe = norm3f(cross3f(g_camf.up, nv));
	v3fmul(&g_camf.strafe, g_camf.strafe, -1);
}

void mfb()
{
	g_camf.f = dtrue;
}
void mfub()
{
	g_camf.f = dfalse;
}

void mbb()
{
	g_camf.b = dtrue;
}
void mbub()
{
	g_camf.b = dfalse;
}

void mlb()
{
	g_camf.l = dtrue;
}
void mlub()
{
	g_camf.l = dfalse;
}

void mrb()
{
	g_camf.r = dtrue;
}
void mrub()
{
	g_camf.r = dfalse;
}

void efree(wg* bw)
{
	free(bw->extra);
	bw->extra = NULL;
}

void makewg()
{
	wgg *gui;
	vl *logo, *loading, *menu, *ops, *bgv;
	bwg *but;
	imw *bg;
	hpl *ng, *lg, *jg, *hg, *op, *cs, *qu, *ed;
	int j;
	vl *edm;
	vp *edv[4];
	dwg *dw, *dw2, *dw3, *dw4, *dw5, *dw6,
		*dw7[20];
	dlw *dl[3];
	hpl *so, *eo;
	lnode *l;
	char s[12];
	v2i *v;
	char c;

	gui = &g_gui;
	wgginit(gui);

	gui->keydownfunc[SDL_SCANCODE_ESCAPE] = ces;
	gui->mmovef = mm;
	gui->keydownfunc[SDL_SCANCODE_W] = mfb;
	gui->keyupfunc[SDL_SCANCODE_W] = mfub;
	gui->keydownfunc[SDL_SCANCODE_S] = mbb;
	gui->keyupfunc[SDL_SCANCODE_S] = mbub;
	gui->keydownfunc[SDL_SCANCODE_A] = mlb;
	gui->keyupfunc[SDL_SCANCODE_A] = mlub;
	gui->keydownfunc[SDL_SCANCODE_D] = mrb;
	gui->keyupfunc[SDL_SCANCODE_D] = mrub;
	gui->keyupfunc[SDL_SCANCODE_F11] = savesc;

	logo = (vl*)malloc(sizeof(vl));
	loading = (vl*)malloc(sizeof(vl));
	menu = (vl*)malloc(sizeof(vl));
	ops = (vl*)malloc(sizeof(vl));
	bgv = (vl*)malloc(sizeof(vl));
	but = (bwg*)malloc(sizeof(bwg));
	bg = (imw*)malloc(sizeof(imw));
	ng = (hpl*)malloc(sizeof(hpl));
	lg = (hpl*)malloc(sizeof(hpl));
	jg = (hpl*)malloc(sizeof(hpl));
	hg = (hpl*)malloc(sizeof(hpl));
	op = (hpl*)malloc(sizeof(hpl));
	cs = (hpl*)malloc(sizeof(hpl));
	qu = (hpl*)malloc(sizeof(hpl));
	ed = (hpl*)malloc(sizeof(hpl));
	edm = (vl*)malloc(sizeof(vl));
	for (j = 0; j<4; ++j)
		edv[j] = (vp*)malloc(sizeof(vp));
	dw = (dwg*)malloc(sizeof(dwg));
	dw2 = (dwg*)malloc(sizeof(dwg));
	dw3 = (dwg*)malloc(sizeof(dwg));
	dw4 = (dwg*)malloc(sizeof(dwg));
	dw5 = (dwg*)malloc(sizeof(dwg));
	dw6 = (dwg*)malloc(sizeof(dwg));
	for(j=0; j<20; ++j)
		dw7[j] = (dwg*)malloc(sizeof(dwg));
	for (j = 0; j < 3; ++j)
		dl[j] = (dlw*)malloc(sizeof(dlw));
	so = (hpl*)malloc(sizeof(hpl));
	eo = (hpl*)malloc(sizeof(hpl));

	vwinit(logo, "logo", (wg*)gui);
	vwinit(loading, "loading", (wg*)gui);
	vwinit(menu, "menu", (wg*)gui);
	vwinit(bgv, "bg", (wg*)gui);
	imwinit(bg, bgv, "",
		"gui/bg.jpg", dtrue,
		szfs, 
		1, 1, 1, 1,
		0, 0, 1, 1);
	bwginit(but, menu, "",
		"", "label", "tooltip",
		MAINFONT16, BUST_LINE,
		szbut, NULL, NULL,
		NULL, NULL,
		NULL, -1, NULL);
	hplinit(ng, menu, "", "new game", MAINFONT16, esz, cng, (void*)0, NULL);
	hplinit(lg, menu, "", "load game", MAINFONT16, esz, clg, (void*)1, NULL);
	hplinit(jg, menu, "", "join game", MAINFONT16, esz, cjg, (void*)2, NULL);
	hplinit(hg, menu, "", "host game", MAINFONT16, esz, chg, (void*)3, NULL);
	hplinit(op, menu, "", "options", MAINFONT16, esz, cop, (void*)4, NULL);
	hplinit(cs, menu, "", "credits", MAINFONT16, esz, ccs, (void*)5, NULL);
	hplinit(qu, menu, "", "quit", MAINFONT16, esz, cqu, (void*)6, NULL);
	hplinit(ed, menu, "", "editor", MAINFONT16, esz, ced, (void*)7, NULL);
	vwinit(edm, "ed", (wg*)gui);
	for(j=0; j<4; ++j)
		vpinit(edv[j], edm, "vp", szfp, fpdraw, 
			NULL, NULL, fmm, NULL, NULL, NULL, NULL, NULL, (void*)j, NULL);
	dwginit(dw, edm, "", "File", MAINFONT16, -1, msz, NULL, NULL, NULL, NULL, dw7[0], dfalse, dfalse);
	dwginit(dw2, dw, "", "New", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, NULL, dw3, dfalse, dfalse);
	dwginit(dw3, dw, "", "Save as...", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw2, dw4, dfalse, dfalse);
	dwginit(dw4, dw, "", "Save", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw3, dw5, dfalse, dfalse);
	dwginit(dw5, dw, "", "Open", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw4, dw6, dfalse, dfalse);
	dwginit(dw6, dw, "", "Exit", MAINFONT16, -1, dwgsz, ces, NULL, NULL, dw5, NULL, dfalse, dfalse);
	dwginit(dw7[0], edm, "", "Add", MAINFONT16, -1, msz, NULL, NULL, NULL, dw, NULL, dfalse, dfalse);
	dwginit(dw7[1], dw7[0], "", "Brush", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, NULL, dw7[2], dfalse, dfalse);
	dwginit(dw7[2], dw7[0], "", "Object", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[1], NULL, dfalse, dfalse);

	dwginit(dw7[3], dw7[2], "", "Vehicles", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, NULL, dw7[4], dfalse, dfalse);
	dwginit(dw7[4], dw7[2], "", "Characters", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[3], NULL, dfalse, dfalse);

	dwginit(dw7[5], dw7[3], "", "Unity fighter", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, NULL, dw7[6], dfalse, dfalse);
	dwginit(dw7[6], dw7[3], "", "Carlyle tank", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[5], dw7[7], dfalse, dfalse);
	dwginit(dw7[7], dw7[3], "", "Outfield", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[6], dw7[8], dfalse, dfalse);
	dwginit(dw7[8], dw7[3], "", "Instinct", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[7], dw7[9], dfalse, dfalse);
	dwginit(dw7[9], dw7[3], "", "Ter", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[8], NULL, dfalse, dfalse);

	dwginit(dw7[10], dw7[4], "", "Battle computer", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, NULL, dw7[11], dfalse, dfalse);
	dwginit(dw7[11], dw7[4], "", "Pilot", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[10], dw7[12], dfalse, dfalse);
	dwginit(dw7[12], dw7[4], "", "Alien", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[11], dw7[13], dfalse, dfalse);
	dwginit(dw7[13], dw7[4], "", "Spacesuit", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[12], dw7[14], dfalse, dfalse);
	dwginit(dw7[14], dw7[4], "", "Zombie 1", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[13], dw7[15], dfalse, dfalse);
	dwginit(dw7[15], dw7[4], "", "Zombie 2", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[14], dw7[16], dfalse, dfalse);
	dwginit(dw7[16], dw7[4], "", "Zombie 3", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[15], dw7[17], dfalse, dfalse);
	dwginit(dw7[17], dw7[4], "", "Human 1", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[16], dw7[18], dfalse, dfalse);
	dwginit(dw7[18], dw7[4], "", "Human 2", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[17], dw7[19], dfalse, dfalse);
	dwginit(dw7[19], dw7[4], "", "Human 3", MAINFONT16, -1, dwgsz, NULL, NULL, NULL, dw7[18], NULL, dfalse, dfalse);

	vwinit(ops, "ops", (wg*)gui);
	dlwinit(dl[0], ops, "f", "Fullscreen", MAINFONT16, 0, esz, NULL);
	dlwinit(dl[1], ops, "r", "Resolution", MAINFONT16, 1, esz, NULL);
	dlwinit(dl[2], ops, "b", "Bit depth", MAINFONT16, 2, esz, NULL);
	hplinit(so, ops, "", "save", MAINFONT16, esz, sop, 3, NULL);
	hplinit(eo, ops, "", "exit", MAINFONT16, esz, ces, 4, NULL);
	dlwadd(dl[0], "No");
	dlwadd(dl[0], "Yes");
	dl[0]->active = g_fs;
	for (l = g_ress.head; l; l = l->next)
	{
		v = (v2i*)&l->data[0];
		sprintf(s, "%dx%d", v->x, v->y);
		fflush(g_applog);
		if (g_selres.x == v->x && g_selres.y == v->y)
			dl[1]->active = dl[1]->noptions;
		dlwadd(dl[1], s);
	}
	for (l = g_bpps.head; l; l = l->next)
	{
		c = *(char*)&l->data[0];
		sprintf(s, "%d", (int)c);
		if (g_bpp == c)
			dl[2]->active = dl[2]->noptions;
		dlwadd(dl[2], s);
	}

	wgadd((wg*)gui, (wg*)bgv);
	wgadd((wg*)bgv, (wg*)bg);
	wgadd((wg*)gui, (wg*)logo);
	wgadd((wg*)gui, (wg*)loading);
	wgadd((wg*)gui, (wg*)menu);
	wgadd((wg*)gui, (wg*)ops);
	wgadd((wg*)menu, (wg*)but);
	wgadd((wg*)menu, (wg*)ng);
	wgadd((wg*)menu, (wg*)lg);
	wgadd((wg*)menu, (wg*)jg);
	wgadd((wg*)menu, (wg*)hg);
	wgadd((wg*)menu, (wg*)op);
	wgadd((wg*)menu, (wg*)cs);
	wgadd((wg*)menu, (wg*)qu);
	wgadd((wg*)menu, (wg*)ed);
	wgadd((wg*)gui, (wg*)edm);
	for (j = 0; j<4; ++j)
		wgadd((wg*)edm, (wg*)edv[j]);
	wgadd((wg*)edm, (wg*)dw);
	wgadd((wg*)dw, (wg*)dw2);
	wgadd((wg*)dw, (wg*)dw3);
	wgadd((wg*)dw, (wg*)dw4);
	wgadd((wg*)dw, (wg*)dw5);
	wgadd((wg*)dw, (wg*)dw6);
	wgadd((wg*)edm, (wg*)dw7[0]);
	wgadd((wg*)dw7[0], (wg*)dw7[1]);
	wgadd((wg*)dw7[0], (wg*)dw7[2]);
	wgadd((wg*)dw7[2], (wg*)dw7[3]);
	wgadd((wg*)dw7[2], (wg*)dw7[4]);
	for (j = 5; j <= 9; ++j)
		wgadd((wg*)dw7[3], (wg*)dw7[j]);
	for (j = 10; j <= 19; ++j)
		wgadd((wg*)dw7[4], (wg*)dw7[j]);
	for (j = 0; j < 3; ++j)
		wgadd((wg*)ops, (wg*)dl[j]);
	wgadd((wg*)ops, (wg*)so);
	wgadd((wg*)ops, (wg*)eo);

	wghide((wg*)but);

	wghideall((wg*)gui);
	wgshow((wg*)logo);
}