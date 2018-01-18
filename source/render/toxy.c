



#include "toxy.h"
#include "../math/3dmath.h"
#include "../math/plane3f.h"
#include "../sys/syswin.h"
#include "../sys/utils.h"
#include "../sys/debug.h"

v3f gpv[8];
v3f gpl[6];
float gpld[6];
float gsa[12];

float vol3f2(v3f a, v3f b, v3f c)
{
	float f;
	b = cross3f(b, c);
	f = dot3f(a, b);
	//f = fabs(f);
	f *= 1.0 / (2.0 * 3.0);
	return f;
}

float vol3f(v3f a, v3f b, v3f c, v3f d)
{
	v3fsub(&a, a, d);
	v3fsub(&b, b, d);
	v3fsub(&c, c, d);
	return vol3f2(a, b, c);
}

float sa3f(v3f a, v3f b, v3f c)
{
	float s, a1, a2, a3;
	v3f ab, ac, bc;

	v3fsub(&ab, a, b);
	v3fsub(&ac, a, c);
	v3fsub(&bc, b, c);

	a1 = mag3f(ab);
	a2 = mag3f(ac);
	a3 = mag3f(bc);

	s = (a1 + a2 + a3) / 2.0f;

	return sqrtf(s * (s - a1) * (s - a2) * (s - a3));
}

