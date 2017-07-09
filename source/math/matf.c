#include "matf.h"
#include "../sys/inc.h"
#include "quatf.h"
#include "v4f.h"

void mfset(mf *m, const float *matrix )
{
	memcpy( m->matrix, matrix, sizeof( float )*16 );
}

void mfreset(mf *m)
{
	float *matrix = m->matrix;
	memset( matrix, 0, sizeof( float )*16 );
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1;
}

float *mfget(mf *m, int row, int col)
{
	return &m->matrix[ row + col*4 ];
}

void mfinit(mf *m, v4f a, v4f b, v4f c, v4f d)
{
#define M(row,col)  matrix[col*4+row]

#undef M
}

void mfinvrotvec(mf *m, float *pvec)
{
	float vec[3];
	float *matrix = m->matrix;

	vec[0] = pvec[0]*matrix[0]+pvec[1]*matrix[1]+pvec[2]*matrix[2];
	vec[1] = pvec[0]*matrix[4]+pvec[1]*matrix[5]+pvec[2]*matrix[6];
	vec[2] = pvec[0]*matrix[8]+pvec[1]*matrix[9]+pvec[2]*matrix[10];

	memcpy( pvec, vec, sizeof( float )*3 );
}

void mfinvtranvec(mf *m, float *vec)
{
	float *matrix = m->matrix;
	vec[0] = vec[0]-matrix[12];
	vec[1] = vec[1]-matrix[13];
	vec[2] = vec[2]-matrix[14];
}

//no longer used for light matrix concatenation (and skeletal animation?)
void mfpostmult(mf *m1, const mf *m2)
{
	float newm[16], *a, *b;
	
	a = m1->matrix;
	b = m2->matrix;
	
#if 0
	
	newm[0] = a[0]*b[0] + a[4]*b[1] + a[8]*b[2];
	newm[1] = a[1]*b[0] + a[5]*b[1] + a[9]*b[2];
	newm[2] = a[2]*b[0] + a[6]*b[1] + a[10]*b[2];
	newm[3] = 0;
	
	newm[4] = a[0]*b[4] + a[4]*b[5] + a[8]*b[6];
	newm[5] = a[1]*b[4] + a[5]*b[5] + a[9]*b[6];
	newm[6] = a[2]*b[4] + a[6]*b[5] + a[10]*b[6];
	newm[7] = 0;
	
	newm[8] = a[0]*b[8] + a[4]*b[9] + a[8]*b[10];
	newm[9] = a[1]*b[8] + a[5]*b[9] + a[9]*b[10];
	newm[10] = a[2]*b[8] + a[6]*b[9] + a[10]*b[10];
	newm[11] = 0;
	
	newm[12] = a[0]*b[12] + a[4]*b[13] + a[8]*b[14] + a[12];
	newm[13] = a[1]*b[12] + a[5]*b[13] + a[9]*b[14] + a[13];
	newm[14] = a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14];
	newm[15] = 1;
	
#else
	
	newm[0]  = a[0]  *b[0]  + a[4]  *b[1]  + a[8]  *b[2]   + a[12]  *b[3];
	newm[1]  = a[1]  *b[0]  + a[5]  *b[1]  + a[9]  *b[2]   + a[13]  *b[3];
	newm[2]  = a[2]  *b[0]  + a[6]  *b[1]  + a[10]  *b[2]  + a[14]  *b[3];
	newm[3]  = a[3]  *b[0]  + a[7]  *b[1]  + a[11]  *b[2]  + a[15]  *b[3];
	
	newm[4]  = a[0]  *b[4]  + a[4]  *b[5]  + a[8]  *b[6]   + a[12]  *b[7];
	newm[5]  = a[1]  *b[4]  + a[5]  *b[5]  + a[9]  *b[6]   + a[13]  *b[7];
	newm[6]  = a[2]  *b[4]  + a[6]  *b[5]  + a[10]  *b[6]  + a[14]  *b[7];
	newm[7]  = a[3]  *b[4]  + a[7]  *b[5]  + a[11]  *b[6]  + a[15]  *b[7];
	
	newm[8]  = a[0]  *b[8]  + a[4]  *b[9]  + a[8]  *b[10]  + a[12]  *b[11];
	newm[9]  = a[1]  *b[8]  + a[5]  *b[9]  + a[9]  *b[10]  + a[13]  *b[11];
	newm[10] = a[2]  *b[8]  + a[6]  *b[9]  + a[10]  *b[10] + a[14]  *b[11];
	newm[11] = a[3]  *b[8]  + a[7]  *b[9]  + a[11]  *b[10] + a[15]  *b[11];
	
	newm[12] = a[0]  *b[12] + a[4]  *b[13] + a[8]  *b[14]  + a[12]  *b[15];
	newm[13] = a[1]  *b[12] + a[5]  *b[13] + a[9]  *b[14]  + a[13]  *b[15];
	newm[14] = a[2]  *b[12] + a[6]  *b[13] + a[10]  *b[14] + a[14]  *b[15];
	newm[15] = a[3]  *b[12] + a[7]  *b[13] + a[11]  *b[14] + a[15]  *b[15];
	
