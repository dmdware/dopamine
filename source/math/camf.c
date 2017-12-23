
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