

#include "walk.h"
#include "part.h"
#include "simvars.h"
#include "fab.h"
#include "../math/math3d.h"
#include "../math/plane3f.h"

void walk2(mp3f* p)
{
}

void disjoin(mp3f* p, mp3f* from)
{
	int i=0;
	for(; i<6; ++i)
	{
		if(p->neibs[i] == from)
			p->neibs[i] = NULL;
	}
}

//membrane manifold fabric sheet point
void pinit(mp3f* p)
{
	p->neibs = NULL;
	p->nneibs = 0;
	p->radpar = NULL;
	p->perpar1 = NULL;
	p->perpar2 = NULL;
	p->coneib1 = NULL;
	p->coneib2 = NULL;
	p->radsub = NULL;
	p->persub1 = NULL;
	p->persub2 = NULL;
}

//membrane manifold fabric sheet point
void pfree(mp3f* p)
{
	int i;

	if(!p)
		return;

	for(i=0; i<6; ++i)
	{
		if(p->neibs[i])
			disjoin(p->neibs[i], p);
	}

	for(i=0; i<6; ++i)
		pfree(p->neibs[i]);

	for(i=0; i<6; ++i)
		p->neibs[i] = NULL;

	free(p);
}

mp3f* pcommon(mp3f* a, mp3f* b)
{
	int i, j;
	for(i=0; i<6; ++i)
	{
		if(!a->neibs[i])
			continue;
		for(j=0; j<6; ++j)
		{
			if(!b->neibs[j])
				continue;
			if(a->neibs[i] == b->neibs[j])
				return a->neibs[i];
		}
	}
	return NULL;
}

int radlayer(int nmshots)
{
	return (nmshots / 6);
}

int israd(int si, int ri)
{
	return (si % (1 + ri) == 0);
}

void pplace(mp3f *neib1, mp3f *neib2, mp3f **last, mp3f **next, int nrshots, int nmshots, int si, mp3f **next2, mp3f *par1, mp3f *par2)
{
	//place first dot
	*next2 = (mp3f*)malloc(sizeof(mp3f));
	pinit(next2);

	(*next2)->neibs[1] = (*next2)->neibs[2] = *next2;
	(*next2)->nneibs += 2;

	*next = next2;
}

void fmplace(mp3f *neib1, mp3f *neib2, mp3f **last, mp3f **next, int nrshots, int nmshots, int si, mp3f **next2, mp3f *par1, mp3f *par2)
{
	//place first new dot of this radial layer
	*next2 = (mp3f*)malloc(sizeof(mp3f));
	pinit(next2);
	(*next2)->radpar = neib1;
	(*next2)->neibs[0] = neib1;
	//neibs are counted:
	//0: radial parent, and then 1,2: neib1,2 and then 3,4,5: left-to-right CW sub-dots with left,center,right, 
	//and the left,right are perimetric sub-dots
	//or
	//0,1: perimetric parent 1 and 2, and then 2,3: neib1,2 and then 4,5: perimetric shots on the left and right CW
	(*next2)->nneibs = 1;
	*next = *next2;
	//...
}

