#include "3dmath.h"
#include "../sys/utils.h"
#include "../sys/syswin.h"
#include "../gui/gui.h"
#include "v3f.h"
#include "camf.h"
#include "quatf.h"
#include "v4f.h"
#include "matf.h"
#include "v3i.h"
#include "plane3f.h"
#include "../sys/syswin.h"
#include "../sim/user.h"

float clipf(float n, float lower, float upper)
{
	return maxf(lower, minf(n, upper));
}

int clipi(int n, int lower, int upper)
{
	return maxi(lower, mini(n, upper));
}

v3f vmin(float mf, v3f v)
{
	v.x = minf(mf, v.x);
	v.y = minf(mf, v.y);
	v.z = minf(mf, v.z);
	return v;
}

float mag3f(v3f v)
{
	return (float)sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float mag2f(v2f v)
{
	return (float)sqrtf((v.x * v.x) + (v.y * v.y));
}

float dot3f3(v3f v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

//TODO these funcs need better names

float dot3f2(v3f v)
{
	return (v.x * v.x) + (v.z * v.z);
}

int mag2i(v2i v)
{
	return isqrt(v.x*v.x + v.y*v.y);
}

int dot2i2(v2i v)
{
	return v.x*v.x + v.y*v.y;
}

// manhattan distance
int man2i(v2i v)
{
	//return std::max(abs(vec.x), abs(vec.y));
	return iabs(v.x) + iabs(v.y);
}

v3f norm3f(v3f n)
{
	float m;

	m = mag3f(n);

	n.x /= m;
	n.y /= m;
	n.z /= m;

	return n;
}

v2f norm2f(v2f n)
{
	float m;

	m = mag2f(n);

	n.x /= m;
	n.y /= m;

	return n;
}

v3f cross3f(v3f v1, v3f v2)
{
	v3f n;

	n.x = ((v1.y * v2.z) - (v1.z * v2.y));
	n.y = ((v1.z * v2.x) - (v1.x * v2.z));
	n.z = ((v1.x * v2.y) - (v1.y * v2.x));

	return n;
}


v3f tov3f(v3f p1, v3f p2)
{
	v3f v;

	v.x = p1.x - p2.x;
	v.y = p1.y - p2.y;
	v.z = p1.z - p2.z;

	return v;
}

// Clockwise
v3f tnorm(v3f t[])
{
	v3f v1;
	v3f v2;
	v3f n;

	v1 = tov3f(t[2], t[0]);
	v2 = tov3f(t[1], t[0]);

	n = cross3f(v1, v2);

	n = norm3f(n);

	return n;
}

// Counter-clockwise
v3f tnorm2(v3f t[])
{
	v3f v1;
	v3f v2;
	v3f n;

	v1 = tov3f(t[2], t[0]);
	v2 = tov3f(t[1], t[0]);

	n = cross3f(v2, v1);

	n = norm3f(n);

	return n;
}

dbool intpg(v3f poly[], v3f line[], v3f *n, float *origindist)
{
#if 0 //TODO
	float d1 = 0, d2 = 0;						// The distances from the 2 points of the line from the plane

	*n = tnorm(poly);

	// Let's find the distance our plane is from the origin.  We can find this value
	// from the normal to the plane (polygon) and any point that lies on that plane (Any vertice)
	origindist = PlaneDistance(n, poly[0]);

	// Get the distance from point1 from the plane using: Ax + By + Cz + D = (The distance from the plane)

	d1 = ((n->x * line[0].x) +					// Ax +
		(n->y * line[0].y) +					// Bx +
		(n->z * line[0].z)) + origindist;	// Cz + D

// Get the distance from point2 from the plane using Ax + By + Cz + D = (The distance from the plane)

	d2 = ((n->x * line[1].x) +					// Ax +
		(n->y * line[1].y) +					// Bx +
		(n->z * line[1].z)) + origindist;	// Cz + D

// Now that we have 2 distances from the plane, if we times them together we either
// get a positive or negative number.  If it's a negative number, that means we collided!
// This is because the 2 points must be on either side of the plane (IE. -1 * 1 = -1).

	if (d1 * d2 >= 0)			// Check to see if both point's distances are both negative or both positive
		return dfalse;
#endif
	return dtrue;
}

// http://en.wikipedia.org/wiki/Line-plane_intersection
dbool intpl2(v3f l0, v3f l, p3f p, v3f *inter)
{
	v3f p0;
	float denom, num;

	denom = dot3f(l, p.normal);

	if (fabs(denom) <= EPSILON / 2.0f)
		//if(denom == 0.0f)
		return dfalse;

	p0 = pop(p);

	p0.x -= l0.x;
	p0.y -= l0.y;
	p0.z -= l0.z;

	num = dot3f(p0, p.normal);

	inter->x = l0.x + l.x * num / denom;
	inter->y = l0.y + l.z * num / denom;
	inter->z = l0.z + l.z * num / denom;

	return dtrue;
}

float dot3f(v3f v1, v3f v2)
{
	return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

int dot3i(v3i v1, v3i v2)
{
	return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

int dot2i(v2i v1, v2i v2)
{
	return ((v1.x * v2.x) + (v1.y * v2.y));
}

double angle3f(v3f v1, v3f v2)
{
	float dot, vmag;
	double angle;

	dot = dot3f(v1, v2);
	vmag = mag3f(v1) * mag3f(v2);
	angle = acos(dot / vmag);

	if (_isnan(angle))
		return 0;

	return(angle);
}

dbool inyaw3f(c3f *c, v3f p, float angle)
{
	v3f d;
	float yaw, yaw2, yaw3;

	yaw = yawf(d.x, d.z);
	yaw2 = yaw - DEGTORAD(360.0f);
	yaw3 = yaw + DEGTORAD(360.0f);

	d.x = p.x - c->pos.x;
	d.y = p.y - c->pos.y;
	d.z = p.z - c->pos.z;

	if (fabs(yaw3f(c) - yaw) <= angle ||
		fabs(yaw3f(c) - yaw2) <= angle ||
		fabs(yaw3f(c) - yaw3) <= angle)
		return dtrue;

	return dfalse;
}

#if 0
float DYaw(c3f *c, v3f p)
{
	v3f d;
	float yaw, yaw2, yaw3, dyaw, dyaw2, dyaw3, mindyaw;

	d.x = p.x - c->pos.x;
	d.y = p.y - c->pos.y;
	d.z = p.z - c->pos.z;
	yaw = yawf(d.x, d.z);
	yaw2 = yaw - DEGTORAD(360.0f);
	yaw3 = yaw + DEGTORAD(360.0f);

	dyaw = yaw - yaw3f(c);
	dyaw2 = yaw2 - c->yaw();
	dyaw3 = yaw3 - c->yaw();

	mindyaw = dyaw;

	if (fabs(dyaw2) < fabs(mindyaw))
		mindyaw = dyaw2;

	if (fabs(dyaw3) < fabs(mindyaw))
		mindyaw = dyaw3;

	return mindyaw;
}
#endif
v3f intpl(v3f n, v3f line[], double d)
{
	v3f p, ldir;
	double num, denom, d2;

	num = denom = d2 = 0.0;

	// Here comes the confusing part.  We need to find the 3D point that is actually
	// on the plane.  Here are some steps to do that:

	// 1)  First we need to get the vector of our line, Then normalize it so it's a length of 1
	ldir = tov3f(line[1], line[0]);		// Get the vector of the line
	ldir = norm3f(ldir);				// Normalize the lines vector


	// 2) Use the plane equation (distance = Ax + By + Cz + D) to find the distance from one of our points to the plane.
	//    Here I just chose a arbitrary point as the point to find that distance.  You notice we negate that
	//    distance.  We negate the distance because we want to eventually go BACKWARDS from our point to the plane.
	//    By doing this is will basically bring us back to the plane to find our intersect point.
	num = -(n.x * line[0].x +		// Use the plane equation with the normal and the line
		n.y * line[0].y +
		n.z * line[0].z + d);

	// 3) If we take the dot product between our line vector and the normal of the polygon,
	//    this will give us the cosine of the angle between the 2 (since they are both normalized - length 1).
	//    We will then divide our Numerator by this value to find the offset towards the plane from our arbitrary point.
	denom = dot3f(n, ldir);		// Get the dot product of the line's vector and the normal of the plane

	// Since we are using division, we need to make sure we don't get a divide by zero error
	// If we do get a 0, that means that there are INFINATE points because the the line is
	// on the plane (the normal is perpendicular to the line - (norm.vector = 0)).
	// In this case, we should just return any point on the line.

	if (denom == 0.0)						// Check so we don't divide by zero
		return line[0];						// Return an arbitrary point on the line

	// We divide the (distance from the point to the plane) by (the dot product)
	// to get the distance (dist) that we need to move from our arbitrary point.  We need
	// to then times this distance (dist) by our line's vector (direction).  When you times
	// a scalar (single number) by a vector you move along that vector.  That is what we are
	// doing.  We are moving from our arbitrary point we chose from the line BACK to the plane
	// along the lines vector.  It seems logical to just get the numerator, which is the distance
	// from the point to the line, and then just move back that much along the line's vector.
	// Well, the distance from the plane means the SHORTEST distance.  What about in the case that
	// the line is almost parallel with the polygon, but doesn't actually intersect it until half
	// way down the line's length.  The distance from the plane is short, but the distance from
	// the actual intersect point is pretty long.  If we divide the distance by the dot product
	// of our line vector and the normal of the plane, we get the correct length.  Cool huh?

	d2 = num / denom;				// Divide to get the multiplying (percentage) factor

	// Now, like we said above, we times the dist by the vector, then add our arbitrary point.
	// This essentially moves the point along the vector to a certain distance.  This now gives
	// us the intersect point.  Yay!

	p.x = (float)(line[0].x + (ldir.x * d2));
	p.y = (float)(line[0].y + (ldir.y * d2));
	p.z = (float)(line[0].z + (ldir.z * d2));

	return p;								// Return the intersect point
}

v3f rot3f(v3f v, float rad, float x, float y, float z)
{
	v3f newv;
	float costheta, sintheta;
	costheta = (float)cos(rad);
	sintheta = (float)sin(rad);

	newv.x = (costheta + (1 - costheta) * x * x)		 *v.x;
	newv.x += ((1 - costheta) * x * y - z * sintheta)	 *v.y;
	newv.x += ((1 - costheta) * x * z + y * sintheta)	 *v.z;

	newv.y = ((1 - costheta) * x * y + z * sintheta)	 *v.x;
	newv.y += (costheta + (1 - costheta) * y * y)		 *v.y;
	newv.y += ((1 - costheta) * y * z - x * sintheta)	 *v.z;

	newv.z = ((1 - costheta) * x * z - y * sintheta)	 *v.x;
	newv.z += ((1 - costheta) * y * z + x * sintheta)	 *v.y;
	newv.z += (costheta + (1 - costheta) * z * z)		 *v.z;

	return newv;
}

v3f rotab3f(v3f v, v3f around, float rad, float x, float y, float z)
{
	v.x -= around.x;
	v.y -= around.y;
	v.z -= around.z;
	v = rot3f(v, rad, x, y, z);
	v.x += around.x;
	v.y += around.y;
	v.z += around.z;
	return v;
}

float yawf(float dx, float dz)
{
	return RADTODEG(atan2(dx, dz));
}

/*
// http://webglfactory.blogspot.ca/2011/06/how-to-create-view-matrix.html
mf lookAt(v3f eye, v3f target, v3f up)
{
		 v3f vz= tnorm(eye - target);
		 v3f vx = normalize(crossProduct(up, vz));
		 // vy doesn't need to be normalized because it's a cross
		 // product of 2 normalized vectors
		 v3f vy = crossProduct(vz, vx);
		 mf inverseViewmf = mf(v4f(vx, 0), v4f(vy, 0), v4f(vz, 0), v4f(eye, 1));
		 return inverseViewmf.getInverse();
}
*/


mf lookat(float eyex, float eyey, float eyez,
	float centerx, float centery, float centerz,
	float upx, float upy, float upz)
{
	float m[16];
	float x[3], y[3], z[3];
	float mag;

	/* Make rotation matrix */

	/* Z vector */
	z[0] = eyex - centerx;
	z[1] = eyey - centery;
	z[2] = eyez - centerz;
	mag = sqrtf(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
	if (mag) {          /* mpichler, 19950515 */
		z[0] /= mag;
		z[1] /= mag;
		z[2] /= mag;
	}

	/* Y vector */
	y[0] = upx;
	y[1] = upy;
	y[2] = upz;

	/* X vector = Y cross Z */
	x[0] = y[1] * z[2] - y[2] * z[1];
	x[1] = -y[0] * z[2] + y[2] * z[0];
	x[2] = y[0] * z[1] - y[1] * z[0];

	/* Recompute Y = Z cross X */
	y[0] = z[1] * x[2] - z[2] * x[1];
	y[1] = -z[0] * x[2] + z[2] * x[0];
	y[2] = z[0] * x[1] - z[1] * x[0];

	/* mpichler, 19950515 */
	/* cross product gives area of parallelogram, which is < 1.0 for
	* non-perpendicular unit-length vectors; so normalize x, y here
	*/
#if 1
	mag = sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
	if (mag) {
		x[0] /= mag;
		x[1] /= mag;
		x[2] /= mag;
	}

	mag = sqrtf(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
	if (mag) {
		y[0] /= mag;
		y[1] /= mag;
		y[2] /= mag;
	}
#endif

#define M(row,col)  m[col*4+row]
	M(0, 0) = x[0];
	M(0, 1) = x[1];
	M(0, 2) = x[2];
	M(0, 3) = 0.0;
	M(1, 0) = y[0];
	M(1, 1) = y[1];
	M(1, 2) = y[2];
	M(1, 3) = 0.0;
	M(2, 0) = z[0];
	M(2, 1) = z[1];
	M(2, 2) = z[2];
	M(2, 3) = 0.0;
	M(3, 0) = 0.0;
	M(3, 1) = 0.0;
	M(3, 2) = 0.0;
	M(3, 3) = 1.0;
#undef M
	//glMultMatrixf(m);
	mf mat;
	mfset(&mat, m);

	/* Translate Eye to Origin */
	//glTranslatef(-eyex, -eyey, -eyez);
	mf mat2;
	float trans[] = { -eyex, -eyey, -eyez };
	mfreset(&mat2);
	//memset(mat2.matrix, 0, sizeof(float) * 16);
	mftransl(&mat2, trans);

	//mat.postmult(mat2);
	mfpostmult(&mat, &mat2);

	return mat;
}

mf lookat2(v3f eye, v3f center, v3f up)
{
	float *m, trans[3];
	v3f x, y, z;
	mf mat;
	mf mat2;

	m = mat.matrix;

	/*Make rotation matrix */

	/*Z vector */
	v3fsub(&z, eye, center);
	z = norm3f(z);

	/*Y vector */
	y = up;

	//v3f vcross = cross3f(view - pos, up);
	/*X vector = Y cross Z */
	x = norm3f(cross3f(y, z));

	// return tnorm( cross3f( strafe, view - pos ) );
	/*Recompute Y = Z cross X */
	y = norm3f(cross3f(z, x));

	/*mpichler, 19950515 */
	/*cross product gives area of parallelogram, which is < 1.0 for
	 * non-perpendicular unit-length vectors; so normalize x, y here
	 */

#define M(row,col)  m[col*4+row]
	 //#define M(row,col)  m[row*4+col]
	M(0, 0) = x.x;
	M(0, 1) = x.y;
	M(0, 2) = x.z;
	M(0, 3) = 0.0;
	//M(0, 3) = eyex;
	M(1, 0) = y.x;
	M(1, 1) = y.y;
	M(1, 2) = y.z;
	M(1, 3) = 0.0;
	//M(1, 3) = eyey;
	M(2, 0) = z.x;
	M(2, 1) = z.y;
	M(2, 2) = z.z;
	M(2, 3) = 0.0;
	M(3, 0) = 0.0;
	M(3, 1) = 0.0;
	M(3, 2) = 0.0;
	//M(3, 2) = eyez;
	M(3, 3) = 1.0;
	//M(3, 0) = -eyex;
	//M(3, 1) = -eyey;
	//M(3, 2) = -eyez;
	//M(3, 3) = 1.0;
#undef M

	//mfset(&mat, m);

	/*Translate Eye to Origin */
	trans[0] = -eye.x;
	trans[1] = -eye.y;
	trans[2] = -eye.z;
	mftransl(&mat2, trans);
	mfpostmult(&mat, &mat2);

	return mat;

}

// http://www.songho.ca/opengl/gl_projectionmatrix.html
// http://www.scratchapixel.com/lessons/3d-advanced-lessons/perspective-and-orthographic-projection-matrix/orthographic-projection/
// http://www.opengl.org/discussion_boards/showthread.php/172280-Constructing-an-orthographic-matrix-for-2D-drawing
mf oproj(float l, float r, float t, float b, float n, float f)
{
	float m[16];
	mf mat;

#define M(row,col)  m[col*4+row]
	M(0, 0) = 2 / (r - l);
	M(0, 1) = 0;
	M(0, 2) = 0;
	M(0, 3) = 0;

	M(1, 0) = 0;
	M(1, 1) = 2 / (t - b);
	M(1, 2) = 0;
	M(1, 3) = 0;

	M(2, 0) = 0;
	M(2, 1) = 0;
	M(2, 2) = -1 / (f - n);
	//M(2, 2) = -2 / (f - n);
	M(2, 3) = 0;

	M(3, 0) = -(r + l) / (r - l);
	M(3, 1) = -(t + b) / (t - b);
	M(3, 2) = -n / (f - n);
	//M(3, 2) = -(f + n) / (f - n);
	M(3, 3) = 1;
#undef M

	mfset(&mat, m);

	return mat;
}

mf pproj2(float fov, float aspect, float znear, float zfar)
{
	float b, t, l, r, n, f;
	float xymax, ymin, xmin, width, height, depth;
	mf mat;

	xymax = znear * tan(fov * PI_OVER_360);
	ymin = -xymax;
	xmin = -xymax;

	width = xymax - xmin;
	height = xymax - ymin;

	r = width / 2.0f;
	l = -r;
	t = height / 2.0f;
	b = -b;

#if 0

	// set OpenGL perspective projection matrix
	M[0][0] = 2 * n / (r - l);
	M[0][1] = 0;
	M[0][2] = 0;
	M[0][3] = 0;

	M[1][0] = 0;
	M[1][1] = 2 * n / (t - b);
	M[1][2] = 0;
	M[1][3] = 0;

	M[2][0] = (r + l) / (r - l);
	M[2][1] = (t + b) / (t - b);
	M[2][2] = -(f + n) / (f - n);
	M[2][3] = -1;

	M[3][0] = 0;
	M[3][1] = 0;
	M[3][2] = -2 * f * n / (f - n);
	M[3][3] = 0;
#endif
}

mf pproj(float fov, float aspect, float znear, float zfar)
{
	float m[16], xymax, ymin, xmin, width, height, depth, q, qn, w, h;
	mf mat;

	xymax = znear * tan(fov * PI_OVER_360);
	ymin = -xymax;
	xmin = -xymax;

	width = xymax - xmin;
	height = xymax - ymin;

	depth = zfar - znear;
	q = -(zfar + znear) / depth;
	qn = -2 * (zfar * znear) / depth;

	w = 2 * znear / width;
	w = w / aspect;
	h = 2 * znear / height;

	m[0] = w;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = h;
	m[6] = 0;
	m[7] = 0;

	m[8] = 0;
	m[9] = 0;
	m[10] = q;
	m[11] = -1;

	m[12] = 0;
	m[13] = 0;
	m[14] = qn;
	m[15] = 0;

	mfset(&mat, m);

	return mat;
}

v4f screen3f(mf *mvp, v3f vec, float width, float height, dbool persp)
{
	v4f sp;
	sp.x = vec.x;
	sp.y = vec.y;
	sp.z = vec.z;
	sp.w = 1;

	v4ftransform(&sp, mvp);

	if (persp)
	{
		//does this need to be commented out for correct orthographic pixel-to-pixel match?
		v4fdiv(&sp, sp, sp.w);
	}

	sp.x = (sp.x * 0.5f + 0.5f) * width;
	sp.y = (-sp.y * 0.5f + 0.5f) * height;

	return sp;
}

v3f tonearo3f(int x, int y, int width, int height, v3f posvec, v3f sidevec, v3f upvec)
{
	float halfw, halfh, ratiox, ratioy, aspect, wnear, hnear;
	v3f result;

	halfw = (float)width / 2.0f;
	halfh = (float)height / 2.0f;

	ratiox = (x - halfw) / halfw;
	ratioy = -(y - halfh) / halfh;

	aspect = fabsf((float)width / (float)height);
	wnear = PROJ_RIGHT * aspect / g_zoom;
	hnear = PROJ_RIGHT / g_zoom;

	v3fmul(&sidevec, sidevec, ratiox * wnear);
	v3fmul(&upvec, upvec, ratioy * hnear);
	v3fadd(&result, posvec, sidevec);
	v3fadd(&result, result, upvec);

	return result;
}

v3f tonearp3f(int x, int y, int width, int height, v3f posvec, v3f sidevec, v3f upvec, v3f viewdir, float fov, float mind)
{
	float halfw, halfh, ratiox, ratioy, aspect, wnear, hnear;
	v3f result;

	viewdir = norm3f(viewdir);

	halfw = (float)width / 2.0f;
	halfh = (float)height / 2.0f;

	ratiox = (x - halfw) / halfw;
	ratioy = -(y - halfh) / halfh;

	aspect = fabsf((float)width / (float)height);
	hnear = 2 * tan(DEGTORAD(fov) / 2) * mind;
	wnear = hnear * aspect;

	v3fmul(&viewdir, viewdir, mind);
	v3fmul(&sidevec, sidevec, ratiox * wnear);
	v3fmul(&upvec, upvec, ratioy * hnear);
	v3fadd(&result, posvec, sidevec);
	v3fadd(&result, result, upvec);
	v3fadd(&result, result, viewdir);

	return result;
}

v3f screenpray3f(int x, int y, int width, int height, v3f posvec, v3f sidevec, v3f upvec, v3f viewdir, float fov)
{
	float halfw, halfh, ratiox, ratioy, aspect, wnear, hnear;
	v3f result;

	viewdir = norm3f(viewdir);

	halfw = (float)width / 2.0f;
	halfh = (float)height / 2.0f;

	ratiox = (x - halfw) / halfw;
	ratioy = -(y - halfh) / halfh;

	aspect = fabsf((float)width / (float)height);
	hnear = 2 * tan(DEGTORAD(fov) / 2);
	wnear = hnear * aspect;

	v3fmul(&sidevec, sidevec, ratiox * wnear / 2.0f);
	v3fmul(&upvec, upvec, ratioy * hnear / 2.0f);
	v3fadd(&result, posvec, sidevec);
	v3fadd(&result, result, upvec);
	v3fadd(&result, result, viewdir);

	return result;
}

float snapf(float base, float value)
{
	int count;

	count = value / base;

	return count * base;
}

float snapfnear(float base, float value)
{
	int count;

	count = (value + base / 2.0f) / base;

	return count * base;
}
