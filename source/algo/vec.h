

#ifndef VEC_H
#define VEC_H

#include "../sys/inc.h"

struct vector
{
	int capacity;
	int total;
	int unitsz;
	char *items;
};

typedef struct vector vector;

void vinit(vector *v, int unitsz);
void vfree(vector *v);
void vpub(vector *v, void *item);
void *vget(vector *v, int i);
void vpob(vector *v, void *dest);
void verase(vector *v, int index);
void vresize(vector *v, int capacity);

#endif