void mjoin(mp3f *neib1, mp3f *neib2, mp3f **last, mp3f **next, int nrshots, int nmshots, int si, mp3f **next2, mp3f *par1, mp3f *par2)
{
	//if the last one ("next") is a radial, add the neighbour accordingly
	if (israd(si, radlayer(nmshots)))
	{
		(*next)->neibs[1] = next2;
		(*next)->nneibs++;
	}
	//otherwise, add the neighbour according to perimetric-generated dot ordering
	else
	{
		(*next)->neibs[2] = next2;
		(*next)->nneibs++;
	}
	//and the parent is radial, so add this neighbour accordingly
	neib1->neibs[4] = next2;
	neib1->nneibs++;

	(*next2)->neibs[2] = *next;
	(*next2)->nneibs++;

	(*next2)->neibs[0] = neib1;
	(*next2)->nneibs++;

	//add the 3,4,5: left-to-right CW sub-dots with left,center,right, 
	//if those parents of this current dot are radial

	//neib2
	//neib1
	//neib0 (ccw-ward of neib1)

	//and otherwise if either of them are perimetric off-shots themselves, then 
	//4,5: perimetric shots on the left and right CW

	//neib2
	//neib1
	//neib0 (ccw-ward of neib1)

	*next = *next2;

	//if it is the last one, remember to connect the last new dot to the first new one in the radial layer,
	//and adjust the "next" to the that first one
	if (si == nrshots + nmshots - 1)
	{
		(*next2)->neibs[1] = *next;
		(*next2)->nneibs++;
		*next = neib1->radsub;
		(*next2)->coneib1 = *next;
	}
	//neib1 is left going ccw, according to right hand rule of cross product of 1.) walk vector with 2.) downward accel
}

void cmplace(mp3f *neib1, mp3f *neib2, mp3f **last, mp3f **next, int nrshots, int nmshots, int si, mp3f **next2, mp3f *par1, mp3f *par2)
{
	//continue along radial layer and advance "next" to the new dot
	*next2 = (mp3f*)malloc(sizeof(mp3f));
	pinit(next2);
	(*next)->coneib1 = *next2;
	(*next2)->coneib2 = *next;
	mjoin(neib1, neib2, last, next, nrshots, nmshots, si, next2, par1, par2);
}

//mid-edge shot
void mshot(mp3f *neib1, mp3f *neib2, mp3f **last, mp3f **next, int nrshots, int nmshots, int si, mp3f *par1, mp3f *par2)
{
	mp3f* next2;

	if (!*last)
	{
		pplace(neib1, neib2, last, next, nrshots, nmshots, si, &next2, par1, par2);
	}
	else if (!*next)
	{
		fmplace(neib1, neib2, last, next, nrshots, nmshots, si, &next2, par1, par2);
	}
	else
	{
		cmplace(neib1, neib2, last, next, nrshots, nmshots, si, &next2, par1, par2);
	}
}


void frplace(mp3f *neib1, mp3f *neib2, mp3f **last, mp3f **next, int nrshots, int nmshots, int si, mp3f **next2, mp3f *par1, mp3f *par2)
{
	//place first new dot of this radial layer
	*next2 = (mp3f*)malloc(sizeof(mp3f));
	pinit(next2);
	(*next2)->radpar = neib1;
	(*next2)->neibs[0] = neib1;
	//neibs are counted:
	//0: radial parent, and then 1,2: neib1,2 and then 3,4,5: left-to-right CW sub-dots with left,center,right, 
	//and the left,right are perimetric sub-dots
	//or
	//0,1: perimetric parent 1 and 2, and then 2,3: neib1,2 and then 4,5: perimetric shots on the left and right CW
	(*next2)->nneibs = 1;
	*next = *next2;
	//...
}

