#include "matfx.h"
#include "../sys/inc.h"
#include "quatf.h"
#include "v4f.h"

#if 0
void mxinit(mx *m)
{
	char i;

	for(i=0; i<16; ++i)
	{
		fxinit(m->matrix[i], FX_PREC);
		fxsetri(m->matrix[i], 0);
	}
}

void mxfree(mx *m)
{
	char i;

	for(i=0; i<16; ++i)
	{
		fxfree(m->matrix[i]); 
	}
}

void mxset(mx *m, fx *matrix )
{
	char i;

	for(i=0; i<16; ++i)
		fxset(m->matrix[i], matrix[i]);
}

void mxreset(mx *m)
{
	fxsetri(m->matrix[1], 0);
	fxsetri(m->matrix[2], 0);
	fxsetri(m->matrix[3], 0);
	fxsetri(m->matrix[4], 0);
	fxsetri(m->matrix[6], 0);
	fxsetri(m->matrix[7], 0);
	fxsetri(m->matrix[8], 0);
	fxsetri(m->matrix[9], 0);
	fxsetri(m->matrix[11], 0);
	fxsetri(m->matrix[12], 0);
	fxsetri(m->matrix[13], 0);
	fxsetri(m->matrix[14], 0);
	
	fxsetri(m->matrix[0], 1);
	fxsetri(m->matrix[5], 1);
	fxsetri(m->matrix[10], 1);
	fxsetri(m->matrix[15], 1);
}

fx *mxget(mx *m, int row, int col)
{
	return &m->matrix[ row + col*4 ];
}

void mxinit2(mx *m, v4f a, v4f b, v4f c, v4f d)
{
#define M(row,col)  matrix[col*4+row]

#undef M
}

void mxinvrotvec(mx *m, fx *pvec)
{
	fx vec[16];
	char i;

	for(i=0; i<16; ++i)
	{
		fxinit(vec[i], FX_PREC);
	}

	//vec[0] = pvec[0]*matrix[0]+pvec[1]*matrix[1]+pvec[2]*matrix[2];
	fxmul(vec[0], m->matrix[0], pvec[0]);
	fxmul(vec[1], m->matrix[1], pvec[1]);
	fxmul(vec[2], m->matrix[2], pvec[2]);
	fxadd(vec[0], vec[0], vec[1]);
	fxadd(vec[0], vec[0], vec[2]);

	//vec[1] = pvec[0]*matrix[4]+pvec[1]*matrix[5]+pvec[2]*matrix[6];
	fxmul(vec[3], m->matrix[4], pvec[0]);
	fxmul(vec[4], m->matrix[5], pvec[1]);
	fxmul(vec[5], m->matrix[6], pvec[2]);
	fxadd(vec[3], vec[3], vec[4]);
	fxadd(vec[3], vec[3], vec[5]);

	//vec[2] = pvec[0]*matrix[8]+pvec[1]*matrix[9]+pvec[2]*matrix[10];
	fxmul(vec[6], m->matrix[8], pvec[0]);
	fxmul(vec[7], m->matrix[9], pvec[1]);
	fxmul(vec[8], m->matrix[10], pvec[2]);
	fxadd(vec[6], vec[6], vec[7]);
	fxadd(vec[6], vec[6], vec[8]);

	fxset(pvec[0], vec[0]);
	fxset(pvec[1], vec[3]);
	fxset(pvec[2], vec[6]);

	for(i=0; i<16; ++i)
	{
		fxfree(vec[i]);
	}
}

void mxinvtranvec(mx *m, fx *vec)
{
	fx *matrix = m->matrix;

	fxsub(vec[0], vec[0], matrix[12]);
	fxsub(vec[1], vec[1], matrix[13]);
	fxsub(vec[2], vec[2], matrix[14]);

	//vec[0] = vec[0]-matrix[12];
	//vec[1] = vec[1]-matrix[13];
	//vec[2] = vec[2]-matrix[14];
}

