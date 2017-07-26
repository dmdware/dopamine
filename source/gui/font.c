#include "../sys/inc.h"
#include "font.h"
#include "../sys/texture.h"
#include "../render/shader.h"
#include "../sys/syswin.h"
#include "../sys/utils.h"
#include "gui.h"
#include "icon.h"
#include "richtext.h"
#include "../algo/bool.h"
#include "../algo/list.h"

font g_font[FONTS];

static short gx;
static short gy;
static int nextlb;  //next [i] to skip line
static int lastspace;
static int j;
static short x0;
static int i;
static short x;
static short y;
static int w;
static int h;
static int line;
//static char *g_str;
static char *g_rtext;
static int g_currfont;
static int startline;
static int starti;
static short gstartx;
static short goffstartx;
static float frame[4];
static float crop[4];
static int glyphi;	//RichText part's [i] index
static unsigned int k;
static float currcolor[4];
static dbool debugtest = dfalse;

void glyph_init(glyph *g)
{
	g->pixel[0] = 0;
	g->pixel[1] = 0;
	g->texsize[0] = 0;
	g->texsize[1] = 0;
	g->offset[0] = 0;
	g->offset[1] = 0;
	g->origsize[0] = 0;
	g->origsize[1] = 0;
	g->texcoord[0] = 0;
	g->texcoord[1] = 0;
	g->texcoord[2] = 0;
	g->texcoord[3] = 0;
}

void Font_init(font *f)
{
	f->width = 0;
	f->height = 0;
	f->gheight = 0;
}

void newl()
{
	font *f;
	
	f = &g_font[g_currfont];
	
	++line;
	x = goffstartx;
	y += (short)f->gheight;
}

//"prepare break" - get the [i] of the next new line
void preb()
{
	font *f;
	glyph *g;
	int lastspace, adv;
	short x0, iconi;
	icot *icon;
	float hscale;
	
	f = &g_font[g_currfont];
	
	lastspace = -1;
	x0 = gstartx;

	for(j=i; g_rtext[j]; j+=adv)
	{
		adv = 0;
		k = toglyph(&g_rtext[j], &adv);
		
		if(k <= RICH_GLYPH_END)
		{
			if(k == '\n')
			{
				nextlb = j+1;
				return;
			}

			g = &f->glyph[k];
			x0 += g->origsize[0];

			if(k == ' ' || k == '\t')
				lastspace = j;
		}
		else if(k >= RICH_ICON_START &&
				k <= RICH_ICON_END)
		{
			iconi = k - RICH_ICON_START;
			icon = &g_icon[iconi];
			hscale = f->gheight / (float)icon->h;
			x0 += (short)((float)icon->w * hscale);
		}

		if(x0 > w+gstartx)
		{
			if(lastspace < 0)
			{
				nextlb = maxi(j, i+1);
				return;
			}

			nextlb = lastspace+1;
			return;
		}
	}
}

//"next break" - put us on a new line, get the next [i] to break, 
//unless there is no line break (at i=0, before there are any characters)
void neb()
{
	font *f;
	
	f = &g_font[g_currfont];

	if(nextlb != starti)
		newl();

	preb();
}

void drawg()
{
	font *f;
	icot *icon;
	float hscale;
	short left, right, top, bottom, iconi;
	glshader *s;
	glyph *g;
	
	s = &g_shader[g_cursh];
	f = &g_font[g_currfont];
	k = toglyph(&g_rtext[i], NULL);

	if(k >= RICH_ICON_START &&
	   k <= RICH_ICON_END)
	{
		iconi = k - RICH_ICON_START;
		icon = &g_icon[iconi];
		hscale = f->gheight / (float)icon->h;

		useicon(iconi);
		glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);

		left = x;
		right = (short)( left + (float)icon->w * hscale );
		top = y;
		bottom = top + (short)f->gheight;
		drawg2((float)left, (float)top, (float)right, (float)bottom, 0, 0, 1, 1);

		usefont();
		glUniform4f(s->slot[SSLOT_COLOR], currcolor[0], currcolor[1], currcolor[2], currcolor[3]);
	}
	else if(k <= RICH_GLYPH_END)
	{
		g = &f->glyph[k];

		left = x + g->offset[0];
		right = left + g->texsize[0];
		top = y + g->offset[1];
		bottom = top + g->texsize[1];

		drawg2((float)left, (float)top, (float)right, (float)bottom, g->texcoord[0], g->texcoord[1], g->texcoord[2], g->texcoord[3]);
	}
}

