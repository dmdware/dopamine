


#include "syswin.h"
#include "../algo/bool.h"
#include "../gui/wg.h"
#include "../gui/gui.h"
#include "../render/shader.h"

#ifndef MATCHMAKER
#endif

dbool g_quit = dfalse;
dbool g_bg = dfalse;
dbool g_fs = dtrue;
v2i g_selres;
list g_ress; /* v2i */
list g_bpps; /* char */
double g_indrawfps = 0;
double g_inupfps = 0;
double g_drawiv = 0;
double g_upiv = 0;

#ifndef MATCHMAKER
int g_currw;
int g_currh;
int g_width = INI_WIDTH;
int g_height = INI_HEIGHT;
int g_bpp = INI_BPP;
v2i g_mouse;
v2i g_mousestart;
dbool g_keyin = dfalse;
dbool g_keys[SDL_NUM_SCANCODES] = {0};
dbool g_mousekeys[5] = {0};
float g_zoom = INI_ZOOM;
dbool g_mouseout = dfalse;
int g_curst = CU_DEFAULT;	//cursor state
int g_kbfocus = 0;	//keyboad focus counter
#endif

void enumdisp()
{
	short i;
	SDL_DisplayMode mode;
	dbool found;
	lnode* rit;	/* v2i */
	v2i* rp;
	v2i r;

	lfree(&g_ress);

	for(i=0; i<SDL_GetNumDisplayModes(0); ++i)
	{
		SDL_GetDisplayMode(0, i, &mode);

		found = dfalse;

		for(rit=g_ress.head; rit; rit=rit->next)
		{
			rp = (v2i*)rit->data;

			if(rp->x == mode.w &&
				rp->y == mode.h)
			{
				found = dtrue;
				break;
			}
		}

		if(found)
			continue;

		r.x = mode.w;
		r.y = mode.h;
		lpub2(&g_ress, sizeof(r), &r);
	}
}