//no longer used for light matrix concatenation (and skeletal animation?)
void mxpostmult(mx *m1, mx *m2)
{
	fx newm[16], *a, *b, ab[16];
	char i;

	for(i=0; i<16; ++i)
	{
		fxinit(newm[i], FX_PREC);
		fxinit(ab[i], FX_PREC);
	}

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

	///////////////////////

	//newm[0]  = a[0]  *b[0]  + a[4]  *b[1]  + a[8]  *b[2]   + a[12]  *b[3];	
	fxmul(ab[0], a[0], b[0]);
	fxmul(ab[4], a[4], b[1]);
	fxmul(ab[8], a[8], b[2]);
	fxmul(ab[12], a[12], b[3]);
	fxadd(ab[0], ab[0], ab[4]);
	fxadd(ab[0], ab[0], ab[8]);
	fxadd(ab[0], ab[0], ab[12]);
	fxset(newm[0], ab[0]);
	
	//newm[1]  = a[1]  *b[0]  + a[5]  *b[1]  + a[9]  *b[2]   + a[13]  *b[3];
	fxmul(ab[1], a[1], b[0]);
	fxmul(ab[5], a[5], b[1]);
	fxmul(ab[9], a[9], b[2]);
	fxmul(ab[13], a[13], b[3]);
	fxadd(ab[1], ab[1], ab[5]);
	fxadd(ab[1], ab[1], ab[9]);
	fxadd(ab[1], ab[1], ab[13]);
	fxset(newm[1], ab[1]);
	
	//newm[2]  = a[2]  *b[0]  + a[6]  *b[1]  + a[10]  *b[2]  + a[14]  *b[3];
	fxmul(ab[2], a[2], b[0]);
	fxmul(ab[6], a[6], b[1]);
	fxmul(ab[10], a[10], b[2]);
	fxmul(ab[14], a[14], b[3]);
	fxadd(ab[2], ab[2], ab[6]);
	fxadd(ab[2], ab[2], ab[10]);
	fxadd(ab[2], ab[2], ab[14]);
	fxset(newm[2], ab[2]);

	//newm[3]  = a[3]  *b[0]  + a[7]  *b[1]  + a[11]  *b[2]  + a[15]  *b[3];	
	fxmul(ab[3], a[3], b[0]);
	fxmul(ab[7], a[7], b[1]);
	fxmul(ab[11], a[11], b[2]);
	fxmul(ab[15], a[15], b[3]);
	fxadd(ab[3], ab[3], ab[7]);
	fxadd(ab[3], ab[3], ab[11]);
	fxadd(ab[3], ab[3], ab[15]);
	fxset(newm[3], ab[3]);

	///////////////////////

	//newm[4]  = a[0]  *b[4]  + a[4]  *b[5]  + a[8]  *b[6]   + a[12]  *b[7];	
	fxmul(ab[0], a[0], b[4]);
	fxmul(ab[4], a[4], b[5]);
	fxmul(ab[8], a[8], b[6]);
	fxmul(ab[12], a[12], b[7]);
	fxadd(ab[0], ab[0], ab[4]);
	fxadd(ab[0], ab[0], ab[8]);
	fxadd(ab[0], ab[0], ab[12]);
	fxset(newm[4], ab[0]);

	//newm[5]  = a[1]  *b[4]  + a[5]  *b[5]  + a[9]  *b[6]   + a[13]  *b[7];	
	fxmul(ab[1], a[1], b[4]);
	fxmul(ab[5], a[5], b[5]);
	fxmul(ab[9], a[9], b[6]);
	fxmul(ab[13], a[13], b[7]);
	fxadd(ab[1], ab[1], ab[5]);
	fxadd(ab[1], ab[1], ab[9]);
	fxadd(ab[1], ab[1], ab[13]);
	fxset(newm[5], ab[1]);

	//newm[6]  = a[2]  *b[4]  + a[6]  *b[5]  + a[10]  *b[6]  + a[14]  *b[7];	
	fxmul(ab[2], a[2], b[4]);
	fxmul(ab[6], a[6], b[5]);
	fxmul(ab[10], a[10], b[6]);
	fxmul(ab[14], a[14], b[7]);
	fxadd(ab[2], ab[2], ab[6]);
	fxadd(ab[2], ab[2], ab[10]);
	fxadd(ab[2], ab[2], ab[14]);
	fxset(newm[6], ab[2]);

	//newm[7]  = a[3]  *b[4]  + a[7]  *b[5]  + a[11]  *b[6]  + a[15]  *b[7];	
	fxmul(ab[3], a[3], b[4]);
	fxmul(ab[7], a[7], b[5]);
	fxmul(ab[11], a[11], b[6]);
	fxmul(ab[15], a[15], b[7]);
	fxadd(ab[3], ab[3], ab[7]);
	fxadd(ab[3], ab[3], ab[11]);
	fxadd(ab[3], ab[3], ab[15]);
	fxset(newm[7], ab[3]);

	///////////////////////

	//newm[8]  = a[0]  *b[8]  + a[4]  *b[9]  + a[8]  *b[10]  + a[12]  *b[11];
	fxmul(ab[0], a[0], b[8]);
	fxmul(ab[4], a[4], b[9]);
	fxmul(ab[8], a[8], b[10]);
	fxmul(ab[12], a[12], b[11]);
	fxadd(ab[0], ab[0], ab[4]);
	fxadd(ab[0], ab[0], ab[8]);
	fxadd(ab[0], ab[0], ab[12]);
	fxset(newm[8], ab[0]);

	//newm[9]  = a[1]  *b[8]  + a[5]  *b[9]  + a[9]  *b[10]  + a[13]  *b[11];
	fxmul(ab[1], a[1], b[8]);
	fxmul(ab[5], a[5], b[9]);
	fxmul(ab[9], a[9], b[10]);
	fxmul(ab[13], a[13], b[11]);
	fxadd(ab[1], ab[1], ab[5]);
	fxadd(ab[1], ab[1], ab[9]);
	fxadd(ab[1], ab[1], ab[13]);
	fxset(newm[9], ab[1]);

	//newm[10] = a[2]  *b[8]  + a[6]  *b[9]  + a[10]  *b[10] + a[14]  *b[11];
	fxmul(ab[2], a[2], b[8]);
	fxmul(ab[6], a[6], b[9]);
	fxmul(ab[10], a[10], b[10]);
	fxmul(ab[14], a[14], b[11]);
	fxadd(ab[2], ab[2], ab[6]);
	fxadd(ab[2], ab[2], ab[10]);
	fxadd(ab[2], ab[2], ab[14]);
	fxset(newm[10], ab[2]);

	//newm[11] = a[3]  *b[8]  + a[7]  *b[9]  + a[11]  *b[10] + a[15]  *b[11];
	fxmul(ab[3], a[3], b[8]);
	fxmul(ab[7], a[7], b[9]);
	fxmul(ab[11], a[11], b[10]);
	fxmul(ab[15], a[15], b[11]);
	fxadd(ab[3], ab[3], ab[7]);
	fxadd(ab[3], ab[3], ab[11]);
	fxadd(ab[3], ab[3], ab[15]);
	fxset(newm[11], ab[3]);

	///////////////////////

	//newm[12] = a[0]  *b[12] + a[4]  *b[13] + a[8]  *b[14]  + a[12]  *b[15];
	fxmul(ab[0], a[0], b[12]);
	fxmul(ab[4], a[4], b[13]);
	fxmul(ab[8], a[8], b[14]);
	fxmul(ab[12], a[12], b[15]);
	fxadd(ab[0], ab[0], ab[4]);
	fxadd(ab[0], ab[0], ab[8]);
	fxadd(ab[0], ab[0], ab[12]);
	fxset(newm[12], ab[0]);

	//newm[13] = a[1]  *b[12] + a[5]  *b[13] + a[9]  *b[14]  + a[13]  *b[15];
	fxmul(ab[1], a[1], b[12]);
	fxmul(ab[5], a[5], b[13]);
	fxmul(ab[9], a[9], b[14]);
	fxmul(ab[13], a[13], b[15]);
	fxadd(ab[1], ab[1], ab[5]);
	fxadd(ab[1], ab[1], ab[9]);
	fxadd(ab[1], ab[1], ab[13]);
	fxset(newm[13], ab[1]);

	//newm[14] = a[2]  *b[12] + a[6]  *b[13] + a[10]  *b[14] + a[14]  *b[15];
	fxmul(ab[2], a[2], b[12]);
	fxmul(ab[6], a[6], b[13]);
	fxmul(ab[10], a[10], b[14]);
	fxmul(ab[14], a[14], b[15]);
	fxadd(ab[2], ab[2], ab[6]);
	fxadd(ab[2], ab[2], ab[10]);
	fxadd(ab[2], ab[2], ab[14]);
	fxset(newm[14], ab[2]);

	//newm[15] = a[3]  *b[12] + a[7]  *b[13] + a[11]  *b[14] + a[15]  *b[15];
	fxmul(ab[3], a[3], b[12]);
	fxmul(ab[7], a[7], b[13]);
	fxmul(ab[11], a[11], b[14]);
	fxmul(ab[15], a[15], b[15]);
	fxadd(ab[3], ab[3], ab[7]);
	fxadd(ab[3], ab[3], ab[11]);
	fxadd(ab[3], ab[3], ab[15]);
	fxset(newm[15], ab[3]);

	///////////////////////

#endif
	
	mxset(m1, newm);
	
	for(i=0; i<16; ++i)
	{
		fxfree(newm[i]);
		fxfree(ab[i]);
	}
}