#endif
	
	mfset(m1, newm);
}

//used only for light matrix (and skeletal animation?), doesn't work with perspective projection because that is not an affine transformation
void mfpostmult2(mf *m1, const mf *m2)
{
	float newm[16], *a, *b;
	
	a = m1->matrix;
	b = m2->matrix;
	
#if 1
	
	newm[0] = a[0]*b[0] + a[4]*b[1] + a[8]*b[2];
	newm[1] = a[1]*b[0] + a[5]*b[1] + a[9]*b[2];
	newm[2] = a[2]*b[0] + a[6]*b[1] + a[10]*b[2];
	newm[3] = 0;
	
	newm[4] = a[0]*b[4] + a[4]*b[5] + a[8]*b[6];
	newm[5] = a[1]*b[4] + a[5]*b[5] + a[9]*b[6];
	newm[6] = a[2]*b[4] + a[6]*b[5] + a[10]*b[6];
	newm[7] = 0;
	
	newm[8] = a[0]*b[8] + a[4]*b[9] + a[8]*b[10];
	newm[9] = a[1]*b[8] + a[5]*b[9] + a[9]*b[10];
	newm[10] = a[2]*b[8] + a[6]*b[9] + a[10]*b[10];
	newm[11] = 0;
	
	newm[12] = a[0]*b[12] + a[4]*b[13] + a[8]*b[14] + a[12];
	newm[13] = a[1]*b[12] + a[5]*b[13] + a[9]*b[14] + a[13];
	newm[14] = a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14];
	newm[15] = 1;
	
#else
	
	newm[0]  = a[0]  *b[0]  + a[4]  *b[1]  + a[8]  *b[2]   + a[12]  *b[3];
	newm[1]  = a[1]  *b[0]  + a[5]  *b[1]  + a[9]  *b[2]   + a[13]  *b[3];
	newm[2]  = a[2]  *b[0]  + a[6]  *b[1]  + a[10]  *b[2]  + a[14]  *b[3];
	newm[3]  = a[3]  *b[0]  + a[7]  *b[1]  + a[11]  *b[2]  + a[15]  *b[3];
	
	newm[4]  = a[0]  *b[4]  + a[4]  *b[5]  + a[8]  *b[6]   + a[12]  *b[7];
	newm[5]  = a[1]  *b[4]  + a[5]  *b[5]  + a[9]  *b[6]   + a[13]  *b[7];
	newm[6]  = a[2]  *b[4]  + a[6]  *b[5]  + a[10]  *b[6]  + a[14]  *b[7];
	newm[7]  = a[3]  *b[4]  + a[7]  *b[5]  + a[11]  *b[6]  + a[15]  *b[7];
	
	newm[8]  = a[0]  *b[8]  + a[4]  *b[9]  + a[8]  *b[10]  + a[12]  *b[11];
	newm[9]  = a[1]  *b[8]  + a[5]  *b[9]  + a[9]  *b[10]  + a[13]  *b[11];
	newm[10] = a[2]  *b[8]  + a[6]  *b[9]  + a[10]  *b[10] + a[14]  *b[11];
	newm[11] = a[3]  *b[8]  + a[7]  *b[9]  + a[11]  *b[10] + a[15]  *b[11];
	
	newm[12] = a[0]  *b[12] + a[4]  *b[13] + a[8]  *b[14]  + a[12]  *b[15];
	newm[13] = a[1]  *b[12] + a[5]  *b[13] + a[9]  *b[14]  + a[13]  *b[15];
	newm[14] = a[2]  *b[12] + a[6]  *b[13] + a[10]  *b[14] + a[14]  *b[15];
	newm[15] = a[3]  *b[12] + a[7]  *b[13] + a[11]  *b[14] + a[15]  *b[15];
	
#endif
	
	mfset(m1, newm);
}

void mftransl(mf *m, const float *translation)
{
	float *matrix;
	
	matrix = m->matrix;
	
#if 0
	matrix[0] = matrix[5] =  matrix[10] = matrix[15] = 1.0;
	matrix[1] = matrix[2] = matrix[3] = matrix[4] = 0.0;
	matrix[6] = matrix[7] = matrix[8] = matrix[9] = 0.0;
	matrix[11] = 0.0;

#elif 1

	matrix[12] = translation[0];
	matrix[13] = translation[1];
	matrix[14] = translation[2];

#elif 0

#define M(row,col)  matrix[col*4+row]
	//#define M(row,col)  matrix[row*4+col]
	/*
	// http://stackoverflow.com/questions/13293469/why-does-my-translation-matrix-needs-to-be-transposed
	1, 0, 0, 0
	0, 1, 0, 0
	0, 0, 1, 0
	x, y, z, 1
	*/
	M(0,0) = 1;
	M(0,1) = 0;
	M(0,2) = 0;
	M(0,3) = translation[0];
	M(1,0) = 0;
	M(1,1) = 1;
	M(1,2) = 0;
	M(1,3) = translation[1];
	M(2,0) = 0;
	M(2,1) = 0;
	M(2,2) = 1;
	M(2,3) = translation[2];
	M(3,0) = 0;
	M(3,1) = 0;
	M(3,2) = 0;
	M(3,3) = 1;
#undef M

#endif
}

