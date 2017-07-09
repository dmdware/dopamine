





#ifndef INCLUDES_H
#define INCLUDES_H

#include "plat.h"

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#ifdef PLAT_WIN
#include <winsock2.h>	// winsock2 needs to be included before windows.h
#include <windows.h>
#include <mmsystem.h>
#include <commdlg.h>
#include "../dirent/dirent.h"
#include <WinDef.h>
//#include "../../../libs/win/dirent-1.20.1/include/dirent.h"
#include <assert.h>
#endif

#ifdef PLAT_LINUX
/* POSIX! getpid(), readlink() */
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
//file listing dirent
#include <dirent.h>
#include <gtk/gtk.h>
//htonl
#include <arpa/inet.h>
#include <sys/time.h>
#endif

#if defined(PLAT_MAC) && !defined(PLAT_IOS)
#include <sys/types.h>
#include <sys/dir.h>
//htonl
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#endif

#if defined(PLAT_IOS)
#include <sys/types.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>	//mkdir
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#ifndef PLAT_WIN
//already included in sdl2
#include <stdint.h>
#endif
#include <limits.h>
#include <string.h>

#ifdef PLAT_WIN
#include "../jpeg/jpeglib.h"
#include "../png/png.h"
//#include <zip.h>
#endif

#ifdef PLAT_LINUX
#include <jpeglib.h>
#include <png.h>
#endif
//#define NO_SDL_GLEXT

#ifdef PLAT_MAC

#if 0
// https://trac.macports.org/ticket/42710
#ifndef FALSE            /* in case these macros already exist */
#define FALSE   0        /* values of boolean */
#endif
#ifndef TRUE
#define TRUE    1
#endif
#define HAVE_BOOLEAN

#endif

#ifdef PLAT_IOS
/*
 Use User Header Search Paths !
(Or else jpeglib.h from system folders will be used, version mismatch)
*/
#include "../jpeglib.h"
#include "../png.h"

#else

#include <jpeglib.h>
#include <png.h>
//#include <zip.h>
#endif

#endif

#ifndef MATCHMAKER
#ifdef PLAT_WIN
#include "../glew/GL/glew.h"
#endif
#endif

#ifndef MATCHMAKER
#ifdef PLAT_LINUX
//#include <GL/xglew.h>
#include <GL/glew.h>
#endif
#endif

//#define GL_GLEXT_PROTOTYPES

#if 1

#ifdef PLAT_LINUX
#include <SDL2/SDL.h>
#ifndef MATCHMAKER
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
//#include <GL/glut.h>
#include <SDL_ttf.h>
#endif
#include <SDL2/SDL_net.h>
#endif

#if defined(PLAT_MAC) && !defined(PLAT_IOS)
#ifndef MATCHMAKER
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#endif

#ifdef PLAT_IOS
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#endif

#if defined(PLAT_MAC) && !defined(PLAT_IOS)
//#include <GL/xglew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
//#include <SDL2/SDL_net.h>
//#include <SDL2/SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_syswm.h>
#endif

#if defined(PLAT_IOS)
//#include <GL/xglew.h>
#include "SDL.h"
#include "SDL_opengles2.h"
//#include <SDL2/SDL_net.h>
//#include <SDL2/SDL_mixer.h>
#include "SDL_net.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#import <CoreMotion/CoreMotion.h>
#endif

#ifdef PLAT_WIN
#include "../glew/GL/wglew.h"
#include "../sdl/SDL.h"
#ifndef MATCHMAKER
#include "../sdl/SDL_opengl.h"
//#include <SDL_mixer.h>
//#include <SDL_ttf.h>
#include "../sdl/SDL_syswm.h"
#endif
//#include <SDL_net.h>
#endif

#endif

#ifdef PLAT_WIN
#ifndef MATCHMAKER
#include "../gl/glaux.h"
#endif
#endif

#ifdef USESTEAM
#include <steaapi.h>
#include <isteamuserstats.h>
#include <isteamremotestorage.h>
#include <isteammatchmaking.h>
#include <steagameserver.h>
#endif

#if 000
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>      // C++ importer interface
//#include <Importer.hpp>      // C++ importer interface
//#include <scene.h>       // Output data structure
//#include <postprocess.h> // Post processing flags
//#include <assimp/DefaultLogger.h>
#endif

#include "../tommath/tommath.h"
//#include "../mpfr/mpfr_mpfr.h"
//#include "../mpc/mpc_mpc.h"

#ifdef PLAT_WIN
//#pragma comment(lib, "SDL2.lib")
//#pragma comment(lib, "SDL2main.lib")
//#pragma comment(lib, "SDL2_mixer.lib")
//#pragma comment(lib, "SDL2_net.lib")
//#pragma comment(lib, "SDL.lib")
//#pragma comment(lib, "SDLmain.lib")
//#pragma comment(lib, "lib32/assimp.lib")
//#pragma comment(lib, "assimp_release-dll_win32/assimp.lib")
//#pragma comment(lib, "assimp.lib")
//#pragma comment(lib, "assimp-vc110-mt.lib")
//#pragma comment(lib, "assimp-vc90-mt.lib")
//#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "../../../source/gl/glaux.lib")
//#pragma comment(lib, "libjpeg.lib")
//#pragma comment(lib, "libpng15.lib")
//#pragma comment(lib, "zlib.lib")
//#pragma comment(lib, "zipstatic.lib")
//#pragma comment(lib, "msvcrt.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "imm32.lib")
#ifdef USESTEAM
#pragma comment(lib, "sdkencryptedappticket.lib")
#pragma comment(lib, "steaapi.lib")
#endif
#endif

#ifndef DMD_MAX_PATH
#define DMD_MAX_PATH 300
#endif

#ifndef PLAT_WIN
#define SOCKET int
typedef unsigned char byte;
typedef unsigned int UINT;
typedef int16_t WORD;
#define _isnan isnan
#define stricmp strcasecmp
#define _stricmp strcasecmp
#define ERROR 0
#define APIENTRY
#endif

#ifdef PLAT_WIN
#define _isnan(x) (x!=x)
#endif

#define ZERO_MEM(x)					memset(x,0,sizeof(x))
typedef unsigned int uint;
#define ARRSZ(x)					(sizeof(x)/sizeof(x[0]))
#define ARRAY_SIZE_IN_ELEMENTS(x)	ARRSZ(x)

#ifdef PLAT_MAC
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#endif

#ifdef PLAT_WIN
#define stricmp _stricmp
#endif

extern SDL_Window *g_win;
extern SDL_GLContext g_gx;

#define SPECBUMPSHADOW

//#define GLDEBUG

#define CHECKGL() glerr(__FILE__,__LINE__)

#ifndef GLDEBUG
#define glerr(a,b); (void)0;
#endif


/* #define DEMO	*/	/* is this a time-restricted version? */
#define DEMOTIME		(5*60*1000)

#ifndef MATCHMAKER
#ifdef USESTEAM
#include <steam.h>
#endif
#endif

#endif