v3f toxy2(v3f vi, float wx, float wy, v3f p[8], v3f pl[6], float pld[6], float *d, float sa[12])
{
	float v[12];
	int i;
	v3f vo;
	v[0] = vol3f(vi, p[0], p[2], p[1]) / sa[0];
	v[1] = vol3f(vi, p[0], p[3], p[2]) / sa[1];
	v[2] = vol3f(vi, p[6], p[1], p[2]) / sa[2];
	v[3] = vol3f(vi, p[5], p[1], p[6]) / sa[3];
	v[4] = vol3f(vi, p[6], p[2], p[7]) / sa[4];
	v[5] = vol3f(vi, p[2], p[3], p[7]) / sa[5];
	v[6] = vol3f(vi, p[7], p[3], p[0]) / sa[6];
	v[7] = vol3f(vi, p[4], p[7], p[0]) / sa[7];
	v[8] = vol3f(vi, p[4], p[1], p[5]) / sa[8];
	v[9] = vol3f(vi, p[4], p[0], p[1]) / sa[9];
	v[10] = vol3f(vi, p[7], p[5], p[6]) / sa[10];
	v[11] = vol3f(vi, p[5], p[7], p[4]) / sa[11];

	//memcpy(d, v, sizeof(float) * 12);

#if 0
	if (plad(pl[0], pld[0], vi) < 0)
	{
		v[0] *= -1;
		v[1] *= -1;
		vi.x = vi.y = vi.z = NAN;
		return vi;
	}
	if (plad(pl[1], pld[1], vi) < 0)
	{
		v[10] *= -1;
		v[11] *= -1;
		vi.x = vi.y = vi.z = NAN;
		return vi;
	}
	if (plad(pl[2], pld[2], vi) < 0)
	{
		v[8] *= -1;
		v[9] *= -1;
		vi.x = vi.y = vi.z = NAN;
		return vi;
	}
	if (plad(pl[3], pld[3], vi) < 0)
	{
		v[2] *= -1;
		v[3] *= -1;
		vi.x = vi.y = vi.z = NAN;
		return vi;
	}
	if (plad(pl[4], pld[4], vi) < 0)
	{
		v[4] *= -1;
		v[5] *= -1;
		vi.x = vi.y = vi.z = NAN;
		return vi;
	}
	if (plad(pl[5], pld[5], vi) < 0)
	{
		v[6] *= -1;
		v[7] *= -1;
		vi.x = vi.y = vi.z = NAN;
		return vi;
	}
#endif

	//for (i = 0; i < 12; ++i)
		//v[i] = fabs(v[i]);

#if 01

	//if ((v[6] < 0 || v[7] < 0) && (v[2] < 0 || v[3] < 0))
	//{
	//	vo.x = ((fabs(v[2]) + fabs(v[3])) / (fabs(v[2]) + fabs(v[3]) + fabs(v[6]) + fabs(v[7])));
	//}
	//else 

	//if ((v[6] < 0 || v[7] < 0) && (v[8] < 0 || v[9] < 0))
	{

		//vo.x = -(fabs(v[6]) + fabs(v[7])) / (-fabs(v[6]) - fabs(v[7]) + v[2] + v[3]);
		//vo.y = -(fabs(v[8]) + fabs(v[9])) / (-fabs(v[8]) - fabs(v[9]) + v[4] + v[5]);

		//fprintf(g_applog, "-- %f,%f,%f->%f,%f,%f\r\n", vi.x, vi.y, vi.z, vo.x, vo.y, vo.z);
	}
	//else
	{
		if (v[6] < 0 || v[7] < 0)
		{
			vo.x = -(fabs(v[6]) + fabs(v[7])) / fabs(-fabs(v[6]) - fabs(v[7]) + v[2] + v[3]);
		}
		else if (v[2] < 0 || v[3] < 0)
		{
			vo.x = (1.0f + (fabs(v[2]) + fabs(v[3])) / fabs(-fabs(v[2]) - fabs(v[3]) + v[6] + v[7]));
		}
		else
		{
			vo.x = (v[6] + v[7]) / (v[6] + v[7] + v[2] + v[3]);
		}

		//if ((v[8] < 0 || v[9] < 0) && (v[4] < 0 || v[5] < 0))
		//{
		//	vo.y = ((fabs(v[4]) + fabs(v[5])) / (fabs(v[4]) + fabs(v[5]) + fabs(v[8]) + fabs(v[9])));
	//	}
		//else 
		if (v[8] < 0 || v[9] < 0)
		{
			vo.y = -(fabs(v[8]) + fabs(v[9])) / fabs(-fabs(v[8]) - fabs(v[9]) + v[4] + v[5]);
		}
		else if (v[4] < 0 || v[5] < 0)
		{
			vo.y = (1.0f + (fabs(v[4]) + fabs(v[5])) / fabs(-fabs(v[4]) - fabs(v[5]) + v[8] + v[9]));
		}
		else
		{
			vo.y = (v[8] + v[9]) / (v[4] + v[5] + v[8] + v[9]);
		}
	}

	if (v[10] < 0 || v[11] < 0)
	{
		vo.z = -((fabs(v[10]) + fabs(v[11])) / fabs((v[0]) + (v[1]) - fabs(v[10]) - fabs(v[11])));
		//vo.z = 1.0f / (vo.z - vo.z);
		//vo.x = 0.5f-(vo.x - 0.5f);
		//vo.y = 0.5f-(vo.y - 0.5f);
	}
	else if (v[0] < 0 || v[1] < 0)
	{
		vo.z = (1.0f + (fabs(v[0]) + fabs(v[1])) / fabs(v[10] + v[11] - fabs(v[0]) - fabs(v[1])));
	}
	else
	{
		vo.z = ((v[10] + v[11]) / (v[0] + v[1] + v[10] + v[11]));
	}

#endif
	//memcpy(&d[12], &vo, sizeof(float) * 3);

	vo.x *= wx;
	vo.y *= wy;
	//vo.z = (vo.z - 1.0f) * 2.0f + 1.0f;
	//vo.z = vo.z + 1.0f;
#if 0
	if (vo.z < 0)
	{
		fprintf(g_applog, "%f,%f,%f -> %f,%f,%f    %f,%f,%f   %f,%f,%f   %f,%f,%f  %f,%f,%f\r\n", vi.x, vi.y, vi.z, vo.x, vo.y, vo.z,
			p[4].x, p[4].y, p[4].z,
			p[5].x, p[5].y, p[5].z,
			p[6].x, p[6].y, p[6].z,
			p[7].x, p[7].y, p[7].z);
	}
#endif
	//vi.z = vi.z - 0.1f;
	
	return vo;
}

