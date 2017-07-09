#ifndef FONT_H
#define FONT_H

#include "../sys/inc.h"
#include "../algo/bool.h"

#define MAX_CHARS	256
#define CODE_POINTS	110000

struct glyph
{
	short pixel[2];
	short texsize[2];
	short offset[2];
	short origsize[2];
	float texcoord[4];
};

typedef struct glyph glyph;

struct font
{
	unsigned int texin;
	float width;    //image width
	float height;   //image height
	float gheight;  //glyph height
	glyph glyph[CODE_POINTS];
};

typedef struct font font;

#define FONT_TERMINAL10		0
#define FONT_CALIBRILIGHT16	1
#define FONTS				2
extern font g_font[FONTS];

//#define MAINFONT8 (FONT_SMALLFONTS10)
#define MAINFONT8 (FONT_TERMINAL10)
//#define MAINFONT8 (FONT_MSUIGOTHIC10)
//#define MAINFONT8 (FONT_TERMINAL10)
#define MAINFONT16 (FONT_CALIBRILIGHT16)
//#define MAINFONT16 (FONT_MSUIGOTHIC16)
//#define MAINFONT16 (FONT_EUROSTILE16)
//#define MAINFONT16 (FONT_SMALLFONTS10)
#define MAINFONT32 (FONT_TERMINAL10)
//#define MAINFONT32 (FONT_SMALLFONTS10)

void loadfonts();
void usefont();
void useicon(int icon);
void drawg2(float left, float top, float right, float bottom, float texleft, float textop, float texright, float texbottom);
void highg2(float left, float top, float right, float bottom);
void drawtl2(int caret);
void drawtl(int fnt, float *inframe, float *incrop, char *text, float *color, int caret);
void drawct(int fnt, float *inframe, float *incrop, char *text, float *color, int caret);
void drawt(int fnt, float *inframe, float *incrop, char *text, float *color, int ln, int caret, dbool shadow, dbool multiline);
int clines(char *text, int fnt, float *inframe);
int linei(char *text, int fnt, float *inframe, int getline);
int endx(const char *text, int lastg, int fnt, float *inframe, dbool multiline);
int matg(char *text, int fnt, float *inframe, int matchx, int matchy, dbool multiline);
void high(int fnt, float *inframe, float *incrop, char *text, int highlstarti, int highlendi, dbool multiline);
void neb();
void advg();
int textw(int fnt, float *inframe, const char *text);

#endif
