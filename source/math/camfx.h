
#ifndef CAMFX_H
#define CAMFX_H

#include "v3i.h"
#include "../algo/bool.h"
#include "quatf.h"

struct c3x 
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

typedef struct c3x c3x;

void cpos3x(v3i *v, c3x *c);
float yaw3x(c3x* c);

extern c3x g_camx;

#endif