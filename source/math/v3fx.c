










#include "v3fx.h"\
	
#if 0

void v3xinit(v3x *v)
{
	fxinit(&v->x);
	fxinit(&v->y);
	fxinit(&v->z);

	fxsetri(&v->x, 0);
	fxsetri(&v->y, 0);
	fxsetri(&v->z, 0);
}

void v3xfree(v3x *v)
{
	fxfree(&v->x);
	fxfree(&v->y);
	fxfree(&v->z);
}

void v3xtransform(v3x *v, fx *m)
{
	fx vec[16];
	char i;

	for(i=0; i<16; ++i)
	{
		fxinit(&vec[i]);
	}

	//vec[0] = v->x*m[0]+v->y*m[4]+v->z*m[8]+m[12];
	fxmul(&vec[0], &m[0], &v->x);
	fxmul(&vec[4], &m[4], &v->y);
	fxmul(&vec[8], &m[8], &v->z);
	fxadd(&vec[0], &vec[0], &vec[4]);
	fxadd(&vec[0], &vec[0], &vec[8]);
	fxadd(&vec[0], &vec[0], &m[12]);

	//vec[1] = v->x*m[1]+v->y*m[5]+v->z*m[9]+m[13];
	fxmul(&vec[1], &m[1], &v->x);
	fxmul(&vec[5], &m[5], &v->y);
	fxmul(&vec[9], &m[9], &v->z);
	fxadd(&vec[1], &vec[1], &vec[5]);
	fxadd(&vec[1], &vec[1], &vec[9]);
	fxadd(&vec[1], &vec[1], &m[13]);

	//vec[2] = v->x*m[2]+v->y*m[6]+v->z*m[10]+m[14];
	fxmul(&vec[2], &m[2], &v->x);
	fxmul(&vec[6], &m[6], &v->y);
	fxmul(&vec[10], &m[10], &v->z);
	fxadd(&vec[2], &vec[2], &vec[6]);
	fxadd(&vec[2], &vec[2], &vec[10]);
	fxadd(&vec[2], &vec[2], &m[14]);

	//vec[3] = v->x*m[3]+v->y*m[7]+v->z*m[11]+m[15];
	
	fxset(&v->x, &vec[0]);
	fxset(&v->y, &vec[1]);
	fxset(&v->z, &vec[2]);

	for(i=0; i<16; ++i)
	{
		fxfree(&vec[i]);
	}
}

void v3xtransform3(v3x *v, fx *m)
{
	fx vec[16];
	char i;

	for(i=0; i<16; ++i)
	{
		fxinit(&vec[i]);
	}

	//vec[0] = v->x*m[0]+v->y*m[4]+v->z*m[8];
	fxmul(&vec[0], &m[0], &v->x);
	fxmul(&vec[4], &m[4], &v->y);
	fxmul(&vec[8], &m[8], &v->z);
	fxadd(&vec[0], &vec[0], &vec[4]);
	fxadd(&vec[0], &vec[0], &vec[8]);

	//vec[1] = v->x*m[1]+v->y*m[5]+v->z*m[9];
	fxmul(&vec[1], &m[1], &v->x);
	fxmul(&vec[5], &m[5], &v->y);
	fxmul(&vec[9], &m[9], &v->z);
	fxadd(&vec[1], &vec[1], &vec[5]);
	fxadd(&vec[1], &vec[1], &vec[9]);

	//vec[2] = v->x*m[2]+v->y*m[6]+v->z*m[10];
	fxmul(&vec[2], &m[2], &v->x);
	fxmul(&vec[6], &m[6], &v->y);
	fxmul(&vec[10], &m[10], &v->z);
	fxadd(&vec[2], &vec[2], &vec[6]);
	fxadd(&vec[2], &vec[2], &vec[10]);

	fxset(&v->x, &vec[0]);
	fxset(&v->y, &vec[1]);
	fxset(&v->z, &vec[2]);

	for(i=0; i<16; ++i)
	{
		fxfree(&vec[i]);
	}
}

void v3xmul(v3x *v, v3x f, fx s)
{
	fxmul(&v->x, &f.x, &s);
	fxmul(&v->y, &f.y, &s);
	fxmul(&v->z, &f.z, &s);
}

void v3xsub(v3x *v, v3x f, v3x s)
{
	fxsub(&v->x, &f.x, &s.x);
	fxmul(&v->y, &f.y, &s.y);
	fxmul(&v->z, &f.z, &s.z);
}

void v3xadd(v3x *v, v3x f, v3x s)
{
	fxadd(&v->x, &f.x, &s.x);
	fxadd(&v->y, &f.y, &s.y);
	fxadd(&v->z, &f.z, &s.z);
}

void v3xdot(v3x v, fx *d)
{
	v3x dv;

	v3xinit(&dv);

	fxmul(&dv.x, &v.x, &v.x);
	fxmul(&dv.y, &v.y, &v.y);
	fxmul(&dv.z, &v.z, &v.z);

	fxadd(d, &dv.x, &dv.y);
	fxadd(d, d, &dv.z);

	v3xfree(&dv);
}

#endif