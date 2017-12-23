










#ifndef V3F_H
#define V3F_H

#include "../sys/inc.h"

struct v3f
{
	float x, y, z;
};

typedef struct v3f v3f;

void v3ftransform(v3f *v, const float *m);
void v3ftransform3(v3f *v, const float *m);
void v3fmul(v3f *v, v3f f, float s);
void v3fsub(v3f *v, v3f f, v3f s);
void v3fadd(v3f *v, v3f f, v3f s);

#endif