void highg()
{
	font *f;
	icot *icon;
	float hscale;
	short left, right, top, bottom, iconi;
	glshader *s;
	glyph *g;
	
	s = &g_shader[g_cursh];
	f = &g_font[g_currfont];
	k = toglyph(&g_rtext[i], NULL);
	
	if(k >= RICH_ICON_START &&
	   k <= RICH_ICON_END)
	{
		iconi = k - RICH_ICON_START;
		icon = &g_icon[iconi];
		hscale = f->gheight / (float)icon->h;

		useicon(iconi);

		left = x;
		right = (short)( left + (float)icon->w * hscale );
		top = y;
		bottom = top + (short)f->gheight;
		highg2((float)left, (float)top, (float)right, (float)bottom);

		usefont();
	}
	else if(k <= RICH_GLYPH_END)
	{
		g = &f->glyph[k];

		left = x + g->offset[0];
		right = left + g->texsize[0];
		top = y + g->offset[1];
		bottom = top + g->texsize[1];
		highg2((float)left, (float)top, (float)right, (float)bottom);
	}
}

void drawcar()
{
	font *f;
	glyph *g;
	short left, right, top, bottom;
	
	f = &g_font[g_currfont];
	g = &f->glyph['|'];

	left = x - g->origsize[1]/14;
	right = left + g->texsize[0];
	top = y + g->offset[1];
	bottom = top + g->texsize[1];
	drawg((float)left, (float)top, (float)right, (float)bottom, g->texcoord[0], g->texcoord[1], g->texcoord[2], g->texcoord[3]);
}

void advg()
{
	font *f;
	glyph *g;
	short iconi;
	icot *icon;
	float hscale;
	
	f = &g_font[g_currfont];
	k = toglyph(&g_rtext[i], NULL);
	
	if(k >= RICH_ICON_START &&
	   k <= RICH_ICON_END)
	{
		iconi = k - RICH_ICON_START;
		icon = &g_icon[iconi];
		hscale = f->gheight / (float)icon->h;
		x += (short)((float)icon->w * hscale);
		
		++glyphi;
	}
	else if(k <= RICH_GLYPH_END)
	{
		g = &f->glyph[k];
		x += g->origsize[0];

		++glyphi;
	}
}

void usefont()
{
	glshader *s;
	s = &g_shader[g_cursh];
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_tex[ g_font[g_currfont].texin ].texname);
	glUniform1i(s->slot[SSLOT_TEXTURE0], 0);
}

void useicon(int icon)
{
	glshader *s;
	s = &g_shader[g_cursh];
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_tex[ g_icon[icon].texi ].texname);
	glUniform1i(s->slot[SSLOT_TEXTURE0], 0);
}

void startt(const char *text, int fnt, int ln, int realstartx, float *inframe, float *incrop)
{
	frame[0] = inframe[0];
	frame[1] = inframe[1];
	frame[2] = inframe[2];
	frame[3] = inframe[3];
	crop[0] = incrop[0];
	crop[1] = incrop[1];
	crop[2] = incrop[2];
	crop[3] = incrop[3];
	g_currfont = fnt;
	g_rtext = (char*)text;
	w = (int)(frame[2]-frame[0]);
	h = (int)(frame[3]-frame[1]);
	startline = ln;
	starti = 0;
	gstartx = realstartx;
}

