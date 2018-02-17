










#ifndef WINW_H
#define WINW_H

#include "../wg.h"
#include "image.h"
#include "../cursor.h"

#define WC_C	0
#define WC_M	1
#define WC_UA	2
#define WC_DA	3
#define WC_LA	4
#define WC_RA	5
#define WC_VB	6
#define WC_HB	7
#define WC_T	8
#define WCS		9

struct ww
{
	wg base;

	char font;
	char* title;
	float opos[4];
	float tpos[4];
	dbool over;
	dbool ldown;
	float ldownp[2];
};

typedef struct ww ww;


void wwinit(ww *b, wg* parent, const char* name, 
	const char* title, char f, void(*reframef)(wg* w),
	dbool start);
void wwfree(wg* bw);
void wwsz(wg* bw);
void wwdraw(wg *bw);
void wwdrawov(wg *bw);
void wwin(wg *bw, inev* ie);
void wwnewsz(wg *bw);

#if 0

class Win : public Widget
{
public:

	Image bg_logo_image;

	Text title_text;
	VScroll m_vscroll;
	
	Button m_trclose;	//top right close
	Button m_trfull;	//top full size toggle
	float prevpos[4];	//for fullscreen toggle

	int32_t m_mousedown[2];

	Win();
	Win(Widget* parent, const char* n, void (*reframef)(Widget* w));

	//in msvs2012, not making these virtual still calls these wtf
	//but not on mac
	virtual void fillout(float* outpos);
	virtual void show();
	virtual void inev(InEv* ie);
	virtual void draw();
	virtual void drawover();
	virtual void reframe();
	virtual void chcall(Widget* ch, int32_t type, void* data);
	virtual void subframe(float* fr);
	void fullsize();	//toggle fullscreen size
};

#endif

#endif
