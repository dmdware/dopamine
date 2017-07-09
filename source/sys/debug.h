#ifndef DEBUG_H
#define DEBUG_H

#include "inc.h"
#include "../gui/richtext.h"
#include "../algo/bool.h"

#define	TM_FRAME				0
#define TM_EVENT				1
#define TM_UPDATE				2
#define TM_DRAW					3
#define TIMERS					4

struct perftm
{
	char name[64];
	double averagems;
	int lastframe;
	int frames;
	//double framems;
	unsigned __int64 starttick;
	//double timescountedperframe;
	//double lastframeaverage;
	int lastframeelapsed;
	int inside;
};

typedef struct perftm perftm;

extern perftm g_prof[TIMERS];

void tminit(perftm *t);

void starttm(int i);
void stoptm(int i);
void deftm(int i, int inside, const char *name);
void wprofs(int in, int layer);
void deftms();

#ifdef GLDEBUG
void glerr(char *file, int line);
#endif

#ifndef MATCHMAKER
#if !defined( PLAT_MAC ) && !defined( PLAT_IOS )
GLvoid APIENTRY glmess(GLenum source, GLenum type, GLuint i, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam);
//DEBUGPROC glmess(GLenum source, GLenum type, GLuint i, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam);
#endif
#endif


dbool outmem(const char *file, int line);

#define OUTMEM()	outmem(__FILE__,__LINE__)

#define NOMEM(x)	((!(x)) && OUTMEM())

#endif	//DEBUG_H
