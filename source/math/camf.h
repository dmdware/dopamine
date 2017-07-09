
#ifndef CAMF_H
#define CAMF_H

#include "v3i.h"
#include "../algo/bool.h"
#include "quatf.h"

struct c3f 
{
	v3i pos;					
	v3i view;			
	v3i up;		
	v3i strafe;		
	v3i vel;
	v3i orientv;	// pitch, yaw, roll
	qf orientq;	//orientation quaternion
	dbool grounded;
};

typedef struct c3f c3f;

void cpos3f(v3i *v, c3f *c);
float yaw3f(c3f* c);

extern c3f g_camf;

#endif