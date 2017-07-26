










#ifndef WG_H
#define WG_H

#include "../sys/utils.h"
#include "../sys/texture.h"
#include "font.h"
#include "../render/shader.h"
#include "../sys/syswin.h"
#include "draw2d.h"
#include "richtext.h"
#include "inev.h"

#define MAX_OPTIONS_SHOWN	7

#define WIDGET_GUI					0
#define WIDGET_IMAGE				1
#define WIDGET_EDITBOX				2
#define WIDGET_BUTTON				3
#define WIDGET_VIEWLAYER			4
#define WIDGET_TEXT					5
#define WIDGET_LINK					6
#define WIDGET_VIEWPORT				7
#define WIDGETS						8

struct wg
{
	/* TODO trim members delegate to sub's */

	char type;
	struct wg* parent;
	float pos[4];
	float crop[4];

	char name[16];

	dbool opened;
	dbool hidden;

	list sub;	/* wg* */
	void* extra;	/* extra user params */

	/*
	float texc[4];	//texture coordinates
	float tpos[4];	//text pos
	float scar[4];	//scrolling area rect for windows
	unsigned int texi;
	unsigned int bgtex;
	unsigned int bgovertex;
	RichText text;
	int font;
	unsigned int frametex, filledtex, uptex, downtex;
	vector options; */ /* RichText */
	/*
	int selected;
	float scroll[2];
	dbool mousescroll;
	float vel[2];
	int param;
	float rgba[4];
	char* value; /* /* RichText */
	/* int caret;
	dbool passw;
	int maxlen;
	dbool shadow;
	int lines;
	RichText label;
	dbool popup;

	void (*clickf)();
	void (*clickfunc2)(int p);
	void (*overfunc)();
	void (*overfunc2)(int p);
	void (*outfunc)();
	void (*changefunc)();
	void (*changefunc2)(int p);
	void (*clickfunc3)(wg* w);
	
		*/

	void (*reframef)(struct wg* w);
};

typedef struct wg wg;

void wginit(wg* w);
void wgfree(wg *w);
void wgdraw(wg *w);
void wgdrawover(wg *w);
void wgin(wg *w, inev* ie);
void wgframeup(wg *w);
void wgreframe(wg *w);	//resized or moved
wg* wgget(wg *w, const char* name);
void wgadd(wg *w, wg* newg);
void wghide(wg *w);
void wgshow(wg *w);
void wgchcall(wg *w, wg* ch, char type, void* data);	/* child callback */
void wgfreech(wg *w);	/* free subwg children */
void wgtofront(wg *w);	/* only used for windows. edit: needed for everything since droplist uses it on parent tree. */
void wghideall(wg *w);
void wggainfocus(wg *w);
void wglosefocus(wg *w);

void subcrop(float *src1, float *src2, float *ndest);
void szfs(wg *w);

#endif
