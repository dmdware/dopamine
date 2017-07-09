







#ifndef IMAGE_H
#define IMAGE_H

#include "../wg.h"
#include "../../algo/bool.h"

struct imw
{
	wg base;
	
	float texc[4];	/* texture coordinates */
	unsigned int texi;
	float rgba[4];
};

typedef struct imw imw;

void imwinit(imw *i,
				wg* parent, 
				const char* nm, 
				const char* filepath, 
				dbool clamp, 
				void (*reframef)(wg* w), 
				float r, 
				float g, 
				float b, 
				float a, 
				float texleft, 
				float textop, 
				float texright, 
				float texbottom);

void imwdraw(wg *bw);

#endif