void rjoin(mp3f *neib1, mp3f *neib2, mp3f **last, mp3f **next, int nrshots, int nmshots, int si, mp3f **next2, mp3f *par1, mp3f *par2)
{
	int ri;

	ri = radlay(nmshots);

	//if the last one ("next") is a radial, add the neighbour accordingly
	if (israd(si, radlayer(nmshots)))
	{
		(*next)->neibs[1] = next2;
		(*next)->nneibs++;
	}
	//otherwise, add the neighbour according to perimetric-generated dot ordering
	else
	{
		(*next)->neibs[2] = next2;
		(*next)->nneibs++;
	}
	//and the parent is radial, so add this neighbour accordingly
	neib1->neibs[4] = next2;
	neib1->nneibs++;

	(*next2)->neibs[2] = *next;
	(*next2)->nneibs++;

	(*next2)->neibs[0] = neib1;
	(*next2)->nneibs++;

	if (ri > 1)
	{
		//add the 3,4,5: left-to-right CW sub-dots with left,center,right, 
		//if those parents of this current dot are radial

		//and otherwise if either of them are perimetric off-shots themselves, then 
		//4,5: perimetric shots on the left and right CW

		//neibs are counted:
		//0: radial parent, and then 1,2: neib1,2 and then 3,4,5: left-to-right CW sub-dots with left,center,right, 
		//and the left,right are perimetric sub-dots
		//or
		//0,1: perimetric parent 1 and 2, and then 2,3: neib1,2 and then 4,5: perimetric shots on the left and right CW

		//neib2 of parent of "next2"
		if ( israd((((si / 3) + ((ri - 1) * 6 + 6) - 1) % ((ri - 1) * 6 + 6)), ri-1) )
		{
			(*next2)->neibs[3] = *next2;
			(*next2)->nneibs ++;
		}
		else
		{
			(*next2)->neibs[3] = *next2;
			(*next2)->nneibs++;
		}
		//neib1 of parent of "next2"
		if ( israd(((si / 3)), ri-1) )
		{
			(*next2)->neibs[4] = *next2;
			(*next2)->nneibs++;
		}
		else
		{

		}
		//neib0 (ccw-ward of neib1) of parent of "next2"
		if ( israd((((si / 3) + ((ri - 1) * 6 + 6) + 1) % ((ri - 1) * 6 + 6)), ri-1) )
		{
			(*next2)->neibs[5] = *next2;
			(*next2)->nneibs++;
		}
		else
		{

		}
	}

	*next = *next2;

	//if it is the last one, remember to connect the last new dot to the first new one in the radial layer,
	//and adjust the "next" to the that first one
	if (si == nrshots + nmshots - 1)
	{
		(*next2)->neibs[1] = *next;
		(*next2)->nneibs++;
		*last = *next;
		*next = neib1->radsub;
		(*next2)->coneib1 = *next;
	}
	//neib1 is left going ccw, according to right hand rule of cross product of 1.) walk vector with 2.) downward accel
}

void crplace(mp3f *neib1, mp3f *neib2, mp3f **last, mp3f **next, int nrshots, int nmshots, int si, mp3f **next2, mp3f *par1, mp3f *par2)
{
	//continue along radial layer and advance "next" to the new dot
	*next2 = (mp3f*)malloc(sizeof(mp3f));
	pinit(next2);
	(*next)->coneib1 = *next2;
	(*next2)->coneib2 = *next;
	rjoin(neib1, neib2, last, next, nrshots, nmshots, si, next2, par1, par2);
}

//radial shot
void rshot(mp3f *neib1, mp3f *neib2, mp3f **last, mp3f **next, int nrshots, int nmshots, int si, mp3f *par1, mp3f *par2)
{
	mp3f* next2;

	if(!*last)
	{
		pplace(neib1, neib2, last, next, nrshots, nmshots, si, &next2, par1, par2);
	}
	else if(!*next)
	{
		frplace(neib1, neib2, last, next, nrshots, nmshots, si, &next2, par1, par2);
	}
	else
	{
		crplace(neib1, neib2, last, next, nrshots, nmshots, si, &next2, par1, par2);
	}
}

