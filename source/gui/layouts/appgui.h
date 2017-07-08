










#ifndef APPGUI_H
#define APPGUI_H

#include "../../sys/includes.h"
#include "../widget.h"

struct vw;
struct wg;

extern char g_lastsave[DMD_MAX_PATH+1];

void szfs(wg* w);
void szfssq(wg* w);
void szapplogo(wg* w);
void szapptit(wg* w);
void makewg();
void skiplogo();
void uplogo();

#endif	//GwggH