void textlayer(int offstartx, int offstarty)
{
	int adv;
	
	x = offstartx;
	y = offstarty;
	goffstartx = offstartx;
	line = 0;
	i = starti;
	glyphi = 0;
	nextlb = -1;  //next [i] to break line
	preb();

	for(; g_rtext[i] && line<startline; i+=adv)
	{
		if(i == nextlb)
			neb();
		adv = 0;
		toglyph(&g_rtext[i], &adv);
		advg();
	}

	for(; g_rtext[i] && i<starti; i+=adv)
	{
		if(i == nextlb)
			neb();
		adv = 0;
		toglyph(&g_rtext[i], &adv);
		advg();
	}

	x = offstartx;
	y = offstarty;
	goffstartx = offstartx;
}

void loadfont(int i, char *fontfile)
{
	font *f;
	glyph *g;
	char extfile[128], fullfont[DMD_MAX_PATH+1];
	FILE *fp;
	unsigned int n;
	short pixel[2], texsize[2], offset[2], origsize[2];
	
	f = &g_font[i];
	
	Font_init(f);
	
	for(i=0; i<CODE_POINTS; ++i)
	{
		g = &f->glyph[i];
		glyph_init(g);
	}
	
	strcpy(extfile, fontfile);
	findtexext(extfile);

	createtex(&f->texin, extfile, dtrue, dfalse, dfalse);
	f->width = (float)g_tex[f->texin].width;
	f->height = (float)g_tex[f->texin].height;

	extfile[0] = 0;
	strcat(extfile, fontfile);
	strcat(extfile, ".fnt");
	
	fullpath(extfile, fullfont);
	fp = fopen(fullfont, "rb");
	if(!fp)
	{
		fprintf(g_applog, "Error loading font %s\r\n", fontfile);
		fprintf(g_applog, "Full path: %s", fullfont);
		return;
	}

	//skip 2 lines
	do
	{
		i = fgetc(fp);
	}while(i!='\n' && i!=EOF);
	do
	{
		i = fgetc(fp);
	}while(i!='\n' && i!=EOF);

	do
	{
		fscanf(fp, "%d %hu %hu %hu %hu %hu %hu %hu %hu\r\n",
			   &n,
			   &pixel[0], &pixel[1],
			   &texsize[0], &texsize[1],
			   &offset[0], &offset[1],
			   &origsize[0], &origsize[1]);
		
		g = &f->glyph[n];
		g->pixel[0] = pixel[0];
		g->pixel[1] = pixel[1];
		g->texsize[0] = texsize[0];
		g->texsize[1] = texsize[1];
		g->offset[0] = offset[0];
		g->offset[1] = offset[1];
		g->origsize[0] = origsize[0];
		g->origsize[1] = origsize[1];
		g->texcoord[0] = (float)g->pixel[0] / f->width;
		g->texcoord[1] = (float)g->pixel[1] / f->height;
		g->texcoord[2] = (float)(g->pixel[0]+g->texsize[0]) / f->width;
		g->texcoord[3] = (float)(g->pixel[1]+g->texsize[1]) / f->height;
	}while(!feof(fp));

	f->gheight = (float)f->glyph['A'].origsize[1] + 1;
	
	fclose(fp);
	fprintf(g_applog, "%s.fnt\r\n", fontfile);
}

