











#include "sprite.h"
#include "../sys/utils.h"
#include "../sys/texture.h"
#include "../gui/gui.h"
#include "../sys/debug.h"

vector g_spload; /* spload */
int g_lastloadsp = -1;
spt g_sp[SPRITES];
spat g_spat[SPATS];

const char* INNAME[] =
{
	"0000",
	"0001",
	"0010",
	"0011",
	"0100",
	"0101",
	"0110",
	"0111",
	"1000",
	"1001",
	"1010",
	"1011",
	"1100",
	"1101",
	"1110"
};

void spinit(spt *s)
{
	s->on = dfalse;
	s->difftexi = 0;
	s->teamtexi = 0;
	s->pixels = NULL;
}

void spfree(spt *s)
{
	if(s->pixels)
		ltexfree(s->pixels);
	free(s->pixels);
	s->pixels = NULL;
	s->on = dfalse;
}

void spatinit(spat *sl)
{
	sl->nsp = 0;
	sl->sprites = NULL;
	sl->on = dfalse;
	sl->nslices = 1;
	sl->full = NULL;
}

void spatfree(spat *sl)
{
	sl->nsp = 0;
	free(sl->sprites);
	sl->sprites = NULL;
	sl->on = dfalse;
	sl->nslices = 1;
	free(sl->full);
	sl->full = NULL;
}

void sploadinit(spload *stl)
{
	stl->relative = NULL;
	stl->spin = NULL;
}

void sploadfree(spload *stl)
{
	free(stl->relative);
	free(stl->spin);
	stl->relative = NULL;
	stl->spin = NULL;
}

void freesps()
{
	spt *s;
	spat *sl;

	for(s=g_sp; s<g_sp+SPRITES; ++s)
	{
		if(!s->on)
			continue;

		spfree(s);
	}
	
	for(sl=g_spat; sl<g_spat+SPATS; ++sl)
	{
		if(!sl->on)
			continue;

		spatfree(sl);
	}
}

dbool loadqsp()
{
	spload* s;
	char m[128];

	if(g_lastloadsp+1 < g_spload.total)
	{
		s = (spload*)vget(&g_spload, g_lastloadsp+1);
		setstat(s->relative);
	}

	CHECKGL();

	if(g_lastloadsp >= 0)
	{
		s = (spload*)vget(&g_spload, g_lastloadsp);
		if(!loadsp(s->relative, s->spin, s->loadteam, s->loaddepth))
		{
			sprintf(m, "Failed to load sprite %s", s->relative);
			errm("Error", m);
		}
	}

	++g_lastloadsp;

	if(g_lastloadsp >= g_spload.total)
	{
		return dfalse;	// Done loading all
	}

	return dtrue;	// Not finished loading
}

void queuesp(const char* relative, unsigned int* spin, dbool loadteam, dbool loaddepth)
{
	spload stl;
	pstrset(&stl.relative, relative);
	stl.spin = spin;
	stl.loadteam = loadteam;
	stl.loaddepth = loaddepth;
	vpub(&g_spload, &stl);
}

int newsp()
{
	spt* s;
	int i;

	for(i=0; i<SPRITES; ++i)
	{
		s = g_sp+i;

		if(!s->on)
			return i;
	}

	return -1;
}

int newspat()
{
	spat* sl;
	int i;

	for(i=0; i<SPATS; ++i)
	{
		sl = g_spat+i;

		if(!sl->on)
			return i;
	}

	return -1;
}

dbool findsp(unsigned int *spin, const char* relative)
{
	char c[DMD_MAX_PATH+1];
	char full[DMD_MAX_PATH+1];
	int i;
	spt* s;
	strcpy(c, relative);
	corslash(c);
	fullpath(c, full);

	for(i=0; i<SPRITES; ++i)
	{
		s = g_sp+i;

		if(s->on && strcmp(s->full, full) == 0)
		{
			*spin = i;
			return dtrue;
		}
	}

	return dfalse;
}

/*
TODO
Convert to C90
All upper case to lower case
createtex -> createtex
All class to struct
texdata -> ltext or texdata
*/

dbool loadsp(const char* relative, unsigned int* spin, dbool loadteam, dbool loaddepth)
{
	int i;
	spt* s;
	char full[DMD_MAX_PATH+1];
	char reltxt[DMD_MAX_PATH+1];
	char reldiff[DMD_MAX_PATH+1];
	char relteam[DMD_MAX_PATH+1];
	char reldepth[DMD_MAX_PATH+1];
	char pixfull[DMD_MAX_PATH+1];

	if(findsp(spin, relative))
		return dtrue;

	i = newsp();

	if(i < 0)
		return dfalse;

	s = g_sp+i;
	s->on = dtrue;
	*spin = i;

	fullpath(relative, full);
	corslash(full);
	s->full = full;

	sprintf(reltxt, "%s.txt", relative);
	sprintf(reldiff, "%s.png", relative);
	sprintf(relteam, "%s_team.png", relative);
	sprintf(reldepth, "%s_depth.png", relative);
	parsesp(reltxt, s);

	createtex(&s->difftexi, reldiff, dtrue, dfalse, dfalse);
	if(loadteam)
		createtex(&s->teamtexi, relteam, dtrue, dfalse, dfalse);
	if(loaddepth)
		createtex(&s->depthtexi, reldepth, dtrue, dfalse, dfalse);
	
	fullpath(reldiff, pixfull);
	s->pixels = loadtex(pixfull);

	if(!s->pixels)
	{
		fprintf(g_applog, "Failed to load sprite %s\r\n", relative);
		return dfalse;
	}
	else
		fprintf(g_applog, "%s\r\n", relative);

	return dtrue;
}

