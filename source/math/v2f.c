

#include "v2f.h"

void v2freset(v2f *v)
{
	v->x = v->y = 0;
}

void v2fset(v2f *v, float x, float y)
{
	v->x = x;
	v->y = y;
}

void v2fset2(v2f *v, const float *values)
{
	v->x = values[0];
	v->y = values[1];
}

dbool v2feq(v2f v1, v2f v2)
{
	if(v1.x == v2.x &&
	   v1.y == v2.y)
		return dtrue;
	
	return dfalse;
}

dbool v2fneq(v2f v1, v2f v2)
{
	if(v1.x == v2.x &&
	   v1.y == v2.y)
		return dfalse;
	
	return dtrue;
}

void v2fadd(v2f *v, v2f v1, v2f v2)
{
	v->x = v1.x + v2.x;
	v->y = v1.y + v2.y;
}

void v2fsub(v2f *v, v2f v1, v2f v2)
{
	v->x = v1.x - v2.x;
	v->y = v1.y - v2.y;
}

void v2fmulf(v2f *v, v2f v1, const float num)
{
	v->x = v1.x * num;
	v->y = v1.y * num;
}

void v2fmulv(v2f *v, const v2f v1, v2f v2)
{
	v->x = v1.x * v2.x;
	v->y = v1.y * v2.y;
}

void v2fdiv(v2f *v, const v2f v1, const float num)
{
	v->x = v1.x / num;
	v->y = v1.y / num;
}