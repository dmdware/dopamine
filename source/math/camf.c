
#include "camf.h"

c3f g_camf;

/* TODO leave as float for now then use fixmath int's */
void cpos3f(v3f *v, c3f *c)
{
}

float yaw3f(c3f* c)
{
	 return atan2(c->view.x, c->view.z);
}

void c3finit(c3f* c)
{
	c->view.x = 0;
	c->view.y = 0;
	c->view.z = 1;
	c->pos.x = 0;
	c->pos.y = 0;
	c->pos.z = 0;
	c->strafe.x = 1;
	c->strafe.y = 0;
	c->strafe.z = 0;
	c->up.x = 0;
	c->up.y = 1;
	c->up.z = 0;
}