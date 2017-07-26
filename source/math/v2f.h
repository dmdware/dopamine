#ifndef V2F_H
#define V2F_H

#include "../algo/bool.h"

struct v2f
{
	float x, y;
};

typedef struct v2f v2f;

void v2freset(v2f *v);

void v2fset(v2f *v, float x, float y);

void v2fset2(v2f *v, const float *values);

dbool v2feq(v2f v1, v2f v2);

dbool v2fneq(v2f v1, v2f v2);

void v2fadd(v2f *v, v2f v1, v2f v2);

void v2fsub(v2f *v, v2f v1, v2f v2);

void v2fmulf(v2f *v, v2f v1, const float num);

void v2fmulv(v2f *v, const v2f v1, v2f v2);

void v2fdiv(v2f *v, const v2f v1, const float num);

#endif
