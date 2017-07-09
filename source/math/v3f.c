










#include "v3f.h"

void v3ftransform(v3f *v, const float *m)
{
	float vec[4];

	vec[0] = v->x*m[0]+v->y*m[4]+v->z*m[8]+m[12];
	vec[1] = v->x*m[1]+v->y*m[5]+v->z*m[9]+m[13];
	vec[2] = v->x*m[2]+v->y*m[6]+v->z*m[10]+m[14];
	vec[3] = v->x*m[3]+v->y*m[7]+v->z*m[11]+m[15];

	v->x = vec[0];
	v->y = vec[1];
	v->z = vec[2];
}

void v3ftransform3(v3f *v, const float *m)
{
	float vec[3];

	vec[0] = v->x*m[0]+v->y*m[4]+v->z*m[8];
	vec[1] = v->x*m[1]+v->y*m[5]+v->z*m[9];
	vec[2] = v->x*m[2]+v->y*m[6]+v->z*m[10];

	v->x = vec[0];
	v->y = vec[1];
	v->z = vec[2];
}

void v3fmul(v3f *v, v3f f, float s)
{
	v->x = f.x * s;
	v->y = f.y * s;
	v->z = f.z * s;
}

void v3fsub(v3f *v, v3f f, v3f s)
{
	v->x = f.x - s.x;
	v->y = f.y - s.y;
	v->z = f.z - s.z;
}

void v3fadd(v3f *v, v3f f, v3f s)
{
	v->x = f.x + s.x;
	v->y = f.y + s.y;
	v->z = f.z + s.z;
}