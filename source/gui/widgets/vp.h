







#ifndef VP_H
#define VP_H

#include "../wg.h"
#include "../../algo/bool.h"

struct vp
{
	wg base;

	void(*drawf)(void *e, int x, int y, int w, int h);
	dbool(*ldownf)(void *e, int relx, int rely, int w, int h);
	dbool(*lupf)(void *e, int relx, int rely, int w, int h);
	dbool(*mmovef)(void *e, int relx, int rely, int w, int h);
	dbool(*rdownf)(void *e, int relx, int rely, int w, int h);
	dbool(*rupf)(void *e, int relx, int rely, int w, int h);
	dbool(*mousewf)(void *e, int d);
	dbool(*mdownf)(void *e, int relx, int rely, int w, int h);
	dbool(*mupf)(void *e, int relx, int rely, int w, int h);
	void(*freef)(wg* w);
	dbool ldown;
	dbool over;
};

typedef struct vp vp;

void vpinit(vp* v, wg* parent, const char* n, void(*reframef)(wg* w),
	void(*drawf)(void *e, int x, int y, int w, int h),
	dbool(*ldownf)(void *e, int relx, int rely, int w, int h),
	dbool(*lupf)(void *e, int relx, int rely, int w, int h),
	dbool(*mmovef)(void *e, int relx, int rely, int w, int h),
	dbool(*rdownf)(void *e, int relx, int rely, int w, int h),
	dbool(*rupf)(void *e, int relx, int rely, int w, int h),
	dbool(*mousewf)(void *e, int d),
	dbool(*mdownf)(void *e, int relx, int rely, int w, int h),
	dbool(*mupf)(void *e, int relx, int rely, int w, int h),
	void *e, void(*freef)(wg* w));
void vpin(wg *w, inev *ie);
void vpdraw(wg *w);
void vpfree(wg* w);

#endif
