

#ifndef WALK_H
#define WALK_H

struct mp3f
{
	v3f v;
	struct mp3f *neibs[6];	//up to all 6 total connected neighbours of this point
	char nneibs;
	struct mp3f *radpar;	//radial parent, from the center of the starting point, to make the sheet
	struct mp3f *perpar1, *perpar2;	//perimetric parents, from around the generational radius of this mp3f, 
		//if it is created by a non-radial parent
	//some generational radii are connected by perimetric lines simply by connecting that generation together,
	//the connected neighbours
	struct mp3f *coneib1, *coneib2;	//left and right, in right hand rule, with cross of parent radial shot with downward gravity,
		//and thus ccw
	//others after the first expansion, are shot by the line perpendicular along the plane, 
	//reflected or angled by the acceleration at that point of origin perhaps,
	//perpendicular to the edge shared between two points, such that,
	//generation 0 has 6 radial shots, which are then perimetrically connected,
	//generation 1 has alternating 6 shots, with 6 mid-edge off-shots that are perpendicular to that edge,
	//which are all then connected for 12 points,
	//and generation 2 will have alternating radial shots and mid-edge off-shots, with 6 radial, in between them,
	//2 mid-edge off-shots each, with a total of 18 perimetrically connected points
	struct mp3 *radsub;
	struct mp3 *persub1, *persub2;
	//for mid-edge shot-off points, there is no radial parent
	//for radial off-shot points, there are no perimetric parents
};

typedef struct mp3f mp3f;

void walk();

#endif