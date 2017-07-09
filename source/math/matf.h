#ifndef MATF_H
#define MATF_H

#include "../sys/inc.h"
#include "quatf.h"

struct qf;
struct v4f;

struct mf
{
	//	mf data, stored in column-major order
	float matrix[16];
};

typedef struct mf mf;

//void mfinit(mf *m, v4f a, v4f b, v4f c, v4f d);

void mfset(mf *m, const float *matrix );

void mfreset(mf *m);

void mfpostmult(mf *m1, const mf *m2);
void mfpostmult2(mf *m1, const mf *m2);

//	Set the translation of the current matrix. Will erase any previous values.
void mftransl(mf *m, const float *translation);

//	Set the inverse translation of the current matrix. Will erase any previous values.
void mfinvtrans(mf *m, const float *translation);

void mfscale(mf *m, const float *scale);

//	Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
void mfrotrad(mf *m, const float *angles);

//	Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
void mfrotdeg(mf *m, const float *angles);

//	Make a rotation matrix from a quaternion. The 4th row and column are unmodified.
void mfrotquat(mf *m, qf *q);

//	Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
void mfinvrotrad(mf *m, const float *angles);

//	Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified.
void mfinvrotdeg(mf *m, const float *angles);

float *mfget(mf *m, int row, int col);

//	Translate a vector by the inverse of the translation part of this matrix.
void mfinvtranvec(mf *m, float *vec);

//	Rotate a vector by the inverse of the rotation part of this matrix.
void mfinvrotvec(mf *m, float *pvec);

#endif