void ofrust(float wx, float wy, v3f view, v3f pos, v3f up, v3f strafe, float maxd, float mind, float *d, v3f *pv, v3f *pl, float *pld, float *sa)
{
	v3f vvvv[3][2][2];
	v3f tn[6][3];

#define X	0
#define Y	1
#define Z	2
#define NEARP	0
#define FARP		1
#define POS		0
#define NEG		1

	vvvv[X][NEARP][POS] = strafe;
	v3fsub(&vvvv[Z][NEARP][POS], view, pos);
	vvvv[Z][NEARP][POS] = norm3f(vvvv[Z][NEARP][POS]);
	vvvv[Y][NEARP][POS] = norm3f(cross3f(vvvv[X][NEARP][POS], vvvv[Z][NEARP][POS]));
	v3fmul(&vvvv[Y][NEARP][POS], vvvv[Y][NEARP][POS], wy/2.0f);
	v3fmul(&vvvv[X][NEARP][POS], vvvv[X][NEARP][POS], wx/2.0f);

	vvvv[X][FARP][POS] = vvvv[X][NEARP][POS];
	vvvv[Y][FARP][POS] = vvvv[Y][NEARP][POS];
	v3fmul(&vvvv[Z][FARP][POS], vvvv[Z][NEARP][POS], maxd);
	v3fmul(&vvvv[Z][NEARP][POS], vvvv[Z][NEARP][POS], mind);

	v3fmul(&vvvv[X][NEARP][NEG], vvvv[X][NEARP][POS], -1);
	v3fmul(&vvvv[Y][NEARP][NEG], vvvv[Y][NEARP][POS], -1);
	v3fmul(&vvvv[Z][NEARP][NEG], vvvv[Z][NEARP][POS], -1);
	v3fmul(&vvvv[X][FARP][NEG], vvvv[X][FARP][POS], -1);
	v3fmul(&vvvv[Y][FARP][NEG], vvvv[Y][FARP][POS], -1);
	v3fmul(&vvvv[Z][FARP][NEG], vvvv[Z][FARP][POS], -1);

	v3fadd(&pv[0], vvvv[X][FARP][NEG], vvvv[Y][FARP][POS]);
	v3fadd(&pv[0], pv[0], vvvv[Z][FARP][POS]);
	v3fadd(&pv[1], vvvv[X][FARP][POS], vvvv[Y][FARP][POS]);
	v3fadd(&pv[1], pv[1], vvvv[Z][FARP][POS]);
	v3fadd(&pv[2], vvvv[X][FARP][POS], vvvv[Y][FARP][NEG]);
	v3fadd(&pv[2], pv[2], vvvv[Z][FARP][POS]);
	v3fadd(&pv[3], vvvv[X][FARP][NEG], vvvv[Y][FARP][NEG]);
	v3fadd(&pv[3], pv[3], vvvv[Z][FARP][POS]);
	v3fadd(&pv[4], vvvv[X][NEARP][NEG], vvvv[Y][NEARP][POS]);
	v3fadd(&pv[4], pv[4], vvvv[Z][NEARP][POS]);
	v3fadd(&pv[5], vvvv[X][NEARP][POS], vvvv[Y][NEARP][POS]);
	v3fadd(&pv[5], pv[5], vvvv[Z][NEARP][POS]);
	v3fadd(&pv[6], vvvv[X][NEARP][POS], vvvv[Y][NEARP][NEG]);
	v3fadd(&pv[6], pv[6], vvvv[Z][NEARP][POS]);
	v3fadd(&pv[7], vvvv[X][NEARP][NEG], vvvv[Y][NEARP][NEG]);
	v3fadd(&pv[7], pv[7], vvvv[Z][NEARP][POS]);

#undef X
#undef Y
#undef Z
#undef NEARP
#undef FARP
#undef POS
#undef NEG

	v3fadd(&pv[0], pv[0], pos);
	v3fadd(&pv[1], pv[1], pos);
	v3fadd(&pv[2], pv[2], pos);
	v3fadd(&pv[3], pv[3], pos);
	v3fadd(&pv[4], pv[4], pos);
	v3fadd(&pv[5], pv[5], pos);
	v3fadd(&pv[6], pv[6], pos);
	v3fadd(&pv[7], pv[7], pos);

	tn[0][0] = pv[0];
	tn[0][1] = pv[1];
	tn[0][2] = pv[2];
	tn[1][0] = pv[5];
	tn[1][1] = pv[4];
	tn[1][2] = pv[6];
	tn[2][0] = pv[1];
	tn[2][1] = pv[0];
	tn[2][2] = pv[4];
	tn[3][0] = pv[2];
	tn[3][1] = pv[1];
	tn[3][2] = pv[5];
	tn[4][0] = pv[3];
	tn[4][1] = pv[2];
	tn[4][2] = pv[7];
	tn[5][0] = pv[4];
	tn[5][1] = pv[0];
	tn[5][2] = pv[3];

	pl[0] = tnorm(tn[0]);
	pl[1] = tnorm(tn[1]);
	pl[2] = tnorm(tn[2]);
	pl[3] = tnorm(tn[3]);
	pl[4] = tnorm(tn[4]);
	pl[5] = tnorm(tn[5]);

	pld[0] = -dot3f(pl[0], tn[0][0]);
	pld[1] = -dot3f(pl[1], tn[1][0]);
	pld[2] = -dot3f(pl[2], tn[2][0]);
	pld[3] = -dot3f(pl[3], tn[3][0]);
	pld[4] = -dot3f(pl[4], tn[4][0]);
	pld[5] = -dot3f(pl[5], tn[5][0]);

	sa[0] = sa3f(pv[0], pv[2], pv[1]);
	sa[1] = sa3f(pv[0], pv[3], pv[2]);
	sa[2] = sa3f(pv[1], pv[2], pv[6]);
	sa[3] = sa3f(pv[1], pv[6], pv[5]);
	sa[4] = sa3f(pv[2], pv[6], pv[7]);
	sa[5] = sa3f(pv[7], pv[3], pv[2]);
	sa[6] = sa3f(pv[7], pv[3], pv[0]);
	sa[7] = sa3f(pv[7], pv[0], pv[4]);
	sa[8] = sa3f(pv[1], pv[5], pv[4]);
	sa[9] = sa3f(pv[0], pv[1], pv[4]);
	sa[10] = sa3f(pv[6], pv[5], pv[7]);
	sa[11] = sa3f(pv[5], pv[4], pv[7]);
}

