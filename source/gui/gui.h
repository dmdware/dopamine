










#ifndef GUI_H
#define GUI_H

#include "../sys/includes.h"
#include "widget.h"

struct wgg
{
	wg base;
	void (*keyupfunc[SDL_NUM_SCANCODES])();
	void (*keydownfunc[SDL_NUM_SCANCODES])();
	void (*anykeyupfunc)(int k);
	void (*anykeydownfunc)(int k);
	void (*mousemovefunc)(inev* ie);
	void (*lbuttondownfunc)();
	void (*lbuttonupfunc)();
	void (*rbuttondownfunc)();
	void (*rbuttonupfunc)();
	void (*mbuttondownfunc)();
	void (*mbuttonupfunc)();
	void (*mousewheelfunc)(int delta);
};

typedef struct wgg wgg;

void wgginit(wgg* gui);
void wggdraw2(wgg* gui);
void wgginev(wg* bw, inev* ie);

extern wgg g_gui;

dbool mousepos();
void cenmouse();
void setstat(const char* status);
void flatview(int width, int height, float r, float g, float b, float a);

#endif