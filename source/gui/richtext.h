

#ifndef RICHTEXT_H
#define RICHTEXT_H

#include "../sys/inc.h"
#include "font.h"
#include "../utf8/utf8.h"

//types of blocks
//max UTF32 = 0x7fffffff (INT_MAX)
#define RICH_GLYPH_START	0
#define RICH_GLYPH_END		(CODE_POINTS-1)
#define RICH_END			(0x7fffffff)
#define RICH_ICON_END		(RICH_END-1)
#define RICH_ICON_START		(RICH_ICON_END-ICONS)
#define RICH_COLOR			(RICH_ICON_START-1)
#define RICH_FONT			(RICH_COLOR-1)

//NOTE: color,font blocks may never be added because that complicates copying,psubstr,rawstr,parsetags too much

void pwver(char **ndest, char *src);	//asterisk-mask password string
void parsetags(char **ndest, char *src, int *caret);
int richlen(const char *in);

//TODO absorb
char *fromglyph(const unsigned int in, int *adv);
unsigned int toglyph(const char *in, int *index);

void pstrset(char **out, const char *in);
void pstradd(char **out, const char *in);
void psubstr(char **out, const char *in, int beg, int len);
void delprev(char **s, int *caret);
void delnext(char **s, int *caret);
int prevlen(char *s, int caret);
int nextlen(char *s, int caret);

#endif
