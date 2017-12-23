
#ifndef CAMF_H
#define CAMF_H

#include "v3f.h"
#include "../algo/bool.h"
#include "quatf.h"

struct c3f 
{
	v3f pos;					
	v3f view;			
	v3f up;		
	v3f strafe;		
	v3f vel;
	v3f orientv;	// pitch, yaw, roll
	qf orientq;	//orientation quaternion
	dbool grounded;
	dbool f, b, l, r;
	v3f v;
};

typedef struct c3f c3f;

void cpos3f(v3f *v, c3f *c);
float yaw3f(c3f* c);

extern c3f g_camf;

#endif