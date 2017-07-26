










#ifndef V3FX_H
#define V3FX_H

#include "../sys/inc.h"
#include "fixfrac.h"

struct v3x
{
	fx x, y, z;
};

typedef struct v3x v3x;

void v3xinit(v3x *v);
void v3xfree(v3x *v);
void v3xtransform(v3x *v, fx *m);
void v3xtransform3(v3x *v, fx *m);
void v3xmul(v3x *v, v3x f, fx s);
void v3xsub(v3x *v, v3x f, v3x s);
void v3xadd(v3x *v, v3x f, v3x s);
void v3xdot(v3x v, fx *d);

#endif
