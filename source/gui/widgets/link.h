










#ifndef LINK_H
#define LINK_H

#include "../widget.h"

//hyperlink
struct hpl
{
	wg base;
	char* text;
	char font;
	void(*clickfunc)();
};

typedef struct imw imw;

class Link : public Widget
{
public:
	Link(Widget* parent, const char* name, const RichText t, int32_t f, void (*reframef)(Widget* w), void (*click)()) : Widget()
	{
		m_parent = parent;
		m_type = WIDGET_LINK;
		m_name = name;
		m_over = false;
		m_ldown = false;
		m_text = t;
		m_font = f;
		reframefunc = reframef;
		clickfunc = click;
		reframe();


		pstrset(&tw->text, t);
	}

	void draw();
	void inev(InEv* ie);
};

#endif
