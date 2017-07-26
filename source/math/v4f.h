#ifndef V4F_H
#define V4F_H

#include "v3f.h"
#include "../algo/bool.h"
#include "matf.h"

struct v3f;

struct mf;

struct v4f
{
	float x, y, z, w;
};

typedef struct v4f v4f;

void v4freset(v4f *v);

void v4fset(v4f *v, float x, float y, float z, float w);

void v4fset2(v4f *v, const float *values);

dbool v4feq(v4f v1, v4f v2);

dbool v4fneq(v4f v1, v4f v2);

void v4fadd(v4f *v, v4f v1, v4f v2);

void v4fsub(v4f *v, v4f v1, v4f v2);

void v4fmulf(v4f *v, v4f v1, const float num);

void v4fmulv(v4f *v, const v4f v1, v4f v2);

void v4fdiv(v4f *v, const v4f v1, const float num);

void v4ftransform(v4f *v, const mf *m);
void v4ftransform3(v4f *v, const mf *m);

#endif