void resize(int width, int height)
{
	wg* gui;

	if(height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	gui = (wg*)&g_gui;

	g_width = width;
	g_height = height;

	wgreframe(gui);
}

void calcdrawrate()
{
	static unsigned __int64 lasttime;
	static unsigned __int64 frametime;
	static int framecounter;

	unsigned __int64 currtime = getticks();

	g_drawiv = (double)(currtime - frametime) / 1000.0f;

	frametime = currtime;

	++framecounter;

	if( currtime - lasttime > 1000 )
	{
		g_indrawfps = framecounter;

		lasttime = currtime;

		framecounter = 0;
	}
}

dbool drawnext()
{
	static unsigned __int64 lastdrawtick;
	static unsigned __int64 elapseddrawtime;

	unsigned __int64 currentTime = getticks();
	int desiredMSPF = 1000/DRAW_FRAME_RATE;
	unsigned __int64 deltaTime = (currentTime - lastdrawtick);

	if(deltaTime >= desiredMSPF)
	{
		lastdrawtick = currentTime;
		return dtrue;
	}

	return dfalse;
}


void calcuprate()
{
	static unsigned __int64 frametime = 0;	
	static int framecounter = 0;
	static unsigned __int64 lasttime;

	unsigned __int64 currtime = getticks();

	g_upiv = (double)(currtime - frametime) / 1000.0f;

	frametime = currtime;

	++framecounter;

	if( currtime - lasttime > 1000 )
	{
		g_inupfps = framecounter;

		lasttime = currtime;

		framecounter = 0;
	}
}

dbool upnext()
{
	static unsigned __int64 lastupdtick;
	static unsigned __int64 elapsedupdtime;

	unsigned __int64 currentTime = getticks();
	int desiredMSPF = 1000/SIM_FRAME_RATE;
	unsigned __int64 deltaTime = (currentTime - lastupdtick);

	if(deltaTime >= desiredMSPF)
	{
		lastupdtick = currentTime;
		return dtrue;
	}

	return dfalse;
}

#ifndef MATCHMAKER

dbool initwin()
{
	char path[DMD_MAX_PATH+1];
	texdata* pixels;
	SDL_Surface* surf;

//#ifndef PLAT_MOBILE
#if 000	//icon
	fullpath("gui/i-64x64.png", path);
	pixels = loadpng(path);

	if(!pixels)
	{
		errm("Error", "Couldn't load icon");
	}

	surf = SDL_CreateRGBSurfaceFrom(pixels->data, pixels->sizex, pixels->sizey, pixels->channels*8, pixels->channels*pixels->sizex, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

	if(!surf)
	{
		char message[256];
		sprintf(message, "Couldn't create icon: %s", SDL_GetError());
		errm("Error", message);
	}

	SDL_SetWindowIcon(g_win, surf);

	ltexfree(pixels);
	free(pixels);
#endif

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
#ifdef PLAT_GL14
	glClearDepth(1.0f);
#endif
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
#ifdef PLAT_GL14
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	
	inglsl();

	return dtrue;
}

SDL_bool isfs(SDL_Window *win)
{
	unsigned int flags = SDL_GetWindowFlags(win);

	if (flags & SDL_WINDOW_FULLSCREEN) return SDL_TRUE;

	return SDL_FALSE;
}

int togglefs(SDL_Window *win)
{
	if (isfs(win))
	{
		if (SDL_SetWindowFullscreen(win, SDL_FALSE) < 0)
		{
			fprintf(g_applog, "Setting windowed failed : %s\r\n", SDL_GetError());
			return -1;
		}

		return 0;
	}

	if (SDL_SetWindowFullscreen(win, SDL_TRUE) < 0)
	{
		fprintf(g_applog, "Setting fullscreen failed : %s\r\n", SDL_GetError());
		return -1;
	}

	return 1; 
}

void breakwin(const char* title)
{
	char msg[256];
	unsigned int flags;
	glshader *s;

	freetexs();

	for(s=g_shader; s<g_shader+SHADERS; ++s)
		shfree(s);

	flags = (SDL_GetWindowFlags(g_win) & ~SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_SetWindowFullscreen(g_win, flags);

	SDL_GL_DeleteContext(g_gx);

	SDL_DestroyWindow(g_win);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);  /* exit black fullscreen on mac */

	if(SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		sprintf(msg, "SDL_Init: %s\n", SDL_GetError());
		errm("Error", msg);
	}

	SDL_ShowCursor(SDL_FALSE);
}

dbool makewin(const char* title)
{
	char msg[256];
	unsigned int flags;
	int startx;
	int starty;
	int winszx;
	int winszy;

#if 1
#ifdef PLAT_GL14
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#endif

	if(g_fs)
	{
		startx = SDL_WINDOWPOS_UNDEFINED;
		starty = SDL_WINDOWPOS_UNDEFINED;
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
	}
	else
	{
		startx = SDL_WINDOWPOS_UNDEFINED;
		starty = SDL_WINDOWPOS_UNDEFINED;
		flags = SDL_WINDOW_OPENGL;
	}

	g_win = SDL_CreateWindow(
		title,   
		startx,    
		starty,   
		g_selres.x,    
		g_selres.y,  
		flags       
		);

	if (g_win == NULL)
	{
		sprintf(msg, "Could not create window: %s\n", SDL_GetError());
		errm("Error", msg);
		return dfalse;
	}

	g_gx = SDL_GL_CreateContext(g_win);

	if(!g_gx)
	{
		breakwin(title);
		errm("Error", "Couldn't create GL context");
		return dfalse;
	}

	SDL_GL_MakeCurrent(g_win, g_gx);

	SDL_GL_SetSwapInterval(0);

	SDL_GetWindowSize(g_win, &winszx, &winszy);
	resize(winszx, winszy);

	if(!initwin())
	{
		breakwin(title);
		errm("Error", "Initialization failed");
		return dfalse;
	}

	cenmouse();

	return dtrue;
}

#endif