void mfinvtrans(mf *m, const float *translation)
{
	float *matrix;
	
	matrix = m->matrix;
	
	matrix[12] = -translation[0];
	matrix[13] = -translation[1];
	matrix[14] = -translation[2];
}

void mfscale(mf *m, const float *scale)
{
	float *matrix;
	
	mfreset(m);

	matrix = m->matrix;
	
	matrix[0] = scale[0];
	matrix[5] = scale[1];
	matrix[10] = scale[2];
	matrix[15] = 1;
}

void mfrotdeg(mf *m, const float *angles)
{
	float vec[3], *matrix;
	
	matrix = m->matrix;
	
	vec[0] = ( float )( angles[0]*180.0/M_PI );
	vec[1] = ( float )( angles[1]*180.0/M_PI );
	vec[2] = ( float )( angles[2]*180.0/M_PI );
	
	mfrotrad(m, vec);
}

void mfinvrotdeg(mf *m, const float *angles)
{
	float vec[3], *matrix;
	
	matrix = m->matrix;
	
	vec[0] = ( float )( angles[0]*180.0/M_PI );
	vec[1] = ( float )( angles[1]*180.0/M_PI );
	vec[2] = ( float )( angles[2]*180.0/M_PI );
	
	mfinvrotrad(m, vec);
}

void mfrotrad(mf *m, const float *angles)
{
	double cr, sr, cp, sp, cy, sy, srsp, crsp;
	float *matrix;
	
	matrix = m->matrix;
	
	cr = cos( angles[0] );
	sr = sin( angles[0] );
	cp = cos( angles[1] );
	sp = sin( angles[1] );
	cy = cos( angles[2] );
	sy = sin( angles[2] );

	matrix[0] = ( float )( cp*cy );
	matrix[1] = ( float )( cp*sy );
	matrix[2] = ( float )( -sp );

	srsp = sr*sp;
	crsp = cr*sp;

	matrix[4] = ( float )( srsp*cy-cr*sy );
	matrix[5] = ( float )( srsp*sy+cr*cy );
	matrix[6] = ( float )( sr*cp );

	matrix[8] = ( float )( crsp*cy+sr*sy );
	matrix[9] = ( float )( crsp*sy-sr*cy );
	matrix[10] = ( float )( cr*cp );
}

void mfinvrotrad(mf *m, const float *angles)
{
	double cr, sr, cp, sp, cy, sy, srsp, crsp;
	float *matrix;
	
	matrix = m->matrix;
	
	cr = cos( angles[0] );
	sr = sin( angles[0] );
	cp = cos( angles[1] );
	sp = sin( angles[1] );
	cy = cos( angles[2] );
	sy = sin( angles[2] );

	matrix[0] = ( float )( cp*cy );
	matrix[4] = ( float )( cp*sy );
	matrix[8] = ( float )( -sp );

	srsp = sr*sp;
	crsp = cr*sp;

	matrix[1] = ( float )( srsp*cy-cr*sy );
	matrix[5] = ( float )( srsp*sy+cr*cy );
	matrix[9] = ( float )( sr*cp );

	matrix[2] = ( float )( crsp*cy+sr*sy );
	matrix[6] = ( float )( crsp*sy-sr*cy );
	matrix[10] = ( float )( cr*cp );
}

void mfrotquat(mf *m, qf *q)
{
	float *matrix, *quat;
	
	quat = (float*)q->quat;
	matrix = m->matrix;
	
	matrix[0] = ( float )( 1.0 - 2.0*quat[1]*quat[1] - 2.0*quat[2]*quat[2] );
	matrix[1] = ( float )( 2.0*quat[0]*quat[1] + 2.0*quat[3]*quat[2] );
	matrix[2] = ( float )( 2.0*quat[0]*quat[2] - 2.0*quat[3]*quat[1] );

	matrix[4] = ( float )( 2.0*quat[0]*quat[1] - 2.0*quat[3]*quat[2] );
	matrix[5] = ( float )( 1.0 - 2.0*quat[0]*quat[0] - 2.0*quat[2]*quat[2] );
	matrix[6] = ( float )( 2.0*quat[1]*quat[2] + 2.0*quat[3]*quat[0] );

	matrix[8] = ( float )( 2.0*quat[0]*quat[2] + 2.0*quat[3]*quat[1] );
	matrix[9] = ( float )( 2.0*quat[1]*quat[2] - 2.0*quat[3]*quat[0] );
	matrix[10] = ( float )( 1.0 - 2.0*quat[0]*quat[0] - 2.0*quat[1]*quat[1] );
}