void walk()
{
	int ri, mshots, j, k, l;	//indices and the index of the left-over manifold points
	float lefta;	//left-over angle from all the currently projected vectors from current man. p.
	v3f shoot;	//the resulting new shoot-off vector for the next manifold point from the current man. p.
	float shoota;	//angle inside the new triangle, from the already-added edge off the current central man.p.
	p3f shootpl;	//planes, used for forming the new triangles, one for each new edge, to reflect a "shootv" vertically, while the plane gives a "horizontal" direction
	v3f shootv;	//vectors to the next 1-5 man.p's for more triangles of the mesh
	mp3f* commonm;	//manifold point shared between the current two off-points, to give the central point
	v3f commonv;	//vertex shared between two man.p's, meant to be only one, as we are adding on to the unfinished mesh
	v3f shoott[3];	//triangle, one for each possible triangle from up to 4 triangles from 5 man. p's from current m.p.
	mp3f *neib1, *neib2, **last, **next, *par1, *par2;

	*last = NULL;
	*next = NULL;
	par1 = NULL;
	par2 = NULL;

	for(ri=0; ri<10; ++ri)
	{
		//neib1 if radial is the actual current radial parent if moving left ccw
		mshots = 6 * ri;
		neib1 = NULL;
		neib2 = NULL;

		for(j=0; j<(6+mshots); ++j)
		{
			if(israd(j, ri))
				rshot(&neib1, &neib2, last, next, 6, mshots, j, par1, par2);
			else
				mshot(&neib1, &neib2, last, next, 6, mshots, j, par1, par2);

			if (ri > 1)
			{
				par2 = par1;
				par1 = par1->coneib1;
			}
		}

		if (ri > 0)
		{
			par1 = *last;
			par2 = par1->coneib2;
		}
	}

	for(i=0; i<10; i++)
	{
		left = 6 - pt[i].neibs;
		lefta = M_PI * 2;

		if(pt[i].nneibs>5)
			continue;

		for(k=0; k<pt[i].nneibs-1; ++k)
		{
			lefta -= angle3f(*pt[i].neib[k], *pt[i].neib[k+1]);
		}

		shoota = lefta / left;

		//fxmuli(&shoota, &g_xpi, 2);
		shoota = M_PI * 2;
		//fxdivi(&shoota, &shoota, (6 - left) + 1);
		shoota = shoota / ((6 - left) + 1);

		for(j=pt[i].nneibs-1; j<6; ++j)
		{
			if(!pt[i].neibs[j])
				continue;
			for(k=j+1; k<6; ++k)
			{
				if(!pt[i].neibs[k])
					continue;
				commonm = pcommon(pt[i].neibs[j], pt[i].neibs[k]);
				commonv = &commonm->v;
				//shootv[0] = tov3f(pt[i].neibs[k], commonv);
				//shootv[1] = tov3f(pt[i].neibs[l], commonv);
				break;
			}
		}

		makepl(&shootpl[0].normal, &shootpl[0].d, &commonaccel, &commonv);



		if(left == 4)
		{
			for(j=0; j<left; ++j)
			{

				shoott[0][0] = pt[i].neibs[k];
				shoott[0][1] = pt[i].neibs[l];
				shoott[0][2] = commonv;	//figure something out later to fix the normality clock-wise-ness
				//v3fadd(&shoott[0][0], shootv[0], commonv);
				//v3fadd(&shoott[0][1], shootv[1], commonv);
				shootnorm = normal(shoott[0]);
				offpl(&shootpl[0].normal, &shootpl[0].d, &commonv, &pt[i].neibs[l], shoota, &shootv[0]);
				offpl(&shootpl[0].normal, &shootpl[0].d, &commonv, &shootv[0], shoota, &shootv[1]);
				//and do something to make sure that the rays or vectors are always shot out of the plane in either cw or ccw order
				v3fsub(&shootv[0], &shootv[0], &commonv);
				v3fsub(&shootv[1], &shootv[1], &commonv);
				cross3f(&shootv[0], &commonaccel, &shootv[0]);
				cross3f(&shootv[1], &commonaccel, &shootv[1]);
				cross3f(&shootv[0], &commonaccel, &shootv[0]);
				cross3f(&shootv[1], &commonaccel, &shootv[1]);
				v3fadd(&shootv[0], &shootv[0], &commonv);
				v3fadd(&shootv[1], &shootv[1], &commonv);
			}

			shoot = 
				//angle along plane of angle "shoota" radians
				//then transform that vector with that 3-d angle,
				//by the magnitude and angle of the acceleration gravity force,
				//causing a "refraction"
		}
		else if(left == 3)
		{
		}
		else if(left == 2)
		{
		}
		else if(left == 1)
		{
		}
	}
}