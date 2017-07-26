#ifndef PLANE3F_H
#define PLANE3F_H

#include "v3f.h"


struct p3f
{
	v3f normal;
	float d;
};

typedef struct p3f p3f;

#if 0
//TODO make c

struct p3f
{
public:
	v3f normal;
	float d;

	dbool operator==(const p3f p) const
	{
		//if(fabs(normal.x - p.normal.x) <= EPSILON && fabs(normal.y - p.normal.y) <= EPSILON && fabs(normal.z - p.normal.z) <= EPSILON && fabs(d - p.d) <= EPSILON)
		//	return dtrue;

		if(normal.x == p.normal.x && normal.y == p.normal.y && normal.z == p.normal.z && d == p.d)
			return dtrue;

		return dfalse;
	}

	p3f();
	p3f(float x, float y, float z, float d);
	~p3f();
};

dbool Close(p3f a, p3f b);
float PlaneDistance(v3f normal, v3f point);
dbool PointBehindPlane(v3f point, p3f plane);
dbool PointOnOrBehindPlane(v3f point, p3f plane, float epsilon=CLOSE_EPSILON);
dbool PointOnOrBehindPlane(v3f point, v3f normal, float dist, float epsilon=CLOSE_EPSILON);
void RotatePlane(p3f& p, v3f about, float radians, v3f axis);
void MakePlane(v3f *norm, float *d, v3f point, v3f setnorm);
void ParamLine(v3f *line, v3f *change);
dbool Lineintpl(const v3f *line, const v3f norm, const float d, v3f *inter);

#endif

v3f pop(p3f p);

#endif
