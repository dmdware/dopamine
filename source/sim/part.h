

#ifndef PART_H
#define PART_H

#include "../math/v3f.h"

struct part3f
{
	v3f pos;
	float mass;
};

typedef struct part3f part3f;

#define PARTICLES	2

#endif