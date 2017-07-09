



#ifndef SYSWIN_H
#define SYSWIN_H

#include "inc.h"
#include "../math/v2i.h"
#include "../math/camf.h"
#include "../algo/vec.h"
#include "../algo/list.h"
#include "../gui/cursor.h"

#define INI_WIDTH			640
#define INI_HEIGHT			480
#define INI_BPP				32
#define DRAW_FRAME_RATE		30
#define SIM_FRAME_RATE		30
#define MIN_DISTANCE		1
#define MAX_DISTANCE		(5 * 1000 * 10)
#define FIELD_OF_VIEW		90
#define PROJ_RIGHT			600
#define MIN_ZOOM		0.05f
#define MAX_ZOOM		0.7f
#define INI_ZOOM			0.05f

extern dbool g_quit;
extern dbool g_bg;
extern dbool g_active;
extern dbool g_fs;
extern double g_indrawfps;
extern double g_inupfps;
extern double g_upiv;
extern double g_drawiv;

extern v2i g_selres;
extern list g_ress;
extern list g_bpps;

#ifndef MATCHMAKER
extern c3f g_camf;
extern int g_currw;
extern int g_currh;
extern int g_width;
extern int g_height;
extern int g_bpp;
extern v2i g_mouse;
extern v2i g_mousestart;
extern dbool g_keyin;
extern dbool g_keys[SDL_NUM_SCANCODES];
extern dbool g_mousekeys[5];
extern float g_zoom;
extern int g_curst;	/* cursor state */
extern int g_kbfocus;	/* keyboad focus counter */
extern dbool g_mouseout;
#endif

void calcdrawrate();
dbool drawnext();
void calcuprate();
dbool upnext();
void enumdisp();
void resize(int width, int height);
void breakwin(const char* title);
dbool makewin(const char* title);

#endif
