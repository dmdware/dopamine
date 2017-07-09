#ifndef MATH3D_H
#define MATH3D_H

#include "../sys/inc.h"
#include "v3f.h"
#include "v4f.h"
#include "v2f.h"
#include "v2i.h"
#include "v3i.h"
#include "camf.h"
#include "plane3f.h"

#define DEGTORAD(a)		(M_PI * a / 180)
#define RADTODEG(a)		(180 * a / M_PI)

#ifndef PI_ON_180
#	define PI_ON_180	(M_PI/180.0)
#endif

#define PI_OVER_360		(M_PI/360.0f)

#define CAMERA_SPEED	650

#define EPSILON			0.1f

struct p3f;
struct v3f;
struct c3f;
struct mf;
struct Triangle;
struct v4f;
struct v2i;
struct v3i;
struct v2f;

v3f vmin(float minf, v3f v);
float mag3f(v3f v);
float mag2f(v2f v);
float dot3f2(v3f v);
float dot3f3(v3f v);
int mag2i(v2i v);
int dot2i2(v2i v);
int man2i(v2i v);
v3f norm3f(v3f n);
v2f norm2f(v2f n);
v3f cross3f(v3f v1, v3f v2);
float dot3f(v3f v1, v3f v2);
v3f tov3f(v3f p1, v3f p2);
int dot3i(v3i v1, v3i v2);
int dot2i(v2i v1, v2i v2);
v3f tov3f(v3f p1, v3f p2);
v3f tnorm(v3f t[]);
v3f tnorm2(v3f t[]);
dbool intpg(v3f poly[], v3f line[], v3f *n, float *origindist);
dbool inyaw3f(c3f *c, v3f p, float angle);
float DYaw(c3f *c, v3f p);
double angle3f(v3f v1, v3f v2);
v3f intpl(v3f n, v3f line[], double d);
dbool intpl2(v3f l0, v3f l, p3f p, v3f *inter);
v3f tonearo3f(int x, int y, int width, int height, v3f posvec, v3f sidevec, v3f upvec);
v3f tonearp3f(int x, int y, int width, int height, v3f posvec, v3f sidevec, v3f upvec, v3f viewdir, float fov, float mind);
v3f screenpray3f(int x, int y, int width, int height, v3f posvec, v3f sidevec, v3f upvec, v3f viewdir, float fov);
v3f rot3f(v3f v, float rad, float x, float y, float z);
v3f rotab3f(v3f v, v3f around, float rad, float x, float y, float z);
float clipf(float n, float lower, float upper);
int clipi(int n, int lower, int upper);
float yawf(float dx, float dz);
mf lookat(float eyex, float eyey, float eyez,
				  float centerx, float centery, float centerz,
				  float upx, float upy, float upz);
mf pproj(float fov, float aspect, float znear, float zfar);
mf oproj(float l, float r, float t, float b, float n, float f);
v4f screen3f(mf *mvp, v3f vec, float width, float height, dbool persp);
float snapf(float base, float value);
float snapfnear(float base, float value);

#endif