void pfrust(float wx, float wy, v3f view, v3f pos, v3f up, v3f strafe, float maxd, float mind, float fov, float *d, v3f *pv, v3f *pl, float *pld, float *sa)
{
	v3f vvvv[3][2][2];
	float ta;
	v3f tn[6][3];

#define X	0
#define Y	1
#define Z	2
#define NEARP	0
#define FARP		1
#define POS		0
#define NEG		1

	vvvv[X][NEARP][POS] = strafe;
	v3fsub(&vvvv[Z][NEARP][POS], view, pos);
	vvvv[Z][NEARP][POS] = norm3f(vvvv[Z][NEARP][POS]);
	vvvv[Y][NEARP][POS] = norm3f(cross3f(vvvv[X][NEARP][POS], vvvv[Z][NEARP][POS]));
	ta = tan(fov / 180.0f * 3.14159f / 2.0f);
	v3fmul(&vvvv[Y][NEARP][POS], vvvv[Y][NEARP][POS], wy / wx * ta);
	v3fmul(&vvvv[X][NEARP][POS], vvvv[X][NEARP][POS], ta);

	v3fmul(&vvvv[X][FARP][POS], vvvv[X][NEARP][POS], maxd);
	v3fmul(&vvvv[Y][FARP][POS], vvvv[Y][NEARP][POS], maxd);
	v3fmul(&vvvv[Z][FARP][POS], vvvv[Z][NEARP][POS], maxd);
	v3fmul(&vvvv[X][NEARP][POS], vvvv[X][NEARP][POS], mind);
	v3fmul(&vvvv[Y][NEARP][POS], vvvv[Y][NEARP][POS], mind);
	v3fmul(&vvvv[Z][NEARP][POS], vvvv[Z][NEARP][POS], mind);

	v3fmul(&vvvv[X][NEARP][NEG], vvvv[X][NEARP][POS], -1);
	v3fmul(&vvvv[Y][NEARP][NEG], vvvv[Y][NEARP][POS], -1);
	v3fmul(&vvvv[Z][NEARP][NEG], vvvv[Z][NEARP][POS], -1);
	v3fmul(&vvvv[X][FARP][NEG], vvvv[X][FARP][POS], -1);
	v3fmul(&vvvv[Y][FARP][NEG], vvvv[Y][FARP][POS], -1);
	v3fmul(&vvvv[Z][FARP][NEG], vvvv[Z][FARP][POS], -1);

	v3fadd(&pv[0], vvvv[X][FARP][NEG], vvvv[Y][FARP][POS]);
	v3fadd(&pv[0], pv[0], vvvv[Z][FARP][POS]);
	v3fadd(&pv[1], vvvv[X][FARP][POS], vvvv[Y][FARP][POS]);
	v3fadd(&pv[1], pv[1], vvvv[Z][FARP][POS]);
	v3fadd(&pv[2], vvvv[X][FARP][POS], vvvv[Y][FARP][NEG]);
	v3fadd(&pv[2], pv[2], vvvv[Z][FARP][POS]);
	v3fadd(&pv[3], vvvv[X][FARP][NEG], vvvv[Y][FARP][NEG]);
	v3fadd(&pv[3], pv[3], vvvv[Z][FARP][POS]);
	v3fadd(&pv[4], vvvv[X][NEARP][NEG], vvvv[Y][NEARP][POS]);
	v3fadd(&pv[4], pv[4], vvvv[Z][NEARP][POS]);
	v3fadd(&pv[5], vvvv[X][NEARP][POS], vvvv[Y][NEARP][POS]);
	v3fadd(&pv[5], pv[5], vvvv[Z][NEARP][POS]);
	v3fadd(&pv[6], vvvv[X][NEARP][POS], vvvv[Y][NEARP][NEG]);
	v3fadd(&pv[6], pv[6], vvvv[Z][NEARP][POS]);
	v3fadd(&pv[7], vvvv[X][NEARP][NEG], vvvv[Y][NEARP][NEG]);
	v3fadd(&pv[7], pv[7], vvvv[Z][NEARP][POS]);

#undef X
#undef Y
#undef Z
#undef NEARP
#undef FARP
#undef POS
#undef NEG

	v3fadd(&pv[0], pv[0], pos);
	v3fadd(&pv[1], pv[1], pos);
	v3fadd(&pv[2], pv[2], pos);
	v3fadd(&pv[3], pv[3], pos);
	v3fadd(&pv[4], pv[4], pos);
	v3fadd(&pv[5], pv[5], pos);
	v3fadd(&pv[6], pv[6], pos);
	v3fadd(&pv[7], pv[7], pos);

	tn[0][0] = pv[0];
	tn[0][1] = pv[1];
	tn[0][2] = pv[2];
	tn[1][0] = pv[5];
	tn[1][1] = pv[4];
	tn[1][2] = pv[6];
	tn[2][0] = pv[1];
	tn[2][1] = pv[0];
	tn[2][2] = pv[4];
	tn[3][0] = pv[2];
	tn[3][1] = pv[1];
	tn[3][2] = pv[5];
	tn[4][0] = pv[3];
	tn[4][1] = pv[2];
	tn[4][2] = pv[7];
	tn[5][0] = pv[4];
	tn[5][1] = pv[0];
	tn[5][2] = pv[3];

	pl[0] = tnorm(tn[0]);
	pl[1] = tnorm(tn[1]);
	pl[2] = tnorm(tn[2]);
	pl[3] = tnorm(tn[3]);
	pl[4] = tnorm(tn[4]);
	pl[5] = tnorm(tn[5]);

	pld[0] = -dot3f(pl[0], tn[0][0]);
	pld[1] = -dot3f(pl[1], tn[1][0]);
	pld[2] = -dot3f(pl[2], tn[2][0]);
	pld[3] = -dot3f(pl[3], tn[3][0]);
	pld[4] = -dot3f(pl[4], tn[4][0]);
	pld[5] = -dot3f(pl[5], tn[5][0]);

	sa[0] = sa3f(pv[0], pv[2], pv[1]);
	sa[1] = sa3f(pv[0], pv[3], pv[2]);
	sa[2] = sa3f(pv[1], pv[2], pv[6]);
	sa[3] = sa3f(pv[1], pv[6], pv[5]);
	sa[4] = sa3f(pv[2], pv[6], pv[7]);
	sa[5] = sa3f(pv[7], pv[3], pv[2]);
	sa[6] = sa3f(pv[7], pv[3], pv[0]);
	sa[7] = sa3f(pv[7], pv[0], pv[4]);
	sa[8] = sa3f(pv[1], pv[5], pv[4]);
	sa[9] = sa3f(pv[0], pv[1], pv[4]);
	sa[10] = sa3f(pv[6], pv[5], pv[7]);
	sa[11] = sa3f(pv[5], pv[4], pv[7]);
}

v3f toxy(v3f vi, float wx, float wy, v3f view, v3f pos, v3f up, v3f strafe, float maxd, float mind, float fov, float *d)
{
	v3f pv[8];
	v3f pl[6];
	float pld[6];
	float sa[12];

	pfrust(wx, wy, view, pos, up, strafe, maxd, mind, fov, d, pv, pl, pld, sa);

	return toxy2(vi, wx, wy, pv, pl, pld, d, sa);
}

v3f toclip(v3f vi)
{
	float d[12];
	return toxy2(vi, 1, 1, gpv, gpl, gpld, d, gsa);
}