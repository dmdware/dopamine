







#ifndef VP_H
#define VP_H

#include "../wg.h"
#include "../../algo/bool.h"

struct vp
{
	wg base;

	void(*drawf)(int p, int x, int y, int w, int h);
	dbool(*ldownf)(int p, int relx, int rely, int w, int h);
	dbool(*lupf)(int p, int relx, int rely, int w, int h);
	dbool(*mmovef)(int p, int relx, int rely, int w, int h);
	dbool(*rdownf)(int p, int relx, int rely, int w, int h);
	dbool(*rupf)(int p, int relx, int rely, int w, int h);
	dbool(*mousewf)(int p, int d);
	dbool(*mdownf)(int p, int relx, int rely, int w, int h);
	dbool(*mupf)(int p, int relx, int rely, int w, int h);
	void(*freef)(wg* w);
	dbool ldown;
};

typedef struct vp vp;

void vpinit(wg* parent, vp* v, const char* n, void(*reframef)(wg* w),
	void(*drawf)(int p, int x, int y, int w, int h),
	dbool(*ldownf)(int p, int relx, int rely, int w, int h),
	dbool(*lupf)(int p, int relx, int rely, int w, int h),
	dbool(*mmovef)(int p, int relx, int rely, int w, int h),
	dbool(*rdownf)(int p, int relx, int rely, int w, int h),
	dbool(*rupf)(int p, int relx, int rely, int w, int h),
	dbool(*mousewf)(int p, int d),
	dbool(*mdownf)(int p, int relx, int rely, int w, int h),
	dbool(*mupf)(int p, int relx, int rely, int w, int h),
	void *e, void(*freef)(wg* w));
void vpin(wg *w, inev *ie);
void vpdraw(wg *w);
void vpfree(wg* w);

#endif