dbool loadspat(const char* relative, unsigned int* splin, dbool loadteam, dbool loaddepth, dbool queue)
{
	int i;
	spat* sl;
	char full[DMD_MAX_PATH+1];
	char txtpath[DMD_MAX_PATH+1];
	int nsides;
	dbool dorots;	//rotations
	dbool dosides;	//sides
	dbool doincls;	//inclines
	dbool doframes;	//frames
	int nframes;
	FILE* fp;
	char line[128];
	char* tok;
	int ncombos;
	int sidechunk;
	int inclchunk;
	int frameschunk;
	int incli;
	int pitchi;
	int yawi;
	int rolli;
	int sidei;
	int framei;
	int ci;
	char combo[DMD_MAX_PATH+1];
	char frame[32];
	char side[32];
	char stage[32];
	char incline[32] = "";

	i = newspat();

	if(i < 0)
		return dfalse;

	sl = &g_spat[i];
	sl->on = dtrue;
	*splin = i;
	
	fullpath(relative, full);
	corslash(full);
	pstrset(&sl->full, full);

	sprintf(txtpath, "%s_list.txt", full);

	nsides = 1;
	dorots = dfalse;	//rotations
	dosides = dfalse;	//sides
	doincls = dfalse;	//inclines
	doframes = dfalse;	//frames
	nframes = 1;

	fp = fopen(txtpath, "r");

	if(!fp)
		return dfalse;

	do
	{
		fgets(line, 100, fp);

		tok = strtok(line, " ");

		if(!tok)
			continue;

		if(strcmp(tok, "inclines") == 0)
		{
			tok = strtok(NULL, " ");

			if(!tok)
				continue;

			sscanf(tok, "%hhu", &doincls);
		}
		else if(strcmp(tok, "frames") == 0)
		{
			tok = strtok(NULL, " ");

			if(!tok)
				continue;

			sscanf(tok, "%hhu", &doframes);

			tok = strtok(NULL, " ");

			if(!doframes || !tok)
				continue;

			sscanf(tok, "%d", &nframes);
		}
		else if(strcmp(tok, "sides") == 0)
		{
			tok = strtok(NULL, " ");

			if(!tok)
				continue;

			sscanf(tok, "%hhu", &dosides);

			tok = strtok(NULL, " ");

			if(!dosides || !tok)
				continue;

			sscanf(tok, "%d", &nsides);
		}
		else if(strcmp(tok, "rotations") == 0)
		{
			tok = strtok(NULL, " ");

			if(!tok)
				continue;

			sscanf(tok, "%hhu", &dorots);

			tok = strtok(NULL, " ");

			if(!dorots || !tok)
				continue;

			sscanf(tok, "%d", &nsides);
		}
	}while(!feof(fp));

	fclose(fp);

	ncombos = 1;
	sidechunk = 1;
	inclchunk = 1;
	frameschunk = 1;

	if(dorots)
	{
		ncombos *= nsides;
		ncombos *= nsides;
		ncombos *= nsides;
		sidechunk = nsides * nsides * nsides;
	}
	else if(dosides)
	{
		ncombos *= nsides;
		sidechunk = nsides;
	}

	inclchunk = sidechunk;

	if(doincls)
	{
		ncombos *= INCLINES;
		inclchunk = sidechunk * INCLINES;
	}

	frameschunk = inclchunk;

	if(doframes)
	{
		ncombos *= nframes;
		frameschunk = inclchunk * nframes;
	}

	sl->frames = doframes;
	sl->nframes = nframes;
	sl->inclines = doincls;
	sl->nsides = nsides;
	sl->sides = dosides;
	sl->rotations = dorots;
	sl->sprites = (unsigned int*)malloc( sizeof(unsigned int) * ncombos );

	NOMEM(sl->sprites);

	incli = 0;
	pitchi = 0;
	yawi = 0;
	rolli = 0;
	sidei = 0;
	framei = 0;

	for(ci=0; ci<ncombos; ci++)
	{
		if(dorots)
		{
			yawi = ci % nsides;
			pitchi = (ci / nsides) % nsides;
			rolli = (ci / nsides / nsides) % nsides;
		}
		else if(dosides)
		{
			sidei = ci % nsides;
		}

		if(doincls)
		{
			incli = (ci / sidechunk) % INCLINES;
		}

		if(doframes)
		{
			framei = (ci / inclchunk) % nframes;
		}

		strcpy(frame, "");
		strcpy(side, "");

		if(doframes)
			sprintf(frame, "_fr%03d", framei);

		if(dosides && !dorots)
			sprintf(side, "_si%d", sidei);
		else if(dorots)
			sprintf(side, "_y%dp%dr%d", yawi, pitchi, rolli);

		//std::string incline = "";

		//TODO use INNAME
		if(doincls)
		{
#if 0
			if(incli == IN_0000)	incline = "_inc0000";
			else if(incli == IN_0001)	incline = "_inc0001";
			else if(incli == IN_0010)	incline = "_inc0010";
			else if(incli == IN_0011)	incline = "_inc0011";
			else if(incli == IN_0100)	incline = "_inc0100";
			else if(incli == IN_0101)	incline = "_inc0101";
			else if(incli == IN_0110)	incline = "_inc0110";
			else if(incli == IN_0111)	incline = "_inc0111";
			else if(incli == IN_1000)	incline = "_inc1000";
			else if(incli == IN_1001)	incline = "_inc1001";
			else if(incli == IN_1010)	incline = "_inc1010";
			else if(incli == IN_1011)	incline = "_inc1011";
			else if(incli == IN_1100)	incline = "_inc1100";
			else if(incli == IN_1101)	incline = "_inc1101";
			else if(incli == IN_1110)	incline = "_inc1110";
#else
			sprintf(incline, "_inc%s", INNAME[incli]);
#endif
		}

		strcpy(stage, "");
#if 0
		if(rendstage == RENDSTAGE_TEAM)
			stage = "_team";
		else if(rendstage == RENDSTAGE_DEPTH)
			stage = "_depth";
#endif

		sprintf(combo, "%s%s%s%s%s", relative, side, frame, incline, stage);

		if(queue)
			queuesp(combo, &sl->sprites[ci], loadteam, loaddepth);
		else
			loadsp(combo, &sl->sprites[ci], loadteam, loaddepth);
	}

	return dtrue;
}

