



#include "shader.h"
#include "../sys/syswin.h"
#include "../sys/debug.h"
#include "../sys/utils.h"

glshader g_shader[SHADERS];
signed char g_cursh = 0;

const char *SHTEXT[SHADERS][4]
=
{
	/* SH_ORTHO */
	{
"SH_ORTHO v",
"SH_ORTHO f",
"														\r\n"
"#version 120											\r\n"\
"														\r\n"\
"														\r\n"\
"uniform float width;									\r\n"\
"uniform float height;									\r\n"\
"														\r\n"\
"void main(void)										\r\n"\
"{														\r\n"\
"	gl_Position = vec4(gl_Vertex.x * 2.0 / width - 1.0,	\r\n"\
"		gl_Vertex.y * -2.0 / height + 1.0,				\r\n"\
"		0.5,											\r\n"\
"		1.0);											\r\n"\
"														\r\n"\
"	gl_TexCoord[0] = gl_MultiTexCoord0;					\r\n"\
"}														\r\n"\
"														\r\n",

"														\r\n"\
"#version 120											\r\n"\
"														\r\n"\
"uniform vec4 color;									\r\n"\
"														\r\n"\
"uniform sampler2D texture0;							\r\n"\
"														\r\n"\
"void main(void)										\r\n"\
"{														\r\n"\
"	gl_FragColor = color * texture2D(texture0, gl_TexCoord[0].xy);\r\n"\
"}														\r\n"\
"														\r\n"
	},

	/* SH_COLOR2D */
	{
"SH_COLOR2D v",
"SH_COLOR2D f",
"														\r\n"\
"#version 120											\r\n"\
"														\r\n"\
"														\r\n"\
"uniform float width;									\r\n"\
"uniform float height;									\r\n"\
"														\r\n"\
"														\r\n"\
"void main(void)										\r\n"\
"{														\r\n"\
"	gl_Position = vec4(gl_Vertex.x * 2.0 / width - 1.0,	\r\n"\
"		gl_Vertex.y * -2.0 / height + 1.0,				\r\n"\
"		0.5,											\r\n"\
"		1.0);											\r\n"\
"														\r\n"\
"}														\r\n"\
"														\r\n",
"														\r\n"\
"#version 120											\r\n"\
"														\r\n"\
"uniform vec4 color;									\r\n"\
"														\r\n"\
"														\r\n"\
"void main(void)										\r\n"\
"{														\r\n"\
"	gl_FragColor = color;								\r\n"\
"}														\r\n"\
"														\r\n"\
	},
	/* SH_COLOR3D */
	{
		"SH_COLOR3D v",
		"SH_COLOR3D f",
#include "../shaders/color3d.vert"
		,
#include "../shaders/color3d.frag"
	},
/* SH_GRID */
{
	"SH_GRID v",
	"SH_GRID f",
#include "../shaders/grid.vert"
	,
#include "../shaders/grid.frag"
}
};

void shinit(glshader* s)
{
	s->program = 0;
	s->vertshader = 0;
	s->fragshader = 0;
}

void shfree(glshader* s)
{
	if(s->vertshader)
	{
		glDetachShader(s->program, s->vertshader);
		glDeleteShader(s->vertshader);
		s->vertshader = 0;
	}

	if(s->fragshader)
	{
		glDetachShader(s->program, s->fragshader);
		glDeleteShader(s->fragshader);
		s->fragshader = 0;
	}

	if(s->program)
	{
		glDeleteProgram(s->program);
		s->program = 0;
	}
}

GLint shgu(glshader* s, const char* strvar)
{
	if(!s->program)
		return -1;
	return glGetUniformLocation(s->program, strvar);
}

GLint shga(glshader* s, const char* strvar)
{
	if(!s->program)
		return -1;
	return glGetAttribLocation(s->program, strvar);
}

void shmu(glshader* s, int slot, const char* variable)
{
	s->slot[slot] = shgu(s, variable);
}

void shma(glshader* s, int slot, const char* variable)
{
	s->slot[slot] = shga(s, variable);
}

