










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

#define MCR	0.7f
#define MCG	0.7f
#define MCB	0.7f
#define LCR	0.8f
#define LCG	0.8f
#define LCB	0.8f
#define DCR	0.5f
#define DCG	0.5f
#define DCB	0.5f
#define TCR	0.9f
#define TCG	0.9f
#define TCB 0.9f
#define MCA	0.8f
#define LCA	0.8f
#define DCA	0.8f
#define TCA 0.8f
#define MCRO	0.8f
#define MCGO	0.8f
#define MCBO	0.8f
#define LCRO	0.9f
#define LCGO	0.9f
#define LCBO	0.9f
#define DCRO	0.6f
#define DCGO	0.6f
#define DCBO	0.6f
#define TCRO	1.0f
#define TCGO	1.0f
#define TCBO	1.0f
#define MCAO	0.8f
#define LCAO	0.8f
#define DCAO	0.8f
#define TCAO	0.8f

extern const float MC[4];
extern const float LC[4];
extern const float DC[4];
extern const float TC[4];
extern const float MCO[4];
extern const float LCO[4];
extern const float DCO[4];
extern const float TCO[4];

#define WG_GUI					0
#define WG_IMAGE				1
#define WG_EDITBOX				2
#define WG_BUTTON				3
#define WG_VIEWLAYER			4
#define WG_TEXT					5
#define WG_LINK					6
#define WG_VIEWPORT				7
#define WG_DROPMENU				8
#define WG_DROPLIST				9
#define WG_WIN					10
#define WGS						11

struct wg
{
	/* TODO trim members delegate to sub's */

	char type;
	struct wg* parent;
	float pos[4];
	float crop[4];

	char name[16];

	dbool hidden;

	list sub;	/* wg* */
	void* extra;	/* extra user params */

	/*
	float texc[4];	//texture coordinates
	float tpos[4];	//text pos
	float scar[4];	//scrolling area rect for windows
	unsigned int texi;
	unsigned int bgtex;
	unsigned int bgovtex;
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
	void (*clickf2)(int p);
	void (*overf)();
	void (*overf2)(int p);
	void (*outf)();
	void (*changef)();
	void (*changef2)(int p);
	void (*clickf3)(wg* w);
	
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
void wgtop(wg *w);	/* only used for windows. edit: needed for everything since droplist uses it on parent tree. */
void wghideall(wg *w);
void wggain(wg *w);
void wglose(wg *w);

void subcrop(float *src1, float *src2, float *ndest);
void szfs(wg *w);

#endif
