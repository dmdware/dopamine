

#include "anim.h"


dbool playanim(int* frame, int first, int last, dbool loop, int rate)
{
    if(*frame < first || *frame >= last)
    {
       *frame = first;
        return ecfalse;
    }

    *frame += rate;

    if(*frame > last)
    {
        if(loop)
            *frame = first;
		else
			*frame = last;

        return ectrue;
    }

    return ecfalse;
}

//Play animation backwards
dbool playanimb(int* frame, int first, int last, dbool loop, int rate)
{
    if(*frame < first-1 || *frame > last)
    {
        *frame = last;
        return ecfalse;
    }

    *frame -= rate;

    if(*frame < first)
    {
        if(loop)
            *frame = last;
		else
			*frame = first;

        return ectrue;
    }

    return ecfalse;
}