//used only for light matrix (and skeletal animation?), doesn't work with perspective projection because that is not an affine transformation
void mxpostmult2(mx *m1, mx *m2)
{
	fx newm[16], *a, *b, ab[16];
	char i;

	for(i=0; i<16; ++i)
	{
		fxinit(newm[i], FX_PREC);
		fxinit(ab[i], FX_PREC);
	}

	a = m1->matrix;
	b = m2->matrix;
	
#if 1

	///////////////////////
	
	//newm[0] = a[0]*b[0] + a[4]*b[1] + a[8]*b[2];
	fxmul(ab[0], a[0], b[0]);
	fxmul(ab[4], a[4], b[1]);
	fxmul(ab[8], a[8], b[2]);
	fxadd(ab[0], ab[0], ab[4]);
	fxadd(ab[0], ab[0], ab[8]);
	fxset(newm[0], ab[0]);

	//newm[1] = a[1]*b[0] + a[5]*b[1] + a[9]*b[2];
	fxmul(ab[1], a[1], b[0]);
	fxmul(ab[5], a[5], b[1]);
	fxmul(ab[9], a[9], b[2]);
	fxadd(ab[1], ab[1], ab[5]);
	fxadd(ab[1], ab[1], ab[9]);
	fxset(newm[1], ab[1]);

	//newm[2] = a[2]*b[0] + a[6]*b[1] + a[10]*b[2];
	fxmul(ab[2], a[2], b[0]);
	fxmul(ab[6], a[6], b[1]);
	fxmul(ab[10], a[10], b[2]);
	fxadd(ab[2], ab[2], ab[6]);
	fxadd(ab[2], ab[2], ab[10]);
	fxset(newm[2], ab[2]);

	//newm[3] = 0;
	fxsetri(newm[3], 0);

	///////////////////////
	
	//newm[4] = a[0]*b[4] + a[4]*b[5] + a[8]*b[6];
	fxmul(ab[0], a[0], b[4]);
	fxmul(ab[4], a[4], b[5]);
	fxmul(ab[8], a[8], b[6]);
	fxadd(ab[0], ab[0], ab[4]);
	fxadd(ab[0], ab[0], ab[8]);
	fxset(newm[4], ab[0]);

	//newm[5] = a[1]*b[4] + a[5]*b[5] + a[9]*b[6];
	fxmul(ab[1], a[1], b[4]);
	fxmul(ab[5], a[5], b[5]);
	fxmul(ab[9], a[9], b[6]);
	fxadd(ab[1], ab[1], ab[5]);
	fxadd(ab[1], ab[1], ab[9]);
	fxset(newm[5], ab[1]);

	//newm[6] = a[2]*b[4] + a[6]*b[5] + a[10]*b[6];
	fxmul(ab[2], a[2], b[4]);
	fxmul(ab[6], a[6], b[5]);
	fxmul(ab[10], a[10], b[6]);
	fxadd(ab[2], ab[2], ab[6]);
	fxadd(ab[2], ab[2], ab[10]);
	fxset(newm[6], ab[2]);

	//newm[7] = 0;
	fxsetri(newm[7], 0);

	///////////////////////
	
	//newm[8] = a[0]*b[8] + a[4]*b[9] + a[8]*b[10];
	fxmul(ab[0], a[0], b[8]);
	fxmul(ab[4], a[4], b[9]);
	fxmul(ab[8], a[8], b[10]);
	fxadd(ab[0], ab[0], ab[4]);
	fxadd(ab[0], ab[0], ab[8]);
	fxset(newm[8], ab[0]);

	//newm[9] = a[1]*b[8] + a[5]*b[9] + a[9]*b[10];
	fxmul(ab[1], a[1], b[8]);
	fxmul(ab[5], a[5], b[9]);
	fxmul(ab[9], a[9], b[10]);
	fxadd(ab[1], ab[1], ab[5]);
	fxadd(ab[1], ab[1], ab[9]);
	fxset(newm[9], ab[1]);

	//newm[10] = a[2]*b[8] + a[6]*b[9] + a[10]*b[10];
	fxmul(ab[2], a[2], b[8]);
	fxmul(ab[6], a[6], b[9]);
	fxmul(ab[10], a[10], b[10]);
	fxadd(ab[2], ab[2], ab[6]);
	fxadd(ab[2], ab[2], ab[10]);
	fxset(newm[10], ab[2]);

	//newm[11] = 0;
	fxsetri(newm[11], 0);

	///////////////////////
	
	//newm[12] = a[0]*b[12] + a[4]*b[13] + a[8]*b[14] + a[12];
	fxmul(ab[0], a[0], b[12]);
	fxmul(ab[4], a[4], b[13]);
	fxmul(ab[8], a[8], b[14]);
	fxadd(ab[0], ab[0], ab[4]);
	fxadd(ab[0], ab[0], ab[8]);
	fxset(newm[12], ab[0]);

	//newm[13] = a[1]*b[12] + a[5]*b[13] + a[9]*b[14] + a[13];
	fxmul(ab[1], a[1], b[12]);
	fxmul(ab[5], a[5], b[13]);
	fxmul(ab[9], a[9], b[14]);
	fxadd(ab[1], ab[1], ab[5]);
	fxadd(ab[1], ab[1], ab[9]);
	fxset(newm[13], ab[1]);

	//newm[14] = a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14];
	fxmul(ab[2], a[2], b[12]);
	fxmul(ab[6], a[6], b[13]);
	fxmul(ab[10], a[10], b[14]);
	fxadd(ab[2], ab[2], ab[6]);
	fxadd(ab[2], ab[2], ab[10]);
	fxset(newm[14], ab[2]);

	//newm[15] = 1;
	fxsetri(newm[15], 1);

	///////////////////////
	
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
	
	mxset(m1, newm);
	
	for(i=0; i<16; ++i)
	{
		fxfree(newm[i]);
		fxfree(ab[i]);
	}
}

