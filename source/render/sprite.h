











#ifndef SPRITE_H
#define SPRITE_H

#include "../sys/texture.h"
#include "../algo/bool.h"
#include "../algo/vec.h"

struct spt
{
	dbool on;
	unsigned int difftexi;
	unsigned int teamtexi;
	unsigned int depthtexi;
	unsigned int elevtexi;
	float offset[4];	//the pixel texture coordinates centered around a certain point, for use for vertex positions when not cropping (old)
	float crop[4];	//the texture coordinates [0..1] of the cropped sprite, used for the texture coords
	float cropoff[4];	//the pixel texture coordinates of the cropped sprite, used for vertex positions
	texdata* pixels;
	char* full;
};

typedef struct spt spt;

void spinit(spt *s);
void spfree(spt *s);

#define SPRITES	40
extern spt g_sp[SPRITES];

struct spat
{
	dbool on;
	dbool inclines;
	dbool sides;
	int nsides;
	dbool rotations;
	dbool frames;
	int nframes;
	int nslices;
	char* full;

	int nsp;
	unsigned int* sprites;

};

typedef struct spat spat;

void spatinit(spat *sl);
void spatfree(spat *sl);

#define SPATS	4
extern spat g_spat[SPATS];

struct spload
{
	char* relative;
	unsigned int* spin;
	dbool loadteam;
	dbool loaddepth;
};

typedef struct spload spload;

void sploadinit(spload *stl);
void sploadfree(spload *stl);

extern vector g_spload;

extern int g_lastloadsp;

dbool loadqsp();
void freesps();
dbool loadsp(const char* relative, unsigned int* spin, dbool loadteam, dbool loaddepth);
void queuesp(const char* relative, unsigned int* spin, dbool loadteam, dbool loaddepth);
void parsesp(const char* relative, spt* s);
int spref(spat* sl, int frame, int incline, int pitch, int yaw, int roll,
				  int slicex, int slicey);
dbool loadspat(const char* relative, unsigned int* splin, dbool loadteam, dbool loaddepth, dbool queue);
int newspat();
int newsp();

#define IN_0000		0
#define IN_0001		1
#define IN_0010		2
#define IN_0011		3
#define IN_0100		4
#define IN_0101		5
#define IN_0110		6
#define IN_0111		7
#define IN_1000		8
#define IN_1001		9
#define IN_1010		10
#define IN_1011		11
#define IN_1100		12
#define IN_1101		13
#define IN_1110		14
#define INCLINES	15

extern const char* INNAME[INCLINES];

#endif
