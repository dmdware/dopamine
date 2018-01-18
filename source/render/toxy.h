

#ifndef TOXY_H
#define TOXY_H

#include "../math/v3f.h"

extern v3f gpv[8];
extern v3f gpl[6];
extern float gpld[6];
extern float gsa[12];

v3f toxy2(v3f vi, float wx, float wy, v3f p[8], v3f pl[6], float pld[6], float *d, float sa[12]);
v3f toxy(v3f vi, float wx, float wy, v3f view, v3f pos, v3f up, v3f strafe, float maxd, float mind, float fov, float *d);
v3f toclip(v3f vi);
void pfrust(float wx, float wy, v3f view, v3f pos, v3f up, v3f strafe, float maxd, float mind, float fov, float *d, v3f *pv, v3f *pl, float *pld, float *sa);
void ofrust(float wx, float wy, v3f view, v3f pos, v3f up, v3f strafe, float maxd, float mind, float *d, v3f *pv, v3f *pl, float *pld, float *sa);

#endif