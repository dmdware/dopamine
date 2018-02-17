
#include "camf.h"
#include "../sys/syswin.h"
#include "../sys/utils.h"
#include "3dmath.h"

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
	c->view.z = 0;
	c->pos.x = 0;
	c->pos.y = 0;
	c->pos.z = -1;
	c->strafe.x = 1;
	c->strafe.y = 0;
	c->strafe.z = 0;
	c->up.x = 0;
	c->up.y = 1;
	c->up.z = 0;
}

dbool cup3f(c3f *c)
{
	v3f v;

	if (c->f)
	{
		v3fsub(&v, c->view, c->pos);
		v3fmul(&v, v, 1.0f * g_upiv / mag3f(v));
		v3fadd(&c->view, c->view, v);
		v3fadd(&c->pos, c->pos, v);
	}
	if (c->b)
	{
		v3fsub(&v, c->view, c->pos);
		v3fmul(&v, v, -1.0f * g_upiv / mag3f(v));
		v3fadd(&c->view, c->view, v);
		v3fadd(&c->pos, c->pos, v);
	}
	if (c->l)
	{
		v3fmul(&v, c->strafe, -1.0f * g_upiv / mag3f(c->strafe));
		v3fadd(&c->view, c->view, v);
		v3fadd(&c->pos, c->pos, v);
	}
	if (c->r)
	{
		v3fmul(&v, c->strafe, 1.0f * g_upiv / mag3f(c->strafe));
		v3fadd(&c->view, c->view, v);
		v3fadd(&c->pos, c->pos, v);
	}

	return c->f || c->b || c->l || c->r;
}