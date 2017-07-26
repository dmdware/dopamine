#include "v3f.h"
#include "plane3f.h"
#include "3dmath.h"
#include "../sys/utils.h"

#if 0

p3f::p3f()
{
}

p3f::p3f(float x, float y, float z, float d)
{
	normal.x = x;
	normal.y = y;
	normal.z = z;
	d = d;
}

p3f::~p3f()
{
}

dbool Close(p3f a, p3f b)
{
	if(fabs(a.normal.x - b.normal.x) <= CLOSE_EPSILON && fabs(a.normal.y - b.normal.y) <= CLOSE_EPSILON && fabs(a.normal.z - b.normal.z) <= CLOSE_EPSILON && fabs(a.d - b.d) <= CLOSE_EPSILON)
		return dtrue;

	if(fabs(-a.normal.x - b.normal.x) <= CLOSE_EPSILON && fabs(-a.normal.y - b.normal.y) <= CLOSE_EPSILON && fabs(-a.normal.z - b.normal.z) <= CLOSE_EPSILON && fabs(-a.d - b.d) <= CLOSE_EPSILON)
		return dtrue;

	return dfalse;
}

//#define PLANE_DEBUG

#endif

v3f pop(p3f p)
{
	//Ax + By + Cz + D = 0
	//x = -D/A	if(A != 0)
	//y = -D/B	if(B != 0)
	//z = -D/C	if(C != 0)

	v3f v;

#ifdef PLANE_DEBUG
	g_log<<"point on plane ("<<p.normal.x<<","<<p.normal.y<<","<<p.normal.z<<"),"<<p.d<<std::endl;
	g_log.flush();
#endif

	int greatest = -1;
	float greatestd = 0;

	if(greatest < 0 || fabs(p.normal.x) > greatestd)
	{
		greatest = 0;
		greatestd = fabs(p.normal.x);
	}
	if(greatest < 0 || fabs(p.normal.y) > greatestd)
	{
		greatest = 1;
		greatestd = fabs(p.normal.y);
	}
	if(greatest < 0 || fabs(p.normal.x) > greatestd)
	{
		greatest = 2;
		greatestd = fabs(p.normal.z);
	}

	if(fabs(p.normal.x) > EPSILON)
		//if(greatest == 0)
	//	return v3f(- p.d / p.normal.x, 0, 0);
	{
		//if(greatest == 2)
		v.x = v.y = v.z = 0;
		v.x = - p.d / p.normal.x;
		return v;
	}

	if(fabs(p.normal.y) > EPSILON)
		//if(greatest == 1)
	//	return v3f(0, - p.d / p.normal.y, 0);
	{
		//if(greatest == 2)
		v.x = v.y = v.z = 0;
		v.y = - p.d / p.normal.y;
		return v;
	}

	if(fabs(p.normal.z) > EPSILON)
	{
		//if(greatest == 2)
		v.x = v.y = v.z = 0;
		v.z = - p.d / p.normal.z;
		return v;
	}

	v.x = v.y = v.z = 0;

	return v;
}

#if 0

float PlaneDistance(v3f normal, v3f point)
{
	float distance = 0; // This variable holds the distance from the plane to the origin

	// Use the plane equation to find the distance (Ax + By + Cz + D = 0)  We want to find D.
	// So, we come up with D = -(Ax + By + Cz)
	// Basically, the negated dot product of the normal of the plane and the point.
	distance = - (normal.x * point.x + normal.y * point.y + normal.z * point.z);

	return distance;
}

dbool PointBehindPlane(v3f point, p3f plane)
{
	float result = point.x*plane.normal.x + point.y*plane.normal.y + point.z*plane.normal.z + plane.d;

	if(result <= 0)
		return dtrue;

	return dfalse;
}

dbool PointOnOrBehindPlane(v3f point, p3f plane, float epsilon)
{
	float result = point.x*plane.normal.x + point.y*plane.normal.y + point.z*plane.normal.z + plane.d;

	if(result <= epsilon)
		return dtrue;

	return dfalse;
}

dbool PointOnOrBehindPlane(v3f point, v3f normal, float dist, float epsilon)
{
	float result = point.x*normal.x + point.y*normal.y + point.z*normal.z + dist;

	if(result <= epsilon)
		return dtrue;

	return dfalse;
}

void RotatePlane(p3f& p, v3f about, float radians, v3f axis)
{
	v3f pop = pop(p);
	pop = rotab3f(pop, about, radians, axis.x, axis.y, axis.z);
	p.normal = rot3f(p.normal, radians, axis.x, axis.y, axis.z);
	p.d = PlaneDistance(p.normal, pop);
}

// http://thejuniverse.org/PUBLIC/LinearAlgebra/LOLA/planes/std.html
void MakePlane(v3f *norm, float *d, v3f point, v3f setnorm)
{
	*norm = setnorm;
	*d = -dot3f(setnorm, point);
}

// Parametric line
void ParamLine(v3f *line, v3f *change)
{
	(*change) = line[1] - line[0];
	//(*change) = tnorm(*change);

}

// line intersects plane?
dbool Lineintpl(const v3f *line, const v3f norm, const float d, v3f *inter)
{
	v3f change = line[1] - line[0];
	//ParamLine(line, &change);

	float denom = dot3f(norm, change);

	if(fabs(denom) <= EPSILON)
		return dfalse;

	float SegScalar = (d - dot3f(norm, line[0])) / denom;

	//TODO: Check if SegScalar is [0.0, 1.0]?
	if(SegScalar < 0.0f)
		return dfalse;

	*inter = change * SegScalar + line[0];

	return dtrue;
}

#endif
