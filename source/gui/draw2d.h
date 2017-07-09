










#ifndef DRAW2D_H
#define DRAW2D_H

#include "../sys/inc.h"

void drawim(unsigned int texi, float left, float top, float right, float bottom, float texleft, float textop, float texright, float texbottom, float *crop);
void drawsq(float r, float g, float b, float a, float left, float top, float right, float bottom, float *crop);
void drawl(float r, float g, float b, float a, float x1, float y1, float x2, float y2, float *crop);

#endif