void drawg2(float left, float top, float right, float bottom, float texleft, float textop, float texright, float texbottom)
{
	float vertices[4*6], newleft, newtop, newright, newbottom, newtexleft, newtextop, newtexright, newtexbottom;
	glshader *s;
	
	newleft = left;
	newtop = top;
	newright = right;
	newbottom = bottom;
	newtexleft = texleft;
	newtextop = textop;
	newtexright = texright;
	newtexbottom = texbottom;

	if(newleft < crop[0])
	{
		newtexleft = texleft+(crop[0]-newleft)*(texright-texleft)/(right-left);
		newleft = crop[0];
	}
	else if(newleft > crop[2])
		return;

	if(newright < crop[0])
		return;
	else if(newright > crop[2])
	{
		newtexright = texleft+(crop[2]-newleft)*(texright-texleft)/(right-left);
		newright = crop[2];
	}

	if(newtop < crop[1])
	{
		newtextop = textop+(crop[1]-newtop)*(texbottom-textop)/(bottom-top);
		newtop = crop[1];
	}
	else if(newtop > crop[3])
		return;

	if(newbottom < crop[1])
		return;
	else if(newbottom > crop[3])
	{
		newtexbottom = textop+(crop[3]-newtop)*(texbottom-textop)/(bottom-top);
		newbottom = crop[3];
	}

	vertices[0] = newleft;
	vertices[1] = newtop;
	vertices[2] = newtexleft;
	vertices[3] = newtextop;
	vertices[4] = newright;
	vertices[5] = newtop;
	vertices[6] = newtexright;
	vertices[7] = newtextop;
	vertices[8] = newright;
	vertices[9] = newbottom;
	vertices[10] = newtexright;
	vertices[11] = newtexbottom;
	vertices[12] = newright;
	vertices[13] = newbottom;
	vertices[14] = newtexright;
	vertices[15] = newtexbottom;
	vertices[16] = newleft;
	vertices[17] = newbottom;
	vertices[18] = newtexleft;
	vertices[19] = newtexbottom;
	vertices[20] = newleft;
	vertices[21] = newtop;
	vertices[22] = newtexleft;
	vertices[23] = newtextop;

    s = &g_shader[g_cursh];

	glVertexPointer(2, GL_FLOAT, sizeof(float)*4, vertices);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float)*4, vertices+2);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void highg2(float left, float top, float right, float bottom)
{
	float vertices[2*6], newleft, newtop, newright, newbottom;
	glshader *s;
	
	newleft = left;
	newtop = top;
	newright = right;
	newbottom = bottom;

	if(newleft < crop[0])
		newleft = crop[0];
	else if(newleft > crop[2])
		newleft = crop[2];

	if(newright < crop[0])
		newright = crop[0];
	else if(newright > crop[2])
		newright = crop[2];

	if(newtop < crop[1])
		newtop = crop[1];
	else if(newtop > crop[3])
		newtop = crop[3];

	if(newbottom < crop[1])
		newbottom = crop[1];
	else if(newbottom > crop[3])
		newbottom = crop[3];

	vertices[0] = newleft;
	vertices[1] = newtop;
	vertices[2] = newright;
	vertices[3] = newtop;
	vertices[4] = newright;
	vertices[5] = newbottom;
	vertices[6] = newright;
	vertices[7] = newbottom;
	vertices[8] = newleft;
	vertices[9] = newbottom;
	vertices[10] = newleft;
	vertices[11] = newtop;
    
    s = &g_shader[g_cursh];
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawtl2(int caret)
{
	int adv;
	
	if(caret == 0)
		drawcar();
	for(; g_rtext[i]; i+=adv)
	{
		if(caret == i)
			drawcar();
		drawg();
		adv = 0;
		toglyph(&g_rtext[i], &adv);
		advg();
	}
	if(caret == i)
		drawcar();
}

void drawtl(int fnt, float *inframe, float *incrop,  char *text, float *color, int caret)
{
	unsigned char c;
	glshader *s;
	
	s = &g_shader[g_cursh];
	
	if(color == NULL)
	{
		glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);
		for(c=0; c<4; c++) currcolor[c] = 1;
	}
	else
	{
		glUniform4f(s->slot[SSLOT_COLOR], color[0], color[1], color[2], color[3]);
		for(c=0; c<4; c++) currcolor[c] = color[c];
	}

	startt(text, fnt, 0, (int)inframe[0], inframe, incrop);
	usefont();
	textlayer((int)inframe[0], (int)inframe[1]);
	drawtl2(caret);
}

