#include "../sys/inc.h"
#include "matf.h"
#include "v4f.h"
#include "v3f.h"

void v4ftransform(v4f *v, const mf *m)
{
	double vec[4];
	float *matrix, x, y, z;
 
	matrix = (float*)m->matrix;
	x = v->x;
	y = v->y;
	z = v->z;

	vec[0] = x*matrix[0]+y*matrix[4]+z*matrix[8]+matrix[12];
	vec[1] = x*matrix[1]+y*matrix[5]+z*matrix[9]+matrix[13];
	vec[2] = x*matrix[2]+y*matrix[6]+z*matrix[10]+matrix[14];
	vec[3] = x*matrix[3]+y*matrix[7]+z*matrix[11]+matrix[15];

	v->x = vec[0];
	v->y = vec[1];
	v->z = vec[2];
	v->w = vec[3];
}

void v4ftransform3(v4f *v, const mf *m)
{
	double vec[3];
	float *matrix, x, y, z;
 
	matrix = (float*)m->matrix;
	x = v->x;
	y = v->y;
	z = v->z;

	vec[0] = x*matrix[0]+y*matrix[4]+z*matrix[8];
	vec[1] = x*matrix[1]+y*matrix[5]+z*matrix[9];
	vec[2] = x*matrix[2]+y*matrix[6]+z*matrix[10];
	
	v->x = vec[0];
	v->y = vec[1];
	v->z = vec[2];
}

void v4freset(v4f *v)
{
	v->x = v->y = v->z = v->w = 0;
}

void v4fset(v4f *v, float x, float y, float z, float w)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

void v4fset2(v4f *v, const float *values)
{
	v->x = values[0];
	v->y = values[1];
	v->z = values[2];
	v->w = values[3];
}

dbool v4feq(v4f v1, v4f v2)
{
	if(v1.x == v2.x &&
	   v1.y == v2.y &&
	   v1.z == v2.z &&
	   v1.w == v2.w)
		return dtrue;
	
	return dfalse;
}

dbool v4fneq(v4f v1, v4f v2)
{
	if(v1.x == v2.x &&
	   v1.y == v2.y &&
	   v1.z == v2.z &&
	   v1.w == v2.w)
		return dfalse;
	
	return dtrue;
}

void v4fadd(v4f *v, v4f v1, v4f v2)
{
	v->x = v1.x + v2.x;
	v->y = v1.y + v2.y;
	v->z = v1.z + v2.z;
	v->w = v1.w + v2.w;
}

void v4fsub(v4f *v, v4f v1, v4f v2)
{
	v->x = v1.x - v2.x;
	v->y = v1.y - v2.y;
	v->z = v1.z - v2.z;
	v->w = v1.w - v2.w;
}

void v4fmulf(v4f *v, v4f v1, const float num)
{
	v->x = v1.x * num;
	v->y = v1.y * num;
	v->z = v1.z * num;
	v->w = v1.w * num;
}

void v4fmulv(v4f *v, const v4f v1, v4f v2)
{
	v->x = v1.x * v2.x;
	v->y = v1.y * v2.y;
	v->z = v1.z * v2.z;
	v->w = v1.w * v2.w;
}

void v4fdiv(v4f *v, const v4f v1, const float num)
{
	v->x = v1.x / num;
	v->y = v1.y / num;
	v->z = v1.z / num;
	v->w = v1.w / num;
}