int spref(spat* sl, int frame, int incline, int pitch, int yaw, int roll,
				  int slicex, int slicey)
{
	//int ncombos = 1;
	int sidechunk;
	int inclchunk;
	int frameschunk;
	int sliceschunk;
	int ci;

	sidechunk = 1;
	inclchunk = 1;
	frameschunk = 1;
	sliceschunk = 1;

	if(sl->rotations)
	{
		sidechunk = sl->nsides * sl->nsides * sl->nsides;
	}
	else if(sl->sides)
	{
		sidechunk = sl->nsides;
	}
	
	inclchunk = sidechunk;

	if(sl->inclines)
	{
		inclchunk = sidechunk * INCLINES;
	}

	frameschunk = inclchunk;

	if(sl->frames)
	{
		frameschunk = inclchunk * sl->nframes;
	}
	
	sliceschunk = frameschunk;

	if(sl->nslices > 1)
	{
		sliceschunk = frameschunk * sl->nslices * sl->nslices;
	}

	ci = 0;
	
	if(sl->rotations)
	{
		ci += yaw + pitch * sl->nsides + roll * sl->nsides * sl->nsides;
	}
	else if(sl->sides)
	{
		ci += yaw;
	}

	if(sl->inclines)
	{
		ci += sidechunk * incline;
	}

	if(sl->frames)
	{
		ci += inclchunk * frame;
	}

	if(sl->nslices > 1)
	{
		ci += frameschunk * ( sl->nslices * slicey + slicex );
	}

	return ci;
}

//TODO size up to power of 2 for mobile etc
void parsesp(const char* relative, spt* s)
{
	char full[DMD_MAX_PATH+1];
	FILE* fp;
	float centerx;
	float centery;
	float width;
	float height;
	float clipszx, clipszy;
	float clipminx, clipminy, clipmaxx, clipmaxy;

	fullpath(relative, full);

	fp = fopen(full, "r");
	if(!fp) return;

	fscanf(fp, "%f %f", &centerx, &centery);
	fscanf(fp, "%f %f", &width, &height);
	fscanf(fp, "%f %f", &clipszx, &clipszy);
	fscanf(fp, "%f %f %f %f", &clipminx, &clipminy, &clipmaxx, &clipmaxy);
	
	s->offset[0] = -centerx;
	s->offset[1] = -centery;
	s->offset[2] = s->offset[0] + width;
	s->offset[3] = s->offset[1] + height;

	s->crop[0] = 0;
	s->crop[1] = 0;
	s->crop[2] = clipszx / width;
	s->crop[3] = clipszy / height;

	s->cropoff[0] = -centerx;
	s->cropoff[1] = -centery;
	s->cropoff[2] = clipszx - centerx;
	s->cropoff[3] = clipszy - centery;

	fclose(fp);
}