//"draw shadow text"
void drawst(int fnt, float *inframe, float *incrop, char *text, const float *color, int caret)
{
	unsigned char c;
	glshader *s;
	
	s = &g_shader[g_cursh];
	
	currcolor[0] = 0;
	currcolor[1] = 0;
	currcolor[2] = 0;
	currcolor[3] = color != NULL ? color[3] : 1;
	glUniform4f(s->slot[SSLOT_COLOR], currcolor[0], currcolor[1], currcolor[2], currcolor[3]);

	startt(text, fnt, 0, (int)inframe[0], inframe, incrop);

	usefont();
	textlayer((int)inframe[0]+1, (int)inframe[1]);

	drawtl2(caret);
	textlayer((int)inframe[0], (int)inframe[1]+1);
	drawtl2(caret);
	textlayer((int)inframe[0]+1, (int)inframe[1]+1);
	drawtl2(caret);

	if(color == NULL)
	{
		glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);
		for(c=0; c<4; c++) currcolor[c] = 1;
	}
	else
	{
		glUniform4f(s->slot[SSLOT_COLOR], color[0], color[1], color[2], color[3]);
		for(c=0; c<4; c++) currcolor[c] = color[c];
	}

	textlayer((int)inframe[0], (int)inframe[1]);
	drawtl2(caret);
	glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);
}

void high(int fnt, float *inframe, float *incrop, char *text, int highlstarti, int highlendi, dbool multiline)
{
	glshader *s;
	int adv;
	
	endsh();
	usesh(SH_COLOR2D);
	s = &g_shader[g_cursh];
	glUniform1f(s->slot[SSLOT_WIDTH], (float)g_currw);
	glUniform1f(s->slot[SSLOT_HEIGHT], (float)g_currh);
	glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 0.5f);

	currcolor[0] = 1;
	currcolor[1] = 1;
	currcolor[2] = 1;
	currcolor[3] = 0.5f;

	startt(text, fnt, 0, (int)inframe[0], inframe, incrop);
	textlayer((int)inframe[0], (int)inframe[1]);

	for(i=0; i<highlstarti; i+=adv)
	{
		if(multiline && i == nextlb)
			neb();
		adv = 0;
		toglyph(&g_rtext[i], &adv);
		advg();
	}
	for(; i<highlendi; i+=adv)
	{
		if(multiline && i == nextlb)
			neb();
		highg();
		adv = 0;
		toglyph(&g_rtext[i], &adv);
		advg();
	}

	endsh();
	flatview(g_currw, g_currh, 1, 1, 1, 1);
}

//"draw center shadow text"
void drawct(int fnt, float *inframe, float *incrop, char *text, float *color, int caret)
{
	float linew, hscale, a;
	int len, adv;
	font *f;
	short iconi;
	icot *icon;
	glshader *s;
	unsigned char c;
 
	s = &g_shader[g_cursh];
	linew = 0;
	len = richlen(text);
	f = &g_font[fnt];
	
	for(i=0; text[i]; i+=adv)
	{
		adv = 0;
		k = toglyph(&text[i], &adv);
		if(k <= RICH_GLYPH_END)
		{
			linew += f->glyph[k].origsize[0];
		}
		else if(k >= RICH_ICON_START &&
				k <= RICH_ICON_END)
		{
			iconi = k - RICH_ICON_START;
			icon = &g_icon[iconi];
			hscale = f->gheight / (float)icon->h;
			linew += (float)icon->w * hscale;
		}
	}

	gstartx -= (short)linew/2;

	a = 1;
	if(color != NULL)
		a = color[3];
	
	currcolor[0] = 0.0f;
	currcolor[1] = 0.0f;
	currcolor[2] = 0.0f;
	currcolor[3] = color != NULL ? color[3] : 1;
	glUniform4f(s->slot[SSLOT_COLOR], currcolor[0], currcolor[1], currcolor[2], currcolor[3]);

	startt(text, fnt, 0, (int)inframe[0], inframe, incrop);
	usefont();
	textlayer((int)inframe[0]+1, (int)inframe[1]);
	drawtl2(caret);
	textlayer((int)inframe[0], (int)inframe[1]+1);
	drawtl2(caret);
	textlayer((int)inframe[0]+1, (int)inframe[1]+1);
	drawtl2(caret);

	if(color == NULL)
	{
		glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);
		for(c=0; c<4; c++) currcolor[c] = 1;
	}
	else
	{
		glUniform4f(s->slot[SSLOT_COLOR], color[0], color[1], color[2], color[3]);
		for(c=0; c<4; c++) currcolor[c] = color[c];
	}

	textlayer((int)inframe[0], (int)inframe[1]);
	for(; g_rtext[i]; i+=adv)
	{
		drawg();
		adv = 0;
		toglyph(&g_rtext[i], &adv);
		advg();
	}

	glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);
}