void glver(int* major, int* minor)
{
	/* for all versions */
	char* ver;

	char vermaj[6];
	char vermin[6];

	int i;
	int j;

	ver = (char*)glGetString(GL_VERSION); /* ver = "3.2.0" */

	for(i=0; i<6; i++)
	{
		if(ver[i] != '.')
			vermaj[i] = ver[i];
		else
		{
			vermaj[i] = '\0';
			i++;
			break;
		}
	}

	for(j=0; j<6; j++, i++)
	{
		if(ver[i] != '.')
			vermin[j] = ver[i];
		else
		{
			vermin[j] = '\0';
			i++;
			break;
		}
	}

	sscanf(vermaj, "%d", major);
	sscanf(vermin, "%d", minor);

	ver = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void inglsl()
{
	int major, minor;
	GLenum glewError;
	char* glexts;

#ifndef PLAT_MAC
	glewExperimental = GL_TRUE;
	glewError = glewInit();
	if( glewError != GLEW_OK )
	{
		errm("Error initializing GLEW!", (const char*)glewGetErrorString( glewError ));
		return;
	}
#endif

#ifdef __glew_h__
	if( !GLEW_VERSION_1_4 )
	{
		errm("Error", "OpenGL 1.4 not supported!\n" );
		goto quit;
	}
#endif

#ifndef PLAT_MOBILE
	/* glGetString deprecated after 3.0 */
	glexts = (char*)glGetString(GL_EXTENSIONS);

	fprintf(g_applog, glexts);

#if !defined( PLAT_MAC ) && !defined( PLAT_IOS )
#ifdef GLDEBUG
	if(!strstr(glexts, "GL_ARB_debug_output"))
	{
		fprintf(g_applog, "GL_ARB_debug_output extension not supported\r\n");
	}
	else
	{
		glDebugMessageCallback(&glmess, 0);

		CHECKGL();
	}
#endif
#endif

	if(!strstr(glexts, "GL_ARB_shader_objects"))
	{
		errm("Error", "GL_ARB_shader_objects extension not supported!");
		goto quit;
	}

	if(!strstr(glexts, "GL_ARB_shading_language_100"))
	{
		errm("Error", "GL_ARB_shading_language_100 extension not supported!");
		goto quit;
	}
#endif

	glver(&major, &minor);

#ifndef PLAT_MOBILE
	if(major < 1 || ( major == 1 && minor < 4 ))
	{
		errm("Error", "OpenGL 1.4 is not supported!");
		goto quit;
	}
#endif

	loadsh(SH_ORTHO, SHTEXT[SH_ORTHO][0], SHTEXT[SH_ORTHO][1], 
		SHTEXT[SH_ORTHO][2], SHTEXT[SH_ORTHO][3], 
		dtrue, dfalse);
	loadsh(SH_COLOR2D, SHTEXT[SH_COLOR2D][0], SHTEXT[SH_COLOR2D][1], 
		SHTEXT[SH_COLOR2D][2], SHTEXT[SH_COLOR2D][3], 
		dfalse, dfalse);
	loadsh(SH_COLOR3D, SHTEXT[SH_COLOR3D][0], SHTEXT[SH_COLOR3D][1], 
		SHTEXT[SH_COLOR3D][2], SHTEXT[SH_COLOR3D][3], 
		dfalse, dfalse);
	loadsh(SH_GRID, SHTEXT[SH_GRID][0], SHTEXT[SH_GRID][1],
		SHTEXT[SH_GRID][2], SHTEXT[SH_GRID][3],
		dfalse, dfalse);

	return;

quit:
	g_quit = dtrue;
}

void loadsh(int shader, 
			const char* namev,
			const char* namef,
			const char* strv, 
			const char* strf, 
			dbool hastexc, 
			dbool hasnorm)
{
	glshader* s;
	GLint loglen;
	GLchar* log;
	GLint status;

	s = g_shader+shader;

	if(s->vertshader || s->fragshader || s->program)
		shfree(s);

	s->hastexc = hastexc;
	s->hasnorm = hasnorm;
	s->vertshader = glCreateShader(GL_VERTEX_SHADER);
	s->fragshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(s->vertshader, 1, &strv, NULL);
	glShaderSource(s->fragshader, 1, &strf, NULL);

	glCompileShader(s->vertshader);

	glGetShaderiv(s->vertshader, GL_INFO_LOG_LENGTH, &loglen);
	if(loglen > 0)
	{
		log = (GLchar*)malloc(loglen);

		if(NOMEM(log))
			return;

		glGetShaderInfoLog(s->vertshader, loglen, &loglen, log);
		fprintf(g_applog, "shader %s compile log: %s\r\n", namev, log);
		fflush(g_applog);
		free(log);
	}

	glCompileShader(s->fragshader);
	glGetShaderiv(s->fragshader, GL_INFO_LOG_LENGTH, &loglen);
	if(loglen > 0)
	{
		log = (GLchar*)malloc(loglen);

		if(NOMEM(log))
			return;

		glGetShaderInfoLog(s->fragshader, loglen, &loglen, log);
		fprintf(g_applog, "shader %s compile log: %s\r\n", namef, log);
		fflush(g_applog);
		free(log);
	}

	s->program = glCreateProgram();
	glAttachShader(s->program, s->vertshader);
	glAttachShader(s->program, s->fragshader);
	glLinkProgram(s->program);

	fprintf(g_applog, "Program %s / %s :", namev, namef);
	fflush(g_applog);

	glGetProgramiv(s->program, GL_INFO_LOG_LENGTH, &loglen);
	if (loglen > 0)
	{
		log = (GLchar*)malloc(loglen);
		glGetProgramInfoLog(s->program, loglen, &loglen, log);
		fprintf(g_applog, "Program link log: %s\r\n", log);
		fflush(g_applog);
		free(log);
	}

	glGetProgramiv(s->program, GL_LINK_STATUS, &status);
	if (status == 0)
	{
		fprintf(g_applog, "link status 0\r\n");
		fflush(g_applog);
	}
	else
	{
		fprintf(g_applog, "link status ok, program=%d\r\n", s->program);
		fflush(g_applog);
	}

	fprintf(g_applog, "\r\n");
	fflush(g_applog);

	shma(s, SSLOT_POSITION, "position");
	shma(s, SSLOT_NORMAL, "normalin");
	shma(s, SSLOT_TEXCOORD0, "texcoordin0");
	shmu(s, SSLOT_TEXTURE0, "texture0");
	shmu(s, SSLOT_COLOR, "color");
	shmu(s, SSLOT_WIDTH, "width");
	shmu(s, SSLOT_HEIGHT, "height");
	shmu(s, SSLOT_MVP, "mvp");
	shmu(s, SSLOT_CAMCEN, "camcen");
	shmu(s, SSLOT_CORNERA, "cornera");
	shmu(s, SSLOT_CORNERB, "cornerb");
	shmu(s, SSLOT_CORNERC, "cornerc");
	shmu(s, SSLOT_CORNERD, "cornerd");
	shmu(s, SSLOT_EP, "ep");
	shmu(s, SSLOT_MP, "mp");
	shmu(s, SSLOT_RX, "rx");
	shmu(s, SSLOT_VIEW, "view");
	shmu(s, SSLOT_RIGHT, "right");
	shmu(s, SSLOT_UP, "up");
	shmu(s, SSLOT_SP, "sp");
}

void usesh(int shader)
{
	glshader* s;

	CHECKGL();
	g_cursh = shader;

	s = g_shader+g_cursh;

	glUseProgram(s->program);
	CHECKGL();

#ifdef PLAT_GL14
	/* opengl 1.4 way */
	glEnableClientState(GL_VERTEX_ARRAY);
	if(s->hastexc)	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	if(s->hasnorm)	glEnableClientState(GL_NORMAL_ARRAY);
#endif
}

void endsh()
{
	glshader* s;
	CHECKGL();

	if(g_cursh < 0)
		return;

	s = g_shader+g_cursh;

#ifdef PLAT_GL14
	//opengl 1.4 way
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
#endif

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(0);

	g_cursh = -1;
}
