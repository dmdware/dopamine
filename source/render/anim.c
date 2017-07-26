

#include "anim.h"


dbool playanim(int* frame, int first, int last, dbool loop, int rate)
{
    if(*frame < first || *frame >= last)
    {
       *frame = first;
        return dfalse;
    }

    *frame += rate;

    if(*frame > last)
    {
        if(loop)
            *frame = first;
		else
			*frame = last;

        return dtrue;
    }

    return dfalse;
}

//Play animation backwards
dbool playanimb(int* frame, int first, int last, dbool loop, int rate)
{
    if(*frame < first-1 || *frame > last)
    {
        *frame = last;
        return dfalse;
    }

    *frame -= rate;

    if(*frame < first)
    {
        if(loop)
            *frame = last;
		else
			*frame = first;

        return dtrue;
    }

    return dfalse;
}