void drawt(int fnt, float *inframe, float *incrop, char *text, float *color, int ln, int caret, dbool shadow, dbool multiline)
{
	glshader *s;
	unsigned char c;
	int adv;
	
	s = &g_shader[g_cursh];
	
	glUniform4f(s->slot[SSLOT_COLOR], 0.3f, 0.3f, 0.3f, color ? color[3] : 1);
	currcolor[0] = 0.3f;
	currcolor[1] = 0.3f;
	currcolor[2] = 0.3f;
	currcolor[3] = color != NULL ? color[3] : 1;

	startt(text, fnt, ln, (int)inframe[0], inframe, incrop);
	usefont();
	
	if(shadow)
	{
		textlayer((int)inframe[0]+1, (int)inframe[1]);
		if(caret == 0)
			drawcar();
		for(; g_rtext[i]; i+=adv)
		{
			if(multiline && i == nextlb)
				neb();
			
			if(caret == i)
				drawcar();
			
			drawg();
			
			adv = 0;
			toglyph(&g_rtext[i], &adv);
			
			advg();
		}
		if(caret == i)
			drawcar();
		
		textlayer((int)inframe[0], (int)inframe[1]+1);
		if(caret == 0)
			drawcar();
		for(; g_rtext[i]; i+=adv)
		{
			if(multiline && i == nextlb)
				neb();
			
			if(caret == i)
				drawcar();
			
			drawg();
			
			adv = 0;
			toglyph(&g_rtext[i], &adv);
			
			advg();
		}
		if(caret == i)
			drawcar();
		
		textlayer((int)inframe[0]+1, (int)inframe[1]+1);
		if(caret == 0)
			drawcar();
		for(; g_rtext[i]; i+=adv)
		{
			if(multiline && i == nextlb)
				neb();
			
			if(caret == i)
				drawcar();
			
			drawg();
			
			adv = 0;
			toglyph(&g_rtext[i], &adv);
			
			advg();
		}
		if(caret == i)
			drawcar();
	}

	if(color == NULL)
	{
		glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);
		for(c=0; c<4; c++) currcolor[c] = 1;
	}
	else
	{
		glUniform4f(s->slot[SSLOT_COLOR], color[0], color[1], color[2], color[3]);
		for(c=0; c<4; c++) currcolor[c] = color[c];
	}

	textlayer((int)inframe[0], (int)inframe[1]);

	if(caret == 0)
		drawcar();

	for(; g_rtext[i]; i+=adv)
	{
		if(multiline && i == nextlb)
			neb();

		if(caret == i)
			drawcar();

		drawg();
		
		adv = 0;
		toglyph(&g_rtext[i], &adv);
		
		advg();
	}
	if(caret == i)
		drawcar();
}