void mxtransl(mx *m, fx *translation)
{
	fx *matrix;
	
	matrix = m->matrix;
	
#if 0
	matrix[0] = matrix[5] =  matrix[10] = matrix[15] = 1.0;
	matrix[1] = matrix[2] = matrix[3] = matrix[4] = 0.0;
	matrix[6] = matrix[7] = matrix[8] = matrix[9] = 0.0;
	matrix[11] = 0.0;

#elif 1

	//matrix[12] = translation[0];
	fxset(matrix[12], translation[0]);

	//matrix[13] = translation[1];
	fxset(matrix[13], translation[1]);

	//matrix[14] = translation[2];
	fxset(matrix[14], translation[2]);

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

void mxinvtrans(mx *m, fx *translation)
{
	fx *matrix;
	
	matrix = m->matrix;
	
	//matrix[12] = -translation[0];
	fxset(matrix[12], translation[0]);
	fxmul_ui(matrix[12], matrix[12], -1);

	//matrix[13] = -translation[1];
	fxset(matrix[13], translation[1]);
	fxmul_ui(matrix[13], matrix[13], -1);

	//matrix[14] = -translation[2];
	fxset(matrix[14], translation[2]);
	fxmul_ui(matrix[14], matrix[14], -1);
}

void mxscale(mx *m, fx *scale)
{
	fx *matrix;
	
	mxreset(m);

	matrix = m->matrix;
	
	//matrix[0] = scale[0];
	fxset(matrix[0], scale[0]);
	//matrix[5] = scale[1];
	fxset(matrix[5], scale[1]);
	//matrix[10] = scale[2];
	fxset(matrix[10], scale[2]);
	//matrix[15] = 1;
	fxsetri(matrix[15], 1);
}

void mxrotdeg(mx *m, fx *angles)
{
	fx vec[3], *matrix;
	fx pi;
	
	matrix = m->matrix;

	fxinit(vec[0], FX_PREC);
	fxinit(vec[1], FX_PREC);
	fxinit(vec[2], FX_PREC);
	fxinit(pi, FX_PREC);
	mpfr_const_pi(pi, MPFR_RNDN);
	
	//vec[0] = ( fx )( angles[0]*180.0/M_PI );
	fxmul_ui(vec[0], angles[0], 180);
	fxdiv_fr(vec[0], vec[0], mpfr_const_pi);

	//vec[1] = ( fx )( angles[1]*180.0/M_PI );
	fxmul_ui(vec[1], angles[1], 180);
	fxdiv_fr(vec[1], vec[1], mpfr_const_pi);

	//vec[2] = ( fx )( angles[2]*180.0/M_PI );
	fxmul_ui(vec[2], angles[2], 180);
	fxdiv_fr(vec[2], vec[2], mpfr_const_pi);
	
	mxrotrad(m, vec);

	fxfree(vec[0]);
	fxfree(vec[1]);
	fxfree(vec[2]);
	mpfr_clear(pi);
}

void mxinvrotdeg(mx *m, fx *angles)
{
	fx vec[3], *matrix;
	fx pi;
	
	matrix = m->matrix;

	fxinit(vec[0], FX_PREC);
	fxinit(vec[1], FX_PREC);
	fxinit(vec[2], FX_PREC);
	fxinit(pi, FX_PREC);
	mpfr_const_pi(pi, MPFR_RNDN);
	
	//vec[0] = ( fx )( angles[0]*180.0/M_PI );
	fxmul_ui(vec[0], angles[0], 180);
	fxdiv_fr(vec[0], vec[0], mpfr_const_pi);

	//vec[1] = ( fx )( angles[1]*180.0/M_PI );
	fxmul_ui(vec[1], angles[1], 180);
	fxdiv_fr(vec[1], vec[1], mpfr_const_pi);

	//vec[2] = ( fx )( angles[2]*180.0/M_PI );
	fxmul_ui(vec[2], angles[2], 180);
	fxdiv_fr(vec[2], vec[2], mpfr_const_pi);
	
	mxinvrotrad(m, vec);

	fxfree(vec[0]);
	fxfree(vec[1]);
	fxfree(vec[2]);
	mpfr_clear(pi);
}

void mxrotrad(mx *m, fx *angles)
{
	fx cr, sr, cp, sp, cy, sy, srsp, crsp;
	fx *matrix;
	fx srspcy, crsy, srspsy, crcy, srcp, crspcy, srsy, crspsy, srcy, crcp;
	
	matrix = m->matrix;

	fxinit(cr, FX_PREC);
	fxinit(sr, FX_PREC);
	fxinit(cp, FX_PREC);
	fxinit(sp, FX_PREC);
	fxinit(cy, FX_PREC);
	fxinit(sy, FX_PREC);
	fxinit(srsp, FX_PREC);
	fxinit(crsp, FX_PREC);

	fxinit(srspcy, FX_PREC);
	fxinit(crsy, FX_PREC);
	fxinit(srspsy, FX_PREC);
	fxinit(crcy, FX_PREC);
	fxinit(srcp, FX_PREC);
	fxinit(crspcy, FX_PREC);
	fxinit(srsy, FX_PREC);
	fxinit(crspsy, FX_PREC);
	fxinit(srcy, FX_PREC);
	fxinit(crcp, FX_PREC);
	
	fxcos( cr, angles[0], MPC_RNDNN );
	fxsin( sr, angles[0], MPC_RNDNN );
	fxcos( cp, angles[1], MPC_RNDNN );
	fxsin( sp, angles[1], MPC_RNDNN );
	fxcos( cy, angles[2], MPC_RNDNN );
	fxsin( sy, angles[2], MPC_RNDNN );

	//matrix[0] = ( fx )( cp*cy );
	fxmul(matrix[0], cp, cy);
	//matrix[1] = ( fx )( cp*sy );
	fxmul(matrix[1], cp, sy);
	//matrix[2] = ( fx )( -sp );
	fxsetri(matrix[2], 0);
	fxsub(matrix[2], matrix[2], sp);

	//srsp = sr*sp;
	fxmul(srsp, sr, sp);
	//crsp = cr*sp;
	fxmul(crsp, cr, sp);

	fxmul(srspcy, srsp, cy);
	fxmul(crsy, cr, sy);
	fxmul(srspsy, srsp, sy);
	fxmul(crcy, cr, cy);
	fxmul(srcp, sr, cp);
	fxmul(crspcy, crsp, cy);
	fxmul(srsy, sr, sy);
	fxmul(crspsy, crsp, sy);
	fxmul(srcy, sr, cy);
	fxmul(crcp, cr, cp);

	//matrix[4] = ( fx )( srsp*cy-cr*sy );
	fxsub(matrix[4], srspcy, crsy);
	//matrix[5] = ( fx )( srsp*sy+cr*cy );
	fxadd(matrix[5], srspsy, crcy);
	//matrix[6] = ( fx )( sr*cp );
	fxset(matrix[6], srcp);

	//matrix[8] = ( fx )( crsp*cy+sr*sy );
	fxadd(matrix[8], crspcy, srsy);
	//matrix[9] = ( fx )( crsp*sy-sr*cy );
	fxsub(matrix[9], crspsy, srcy);
	//matrix[10] = ( fx )( cr*cp );
	fxset(matrix[10], crcp);
	
	fxfree(cr);
	fxfree(sr);
	fxfree(cp);
	fxfree(sp);
	fxfree(cy);
	fxfree(sy);
	fxfree(srsp);
	fxfree(crsp);

	fxfree(srspcy);
	fxfree(crsy);
	fxfree(srspsy);
	fxfree(crcy);
	fxfree(srcp);
	fxfree(crspcy);
	fxfree(srsy);
	fxfree(crspsy);
	fxfree(srcy);
	fxfree(crcp);
}

void mxinvrotrad(mx *m, fx *angles)
{
	fx cr, sr, cp, sp, cy, sy, srsp, crsp;
	fx *matrix;
	fx srspcy, crsy, srspsy, crcy, srcp, crspcy, srsy, crspsy, srcy, crcp;
	
	matrix = m->matrix;

	fxinit(cr, FX_PREC);
	fxinit(sr, FX_PREC);
	fxinit(cp, FX_PREC);
	fxinit(sp, FX_PREC);
	fxinit(cy, FX_PREC);
	fxinit(sy, FX_PREC);
	fxinit(srsp, FX_PREC);
	fxinit(crsp, FX_PREC);

	fxinit(srspcy, FX_PREC);
	fxinit(crsy, FX_PREC);
	fxinit(srspsy, FX_PREC);
	fxinit(crcy, FX_PREC);
	fxinit(srcp, FX_PREC);
	fxinit(crspcy, FX_PREC);
	fxinit(srsy, FX_PREC);
	fxinit(crspsy, FX_PREC);
	fxinit(srcy, FX_PREC);
	fxinit(crcp, FX_PREC);
	
	fxcos( cr, angles[0], MPC_RNDNN );
	fxsin( sr, angles[0], MPC_RNDNN );
	fxcos( cp, angles[1], MPC_RNDNN );
	fxsin( sp, angles[1], MPC_RNDNN );
	fxcos( cy, angles[2], MPC_RNDNN );
	fxsin( sy, angles[2], MPC_RNDNN );

	//matrix[0] = ( fx )( cp*cy );
	fxmul(matrix[0], cp, cy);
	//matrix[4] = ( fx )( cp*sy );
	fxmul(matrix[4], cp, sy);
	//matrix[8] = ( fx )( -sp );
	fxsetri(matrix[8], 0);
	fxsub(matrix[8], matrix[8], sp);

	//srsp = sr*sp;
	fxmul(srsp, sr, sp);
	//crsp = cr*sp;
	fxmul(crsp, cr, sp);

	fxmul(srspcy, srsp, cy);
	fxmul(crsy, cr, sy);
	fxmul(srspsy, srsp, sy);
	fxmul(crcy, cr, cy);
	fxmul(srcp, sr, cp);
	fxmul(crspcy, crsp, cy);
	fxmul(srsy, sr, sy);
	fxmul(crspsy, crsp, sy);
	fxmul(srcy, sr, cy);
	fxmul(crcp, cr, cp);

	//matrix[1] = ( fx )( srsp*cy-cr*sy );
	fxsub(matrix[1], srspcy, crsy);
	//matrix[5] = ( fx )( srsp*sy+cr*cy );
	fxadd(matrix[5], srspsy, crcy);
	//matrix[9] = ( fx )( sr*cp );
	fxset(matrix[9], srcp);

	//matrix[2] = ( fx )( crsp*cy+sr*sy );
	fxadd(matrix[2], crspcy, srsy);
	//matrix[6] = ( fx )( crsp*sy-sr*cy );
	fxsub(matrix[6], crspsy, srcy);
	//matrix[10] = ( fx )( cr*cp );
	fxset(matrix[10], crcp);
	
	fxfree(cr);
	fxfree(sr);
	fxfree(cp);
	fxfree(sp);
	fxfree(cy);
	fxfree(sy);
	fxfree(srsp);
	fxfree(crsp);

	fxfree(srspcy);
	fxfree(crsy);
	fxfree(srspsy);
	fxfree(crcy);
	fxfree(srcp);
	fxfree(crspcy);
	fxfree(srsy);
	fxfree(crspsy);
	fxfree(srcy);
	fxfree(crcp);
}

void mxrotquat(mx *m, qx *q)
{
	fx *matrix, *quat;
	fx qm[16];
	char i, r, c;

	quat = q->quat;
	matrix = m->matrix;

#define M(row,col)  (col*4+row)

	for(r=0; r<4; ++r)
	{
		for(c=0; c<4; ++c)
		{
			fxinit(qm[i], FX_PREC);
			fxsetri(qm[i], 2);
			fxmul(qm[i], quat[r], quat[c]); 
		}
	}
	
	//matrix[0] = ( fx )( 1.0 - 2.0*quat[1]*quat[1] - 2.0*quat[2]*quat[2] );
	fxsetri(matrix[0], 1);
	fxsub(matrix[0], matrix[0], qm[M(1,1)]);
	fxsub(matrix[0], matrix[0], qm[M(2,2)]);
	//matrix[1] = ( fx )( 2.0*quat[0]*quat[1] + 2.0*quat[3]*quat[2] );
	fxset(matrix[1], qm[M(0,1)]);
	fxadd(matrix[1], matrix[1], qm[M(3,2)]);
	//matrix[2] = ( fx )( 2.0*quat[0]*quat[2] - 2.0*quat[3]*quat[1] );
	fxset(matrix[2], qm[M(0,2)]);
	fxadd(matrix[2], matrix[2], qm[M(3,1)]);

	//matrix[4] = ( fx )( 2.0*quat[0]*quat[1] - 2.0*quat[3]*quat[2] );
	fxset(matrix[4], qm[M(0,1)]);
	fxsub(matrix[4], matrix[4], qm[M(3,2)]);
	//matrix[5] = ( fx )( 1.0 - 2.0*quat[0]*quat[0] - 2.0*quat[2]*quat[2] );
	fxsetri(matrix[5], 1);
	fxsub(matrix[5], matrix[5], qm[M(0,0)]);
	fxsub(matrix[5], matrix[5], qm[M(2,2)]);
	//matrix[6] = ( fx )( 2.0*quat[1]*quat[2] + 2.0*quat[3]*quat[0] );
	fxset(matrix[6], qm[M(1,2)]);
	fxadd(matrix[6], matrix[6], qm[M(3,0)]);

	//matrix[8] = ( fx )( 2.0*quat[0]*quat[2] + 2.0*quat[3]*quat[1] );
	fxset(matrix[8], qm[M(0,2)]);
	fxadd(matrix[8], matrix[8], qm[M(3,1)]);
	//matrix[9] = ( fx )( 2.0*quat[1]*quat[2] - 2.0*quat[3]*quat[0] );
	fxset(matrix[9], qm[M(1,2)]);
	fxsub(matrix[9], matrix[9], qm[M(3,0)]);
	//matrix[10] = ( fx )( 1.0 - 2.0*quat[0]*quat[0] - 2.0*quat[1]*quat[1] );
	fxsetri(matrix[10], 1);
	fxsub(matrix[10], matrix[10], qm[M(0,0)]);
	fxsub(matrix[10], matrix[10], qm[M(1,1)]);
	
#undef M

	for(i=0; i<16; ++i)
	{
		fxfree(qm[i]);
	}
}

#endif