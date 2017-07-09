

#include "vec.h"
#include "bool.h"
#include "../sys/debug.h"

void vinit(vector *v, int unitsz)
{
	v->items = NULL;
	v->total = 0;
	v->capacity = 0;
	v->unitsz = unitsz;
}

void vfree(vector *v)
{
	free(v->items);
	v->items = NULL;
	//is it the responsibility of the vector to free the item contents?
	
	v->total = 0;
	v->capacity = 0;
	v->unitsz = 0;
}

//"push back"
void vpub(vector *v, void *item)
{
	if(v->total == v->capacity)
		vresize(v, v->capacity * 2);
	
	memcpy(vget(v, v->total), item, v->unitsz);
	++v->total;
}

void *vget(vector *v, int i)
{
	return v->items + i*v->unitsz;
}

//"pop back"
void vpob(vector *v, void *dest)
{
	void *item;
	
	item = vget(v, v->total-1);
	
	memcpy(dest, item, v->unitsz);
	memset(item, 0, v->unitsz);
	
	--v->total;
}

void verase(vector *v, int index)
{
	char *last, *ati;
	
	ati = vget(v, index);
	last = vget(v, v->total-1);

	for(; ati<last; ati+=v->unitsz)
		memcpy(ati, ati+v->unitsz, v->unitsz);
	
	memset(last, 0, v->unitsz);
	
	--v->total;
	
	if(v->total > 0 && v->total == v->capacity / 4)
		vresize(v, v->capacity / 2);
}

void vresize(vector *v, int capacity)
{
	char *items;
 
	items = realloc(v->items, v->unitsz * capacity);
	
	if(items)
	{
		v->items = items;
		v->capacity = capacity;
	}
	else
		OUTMEM();
}
