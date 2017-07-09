#ifndef MATFX_H
#define MATFX_H

#include "../sys/inc.h"
#include "quatfx.h"
#include "fixfrac.h"

struct qx;
struct v4f;

struct mx
{
	//	mx data, stored in column-major order
	fx matrix[16];
};

typedef struct mx mx;

void mxinit(mx *m);
void mxfree(mx *m);

//void mxinit(mx *m, v4f a, v4f b, v4f c, v4f d);

void mxset(mx *m, fx *matrix );

void mxreset(mx *m);

void mxpostmult(mx *m1, mx *m2);
void mxpostmult2(mx *m1, mx *m2);

//	Set the translation of the current matrix. Will erase any previous values.
void mxtransl(mx *m, fx *translation);

//	Set the inverse translation of the current matrix. Will erase any previous values.
void mxinvtrans(mx *m, fx *translation);

void mxscale(mx *m, fx *scale);

//	Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
void mxrotrad(mx *m, fx *angles);

//	Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
void mxrotdeg(mx *m, fx *angles);

//	Make a rotation matrix from a quaternion. The 4th row and column are unmodified.
void mxrotquat(mx *m, qx *q);

//	Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
void mxinvrotrad(mx *m, fx *angles);

//	Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
void mxinvrotdeg(mx *m, fx *angles);

fx *mxget(mx *m, int row, int col);

//	Translate a vector by the inverse of the translation part of this matrix.
void mxinvtranvec(mx *m, fx *vec);

//	Rotate a vector by the inverse of the rotation part of this matrix.
void mxinvrotvec(mx *m, fx *pvec);

#endif
