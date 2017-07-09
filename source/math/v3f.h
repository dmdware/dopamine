










#ifndef VEC3F_H
#define VEC3F_H

#include "../sys/inc.h"

struct v3f
{
	float x, y, z;
};

typedef struct v3f v3f;

void v3ftransform(v3f *v, const float *m);
void v3ftransform3(v3f *v, const float *m);

#endif