int clines(char *text, int fnt, float *inframe)
{
	int adv;
	
	startt(text, fnt, 0, (int)inframe[0], inframe, inframe);
	textlayer((int)inframe[0], (int)inframe[1]);

	for(i=0; g_rtext[i]; i+=adv)
	{
		if(i == nextlb)
			neb();
		
		adv = 0;
		toglyph(&g_rtext[i], &adv);

		advg();
	}

	return line+1;	//add 1 since line starts count from 0
}

int linei(char *text, int fnt, float *inframe, int getline)
{
	int adv;
	
	startt(text, fnt, 0, (int)inframe[0], inframe, inframe);
	textlayer((int)inframe[0], (int)inframe[1]);

	for(i=0; g_rtext[i]; i+=adv)
	{
		if(i == nextlb)
			neb();

		if(line == getline)
			return i;
		
		adv = 0;
		toglyph(&g_rtext[i], &adv);

		advg();
	}
	
	if(line == getline)
		return i;

	return i;	//return last glyph anyway
}

int endx(const char *text, int lastg, int fnt, float *inframe, dbool multiline)
{
	int highx, adv;

	startt(text, fnt, 0, (int)inframe[0], inframe, inframe);
	textlayer((int)inframe[0], (int)inframe[1]);

	highx = (int)gstartx;

	for(i=0; g_rtext[i] && glyphi<lastg; i+=adv)
	{
		if(multiline && i == nextlb)
			neb();
		
		adv = 0;
		toglyph(&g_rtext[i], &adv);

		advg();

		if(x > highx)
			highx = x;
	}

	return highx;
}

int matg(char *text, int fnt, float *inframe, int matchx, int matchy, dbool multiline)
{
	int lastclose, lastx, lasty, adv;
	font *f;
	float gw;
	
	lastclose = 0;
	f = &g_font[fnt];

	startt(text, fnt, 0, (int)inframe[0], inframe, inframe);
	textlayer((int)inframe[0], (int)inframe[1]);

	if(x >= matchx ||
	   y >= matchy ||
	   //size <= 0
	   !g_rtext[i])
		return lastclose;

	lastx = x;
	lasty = y;

	for(i=0; g_rtext[i]; i+=adv)
	{
		if(multiline && i == nextlb)
			neb();
		
		adv = 0;
		k = toglyph(&g_rtext[i], &adv);
		
		if(k <= RICH_GLYPH_END)
		{
			gw = f->glyph[k].origsize[0];
			
			if(matchx >= x+gw/2.0f)
			{
				if(!multiline)
					goto match;
				else if(matchy >= y &&
						matchy <= y+f->gheight)
					goto match;
			}
			
			continue;
			
		match:
			lastclose = i;
			lastx = x;
			lasty = y;
		}
		
		advg();
		
		if((float)x >= matchx &&
		   (float)y >= matchy)
			return lastclose;
	}

	return lastclose;
}

int textw(int fnt, float *inframe, const char *text)
{
	return endx(text, richlen(text), fnt, inframe, dfalse) - (int)inframe[0];
}

void loadfonts()
{
#if 0
	loadfont(FONT_EUROSTILE32, "fonts/eurostile32");
	loadfont(FONT_MSUIGOTHIC16, "fonts/msuigothic16");
	loadfont(FONT_MSUIGOTHIC10, "fonts/msuigothic10");
	loadfont(FONT_SMALLFONTS8, "fonts/smallfonts8");
	loadfont(FONT_SMALLFONTS10, "fonts/smallfonts10");
	loadfont(FONT_ARIAL10, "fonts/arial10s");
	loadfont(FONT_GULIM32, "fonts/gulim32");
	loadfont(FONT_EUROSTILE16, "fonts/eurostile16");
#endif
	//loadfont(FONT_CALIBRILIGHT16, "fonts/gulim16");
	loadfont(FONT_TERMINAL10, "fonts/terminal10");
	loadfont(FONT_CALIBRILIGHT16, "fonts/calibrilight16s-full");
}
