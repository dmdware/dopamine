










#include "gui.h"
#include "../render/shader.h"
#include "../sys/texture.h"
#include "../app/appmain.h"
#include "vl.h"
#include "widgets/text.h"

wgg g_gui;

void wgginit(wgg* gui)
{
	wg *w;
	int i;

	w = (wg*)gui;

	wginit(w);

	w->type = WIDGET_GUI;

	for(i=0; i<SDL_NUM_SCANCODES; ++i)
	{
		gui->keyupfunc[i] = NULL;
		gui->keydownfunc[i] = NULL;
	}

	gui->anykeyupfunc = NULL;
	gui->anykeydownfunc = NULL;
	gui->lbuttondownfunc = NULL;
	gui->lbuttonupfunc = NULL;
	gui->rbuttondownfunc = NULL;
	gui->rbuttonupfunc = NULL;
	gui->mbuttondownfunc = NULL;
	gui->mbuttonupfunc = NULL;
	gui->mousewheelfunc = NULL;
	gui->mmovef = NULL;

	w->hidden = dfalse;
}

void wggdraw2(wgg *gui)
{
	unsigned int spi;
	spt* sp;
	wg *bw;

	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	CHECKGL();
	flatview(g_width, g_height, 1, 1, 1, 1);
	CHECKGL();

	/* TODO
	in wg::
	wgdraw((wg*)gui);
	CHECKGL();
	wgdrawover((wg*)gui);
	*/

	if(g_appmode == APPMODE_PLAY &&
		g_keys[SDL_SCANCODE_TAB])
	{
//		Lobby_DrawPyL();
//		Lobby_DrawState();
	}

	spi = g_cursor[g_curst];
	sp = &g_sp[spi];

	bw = (wg*)gui;

	bw->crop[0] = 0;
	bw->crop[1] = 0;
	bw->crop[2] = (float)g_width-1;
	bw->crop[3] = (float)g_height-1;

	drawim(g_tex[sp->difftexi].texname, 
		g_mouse.x+sp->offset[0], g_mouse.y+sp->offset[1], 
		g_mouse.x+sp->offset[2], g_mouse.y+sp->offset[3], 
		0,0,1,1, bw->crop);
	
	CHECKGL();

	endsh();
	CHECKGL();

	glEnable(GL_DEPTH_TEST);
}

void wggin(wg *bw, inev* ie)
{
	wgg *gui;

	gui = (wgg*)bw;

	if(!ie->intercepted)
	{
		if(ie->type == INEV_MOUSEMOVE && gui->mmovef) gui->mmovef(ie);
		else if(ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_LEFT && gui->lbuttondownfunc) gui->lbuttondownfunc();
		else if(ie->type == INEV_MOUSEUP && ie->key == MOUSE_LEFT && gui->lbuttonupfunc) gui->lbuttonupfunc();
		else if(ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_MIDDLE && gui->mbuttondownfunc) gui->mbuttondownfunc();
		else if(ie->type == INEV_MOUSEUP && ie->key == MOUSE_MIDDLE && gui->mbuttonupfunc) gui->mbuttonupfunc();
		else if(ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_RIGHT && gui->rbuttondownfunc) gui->rbuttondownfunc();
		else if(ie->type == INEV_MOUSEUP && ie->key == MOUSE_RIGHT && gui->rbuttonupfunc) gui->rbuttonupfunc();
		else if(ie->type == INEV_MOUSEWHEEL && gui->mousewheelfunc) gui->mousewheelfunc(ie->amount);
		else if(ie->type == INEV_KEYDOWN && gui->keydownfunc[ie->scancode]) gui->keydownfunc[ie->scancode]();
		else if(ie->type == INEV_KEYUP && gui->keyupfunc[ie->scancode]) gui->keyupfunc[ie->scancode]();
		else if(ie->type == INEV_COPY && gui->keydownfunc[ie->scancode]) gui->keydownfunc[ie->scancode]();
		else if(ie->type == INEV_PASTE && gui->keydownfunc[ie->scancode]) gui->keydownfunc[ie->scancode]();

		if(ie->type != INEV_MOUSEMOVE && gui->anykeydownfunc) gui->anykeydownfunc(-1);
	}
}

void setstat(const char* status)
{
	wg *gui, *loading;
	twg *stat;

	gui = (wg*)&g_gui;

	loading = wgget(gui, "loading");

	if(!loading)
		return;

	stat = (twg*)wgget(loading, "status");

	if(!stat)
		return;

	free(stat->text);
	stat->text = (char*)malloc(strlen(status)+1);
	strcpy(stat->text, status);
}

dbool mousepos()
{
	v2i old = g_mouse;
	SDL_GetMouseState(&g_mouse.x, &g_mouse.y);

	if(g_mouse.x == old.x && g_mouse.y == old.y)
		return dfalse;

	return dtrue;
}

void cenmouse()
{
	g_mouse.x = g_width/2;
	g_mouse.y = g_height/2;
	
	SDL_WarpMouseInWindow(g_win, g_mouse.x, g_mouse.y);
}

void flatview(int width, int height, float r, float g, float b, float a)
{
	glshader* s;
	CHECKGL();
	usesh(SH_ORTHO);
	s = g_shader+g_cursh;
	glUniform1f(s->slot[SSLOT_WIDTH], (float)width);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)height);
	glUniform4f(s->slot[SSLOT_COLOR], r, g, b, a);
	g_currw = width;
	g_currh = height;
	CHECKGL();
}
