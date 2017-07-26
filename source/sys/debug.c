#include "inc.h"
#include "debug.h"
#include "utils.h"
#include "../gui/gui.h"
#include "../gui/wg.h"
#include "syswin.h"
#include "../app/appmain.h"
#include "../algo/bool.h"

perftm g_prof[TIMERS];
dbool g_debug = dfalse;

dbool outmem(const char *file, int line)
{
	char msg[2048];
	sprintf(msg, "Failed to allocate memory in %s on line %d.", file, line);
	errm("Out of memory", msg);
	//g_quit = dtrue;
	return dtrue;
}

void tminit(perftm *t)
{
	t->averagems = 0.0;
	t->lastframeelapsed = 0;
	t->lastframe = 0;
	t->frames = 0;
	t->inside = -1;
}

void starttm(int i)
{
	g_prof[i].starttick = getticks();
}

void stoptm(int i)
{
    unsigned __int64 elapsed;
    
	if(g_appmode != APPMODE_PLAY)
		return;

	elapsed = getticks() - g_prof[i].starttick;
	g_prof[i].starttick = getticks();
	g_prof[i].lastframeelapsed += (int)elapsed;

	if(i == TM_FRAME || g_prof[i].lastframe < g_prof[TM_FRAME].lastframe)
	{
		g_prof[i].averagems = ( (double)g_prof[i].lastframeelapsed + g_prof[i].averagems*(double)g_prof[i].frames ) / (double)(g_prof[i].frames+1);
		g_prof[i].frames++;
		g_prof[i].lastframeelapsed = 0;
		g_prof[i].lastframe = g_prof[TM_FRAME].lastframe;
	}
	if(i == TM_FRAME)
		g_prof[i].lastframe++;
}

void wprofs(int in, int layer)
{
    double parentavgms, ofparentpct, totalms, totalofparentpct, percentage;
    int subprofiles, k;
    char full[DMD_MAX_PATH+1];
    static FILE *f;
    perftm *t;
	char j;
    
    subprofiles = 0;

	if(in == -1)
	{
		fullpath("profiles.txt", full);
		f = fopen(full, "w");
        t = &g_prof[TM_FRAME];
		parentavgms = t->averagems;
	}
	else
	{
		parentavgms = g_prof[in].averagems;
	}

    totalms = 0;
    totalofparentpct = 0;
    subprofiles = 0;

	for(j=0; j<TIMERS; j++)
    {
        t = &g_prof[j];
        
		if(t->inside != in)
			continue;

		totalms += g_prof[j].averagems;
	}

	for(j=0; j<TIMERS; j++)
	{
        t = &g_prof[j];
        
		if(t->inside != in)
			continue;

		percentage = 100.0 * t->averagems / totalms;
		ofparentpct = 100.0 * t->averagems / parentavgms;
		totalofparentpct += ofparentpct;
		subprofiles++;

		for(k=0; k<layer; k++)
			fprintf(f, "\t");

		fprintf(f, "%s\t...\t %lf ms per frame, %lf%% of this level's total\r\n", t->name, t->averagems, percentage);

		wprofs(j, layer+1);
	}

	if(subprofiles > 0)
	{
        for(k=0; k<layer; k++)
            fprintf(f, "\t");

		fprintf(f, "level total sum: %lf ms per frame, that means %lf%% of this parent's duration underwent profiling\r\n", totalms, totalofparentpct);
	}

	if(in == -1)
	{
        fclose(f);
	}
}

void deftm(int i, int inside, const char *name)
{
    perftm *t;
    
    t = &g_prof[i];
    
    t->inside = inside;
	strcpy(t->name, name);
}

void deftms()
{
	deftm(TM_FRAME, -1, "frame");
	deftm(TM_EVENT, TM_FRAME, "evproc");
	deftm(TM_DRAW, TM_FRAME, "draw");
	deftm(TM_UPDATE, TM_FRAME, "update");
}

#ifdef GLDEBUG
void glerr(char *file, int line)
{
	//char msg[2048];
	//sprintf(msg, "Failed to allocate memory in %s on line %d.", file, line);
	//errm("Out of memory", msg);
	int error = glGetError();

	if(error == GL_NO_ERROR)
		return;

	fprintf(g_applog, "GL Error #%d in %s on line %d using shader #%d\r\n", error, file, line, g_cursh);
}
#endif

#if !defined( PLAT_MAC ) && !defined( PLAT_IOS )
GLvoid APIENTRY glmess(GLenum source, GLenum type, GLuint i, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
//DEBUGPROC glmess(GLenum source, GLenum type, GLuint i, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
{
	//errm("GL Error", message);
	fprintf(g_applog, "GL Message: %s\r\n", message);
}
#endif


