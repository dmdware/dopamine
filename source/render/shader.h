

#ifndef SHADER_H
#define SHADER_H

#include "../sys/inc.h"
#include "../algo/bool.h"

#define SSLOT_TEXTURE0			0
#define SSLOT_POSITION			1
#define SSLOT_NORMAL			2
#define SSLOT_COLOR				3
#define SSLOT_WIDTH				4
#define SSLOT_HEIGHT			5
#define SSLOT_TEXCOORD0			6
#define SSLOT_MVP				7
#define SSLOT_CAMCEN			8
#define SSLOT_CORNERA			9
#define SSLOT_CORNERB			10
#define SSLOT_CORNERC			11
#define SSLOT_CORNERD			12
#define SSLOT_EP				13
#define SSLOT_MP				14
#define SSLOT_RX				15
#define SSLOT_VIEW				16
#define SSLOT_RIGHT				17
#define SSLOT_UP				18
#define SSLOT_SP				19
#define SSLOTS					20

struct glshader
{
	dbool hastexc;
	dbool hasnorm;
	GLint slot[SSLOTS];

	// http://lists.apple.com/archives/mac-opengl/2005/Nov/msg00182.html
#if defined( PLAT_MAC ) || defined( PLAT_IOS )
	GLuint vertshader;
	GLuint fragshader;
	GLuint program;
#else
	GLhandleARB vertshader;
	GLhandleARB fragshader;
	GLhandleARB program;
#endif
};

typedef struct glshader glshader;

#define SH_ORTHO				0
#define SH_COLOR2D				1
#define SH_COLOR3D				2
#define SH_GRID					3
#define SHADERS					4

extern glshader g_shader[SHADERS];
extern signed char g_cursh;

void shinit(glshader* s);
void shfree(glshader* s);

GLint shgu(glshader *s, const char* strvar);
GLint shga(glshader *s, const char* strvar);

void shmu(glshader *s, int slot, const char* variable);
void shma(glshader *s, int slot, const char* variable);

void usesh(int shader);
void endsh();
void inglsl();
void loadsh(int shader, 
			const char* namev,
			const char* namef,
			const char* strv, 
			const char* strf, 
			dbool hastexc, 
			dbool hasnorm);

extern const char *SHTEXT [SHADERS][4];

#endif


