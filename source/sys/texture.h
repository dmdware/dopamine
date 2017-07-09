











#ifndef TEXTURE_H
#define TEXTURE_H

#include "inc.h"
#include "../algo/vec.h"
#include "../algo/bool.h"
#include "../math/v2i.h"

struct gltex
{
	unsigned int texname;
	char* full;
	dbool loaded;
	int width, height;
	dbool transp;
	dbool sky;
	dbool breakable;
	dbool passthru;
	dbool grate;
	dbool ladder;
	dbool water;
	dbool fabric;
	dbool clamp;
	dbool mipmaps;
};

typedef struct gltex gltex;

#define TEXTURES	40960
extern gltex g_tex[TEXTURES];

#define TGA_RGB		 2		// This tells us it's a normal RGB (really BGR) file
#define TGA_A		 3		// This tells us it's an ALPHA file
#define TGA_RLE		10		// This tells us that the targa is Run-Length Encoded (RLE)

#ifndef int_p_NULL
#define int_p_NULL (int*)NULL
#endif
#define png_infopp_NULL (png_infopp)NULL
#define png_voidp_NULL	(png_voidp)NULL

#define JPEG_BUFFER_SIZE (8 << 10)

struct texdata
{
	int channels;			// The channels in the image (3 = RGB : 4 = RGBA)
	int sizex;				// The width of the image in pixels
	int sizey;				// The height of the image in pixels
	unsigned char *data;	// The image pixel data
};

typedef struct texdata texdata;

void ltexinit(texdata *lt);
void ltexfree(texdata *lt);

struct textoload
{
	unsigned int* ptexin;
	unsigned int texin;
	char relative[DMD_MAX_PATH+1];	//TODO char*
	dbool clamp;
	dbool reload;
	dbool mipmaps;
};

typedef struct textoload textoload;

extern vector g_texload;

extern int g_lastloadtex;

void texinit(gltex *tex);
void texfree(gltex *tex);

texdata *loadjpg(const char *full);
texdata *loadpng(const char *full);
dbool findtex(unsigned int *texture, const char* relative);
int newtex();
dbool regtex(unsigned int texture, 
				 const char* relative, 
				 dbool transp, 
				 dbool clamp, 
				 dbool mipmaps, 
				 unsigned int* texin,
				 dbool reload,
				 int sizex, int sizey);
void findtexext(char *relative);
void freetexs();
dbool loadqtex();
void queuetex(unsigned int* texin, const char* relative, dbool clamp, dbool mipmaps);
void reloadtex(unsigned int texin, const char* relative, dbool clamp, dbool mipmaps);
texdata* loadtex(const char* full);
dbool createtex2(texdata* pim, unsigned int* texname, dbool clamp, dbool mipmaps);
dbool createtex(unsigned int *texin, const char* relative, dbool clamp, dbool mipmaps, dbool reload);
void reloadtexs();
void freetex(int i);
void diffpathj(const char* basepath, char* diffpath);
void diffpathp(const char* basepath, char* diffpath);
void specpathj(const char* basepath, char* specpath);
void normpathj(const char* basepath, char* normpath);
void ownpathp(const char* basepath, char* ownpath);
void alloctex(texdata* empty, int width, int height, int channels);
void blittex(texdata* src, texdata* dest, v2i pos);
dbool savejpg(const char* full, texdata* image, float quality);
dbool savepng(const char* full, texdata* image);
void fliptex(texdata* image);
dbool saveraw(const char* full, texdata* image);
void resample(texdata* original, texdata* empty, v2i newdim);
void extract(texdata* original, texdata* empty, int x1, int y1, int x2, int y2);


#endif
