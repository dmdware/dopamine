#include "../wg.h"
#include "text.h"
#include "../../sim/user.h"
#include "../gui.h"
#include "../richtext.h"
#include "../../sys/debug.h"

#if 0

/*
Add parameter to edit box create user parameter pointer
For these make it point to text widget behind to text align using resize func using the user parameter 
And add user free to widgets func.  And make null for this case no allocation of pointer

TODO use orientability maps for planets or something detail surface? 
Inside-out orientability sphere for background nebulae star map?
Write exporter to model from earth 16-bit height map and high-res colour data.
*/

void ebwinit(ebw *eb, wg *parent, const char *n, const char *t, int f, void (*reframef)(wg *w), dbool pw, int maxl, void (*change3)(unsigned int key, unsigned int scancode, dbool down, int parm), void (*submitf)(), int parm, dbool multiline)
{
	wg *bw;
	bw = &eb->base;
	wginit(bw);
	bw->parent = parent;
	bw->type = WIDGET_EDITBOX;
	strcpy(bw->name, n);
	bw->font = f;
	free(bw->val);
	strset(&bw->val, t);
	bw->caret = strlen(bw->val);
	bw->opened = dfalse;
	bw->passw = pw;
	bw->maxlen = maxl;
	bw->reframef = reframef;
	bw->submitfunc = submitf;
	bw->changefunc = NULL;
	bw->changefunc2 = NULL;
	bw->changefunc3 = change3;
	bw->scroll[0] = 0;
	bw->highl[0] = 0;
	bw->highl[1] = 0;
	createtex(bw->frametex, "gui/frame.jpg", dtrue, dfalse);
	bw->param = parm;
	eb->multiline = multiline;
	wgreframe(bw);
}

char *ebwdrawvalue(ebw *eb)
{
	char *val;
	wg *bw;
	
	bw = &eb->base;

	if(!bw->passw)
		strset(&val, bw->val);
	else
		pwver(&val, bw->val);
	
	return val;
}

void ebwdraw(ebw *eb)
{
	glshader *s;
	wg *bw;
	float cp[4], tpos[4];
	char *val;
	
	bw = &eb->base;
	s = &g_shader[g_cursh];
	
	glUniform4f(s->slot[SSLOT_COLOR], 1, 1, 1, 1);

	subcrop(bw->pos, bw->crop, cp);
	//TODO make line-based
	drawim(g_texture[bw->frametex].texname, pos[0], pos[1], pos[2], pos[3], 0, 0, 1, 1, cp);

	val = ebwdrawvalue(eb);

	tpos[0] = bw->pos[0] + bw->scroll[0];
	tpos[1] = bw->pos[1];
	
	DrawText(bw->font, tpos, cp, val, NULL, 0, bw->opened ? bw->caret : -1, dtrue, eb->multiline);
	high(bw->font, tpos, cp, val, bw->highl[0], bw->highl[1], eb->multiline);
	
	free(val);
}

void ebwframeup(ebw *eb)
{
	wg *bw;
	dbool movedcar;
	font *f;
	char *val;
	int newcaret, vallen, endx;
	float tpos[4], *scroll, *pos, *highl;
	
	bw = &eb->base;
	scroll = bw->scroll;
	pos = bw->pos;
	highl = bw->highl;
	f = &g_font[bw->font];
	
	if(bw->ldown)
	{
		movedcar = dfalse;

		if(g_mouse.x >= pos[2]-5)
		{
			scroll[0] -= maxf(1, f->gheight/4.0f);

			val = ebwdrawvalue(eb);
			vallen = strlen(val);
			
			tpos[0] = pos[0] + scroll[0];
			tpos[1] = pos[1];
			endx = endx(val, vallen, bw->font, tpos, dfalse);

			if(endx < pos[2])
				scroll[0] += pos[2] - endx;

			if(scroll[0] > 0.0f)
				scroll[0] = 0.0f;

			movedcar = dtrue;
			
			free(val);
		}
		else if(g_mouse.x <= pos[0]+5)
		{
			scroll[0] += maxf(1, f->gheight/4.0f);

			if(scroll[0] > 0.0f)
				scroll[0] = 0.0f;

			movedcar = dtrue;
		}

		if(movedcar)
		{
			val = ebwdrawvalue(eb);
			tpos[0] = pos[0] + scroll[0];
			tpos[1] = pos[1];
			//TODO check if new matg drag selection scroll works
			newcaret = matg(val, bw->font, tpos, g_mouse.x, g_mouse.y, dfalse);

			if(newcaret > bw->caret)
			{
				highl[0] = bw->caret;
				highl[1] = newcaret;
			}
			else if(newcaret < bw->caret)
			{
				highl[0] = newcaret;
				highl[1] = bw->caret;
			}
			
			free(val);
		}
	}
}

void ebwin(ebw *eb, inev *ie)
{
	wg *bw;
	char *val;
	float *highl, tpos[4], *pos, *scroll;
	int newcaret, len, endx;
	
	bw = &eb->base;
	pos = bw->pos;
	scroll = bw->scroll;
	highl = bw->highl;
	val = ebwdrawvalue(eb);
	
	if(ie->type == INEV_MOUSEMOVE)
	{
		if(!ie->intercepted)
		{
			if(bw->ldown)
			{
				tpos[0] = pos[0] + scroll[0];
				tpos[1] = pos[1];
				tpos[2] = pos[2];
				tpos[3] = pos[3];
				newcaret = matg(val, bw->font, tpos, g_mouse.x, g_mouse.y, eb->multiline);

				if(newcaret > bw->caret)
				{
					highl[0] = bw->caret;
					highl[1] = newcaret;
				}
				else
				{
					highl[0] = newcaret;
					highl[1] = bw->caret;
				}

				ie->intercepted = dtrue;
				goto clean;
			}

			if(g_mouse.x >= pos[0] && g_mouse.x <= pos[2] && g_mouse.y >= pos[1] && g_mouse.y <= pos[3])
			{
				bw->over = dtrue;

				g_mouseoveraction = dtrue;

				ie->intercepted = dtrue;
				goto clean;
			}
			else
			{
				bw->over = dfalse;

				goto cleaup;
			}
		}
	}
	else if(ie->type == INEV_MOUSEDOWN && ie->key == MOUSE_LEFT)
	{
		if(bw->opened)
		{
			bw->opened = dfalse;
			highl[0] = highl[1] = 0;
		}

		if(!ie->intercepted)
		{
			if(bw->over)
			{
				bw->ldown = dtrue;
				
				tpos[0] = pos[0] + scroll[0];
				tpos[1] = pos[1];
				tpos[2] = pos[2];
				tpos[3] = pos[3];
				bw->caret = matg(val, bw->font, tpos, g_mouse.x, g_mouse.y, eb->multiline);

				highl[0] = 0;
				highl[1] = 0;

				ie->intercepted = dtrue;
				
				goto clean;
			}
		}
	}
	else if(ie->type == INEV_MOUSEUP && ie->key == MOUSE_LEFT && !ie->intercepted)
	{
		if(bw->ldown)
		{
			bw->ldown = dfalse;

			if(highl[1] > 0 && highl[0] != highl[1])
			{
				bw->caret = -1;
			}

			ie->intercepted = dtrue;
			ebwgainfocus(eb);

			goto clean;
		}

		bw->ldown = dfalse;

		if(bw->opened)
		{
			ie->intercepted = dtrue;
			goto clean;
		}
	}
	else if(ie->type == INEV_KEYDOWN && !ie->intercepted)
	{
		if(!bw->opened)
			goto clean;

		len = strlen(val);
		
		if(bw->caret > len)
			bw->caret = len;

		if(ie->key == SDLK_F1)
			goto clean;

		if(ie->key == SDLK_LEFT)
		{
			if(highl[0] > 0 && highl[0] != highl[1])
			{
				bw->caret = highl[0];
				highl[0] = highl[1] = 0;
			}
			else if(bw->caret <= 0)
			{
				ie->intercepted = dtrue;
				goto clean;
			}
			else
				bw->caret -= prevlen(val, bw->caret);

			tpos[0] = pos[0] + scroll[0];
			tpos[1] = pos[1];
			tpos[2] = pos[2];
			tpos[3] = pos[3];
			endx = endx(val, bw->caret, bw->font, tpos, eb->multiline);

			if(endx <= pos[0])
				scroll[0] += pos[0] - endx + 1;
		}
		else if(ie->key == SDLK_RIGHT)
		{
			len = strlen(val);

			if(highl[0] > 0 && highl[0] != highl[1])
			{
				bw->caret = highl[1];
				highl[0] = highl[1] = 0;
			}
			else if(bw->caret >= len)
			{
				ie->intercepted = dtrue;
				goto clean;
			}
			else
				bw->caret += nextlen(val, bw->caret);

			tpos[0] = pos[0] + scroll[0];
			tpos[1] = pos[1];
			tpos[2] = pos[2];
			tpos[3] = pos[3];
			endx = endx(val, bw->caret, bw->font, tpos, eb->multiline);
			
			if(endx >= pos[2])
				scroll[0] -= endx - pos[2] + 1;
		}
		else if(ie->key == SDLK_DELETE)
		{
			len = strlen(val);

			if((highl[1] <= 0 || highl[0] == highl[1]) && caret >= len || len <= 0)
			{
				ie->intercepted = dtrue;
				goto clean;
			}

			ebwdelnext(eb);

			if(!bw->passw)
				parsetags(&bw->val, bw->val, &bw->caret);
		}
		if(ie->key == SDLK_BACKSPACE)
		{
			len = strlen(val);

			if((highl[1] <= 0 || highl[0] == highl[1]) && len <= 0)
			{
				ie->intercepted = dtrue;
				goto clean;
			}

			ebwdelprev(eb);
			
			if(!bw->passw)
				parsetags(&bw->val, bw->val, &bw->caret);
		}
		else if(ie->key == SDLK_LSHIFT || ie->key == SDLK_RSHIFT)
		{
			ie->intercepted = dtrue;
			goto clean;
		}
		else if(ie->key == SDLK_CAPSLOCK)
		{
			ie->intercepted = dtrue;
			goto clean;
		}
		else if(ie->key == SDLK_RETURN || ie->key == SDLK_RETURN2)
		{
			ie->intercepted = dtrue;
			if(eb->multiline)
			{
				ebwplacestr(eb, "\n");
			}
			else
			{
				if(eb->submitfunc)
					eb->submitfunc();
				goto clean;
			}
		}

		if(bw->changefunc2 != NULL)
			bw->changefunc2(param);

		if(bw->changefunc3 != NULL)
			bw->changefunc3(ie->key, ie->scancode, dtrue, param);

		ie->intercepted = dtrue;
	}
	else if(ie->type == INEV_KEYUP && !ie->intercepted)
	{
		if(!bw->opened)
			goto clean;

		if(bw->changefunc3 != NULL)
			bw->changefunc3(ie->key, ie->scancode, dfalse, param);

		ie->intercepted = dtrue;
	}
	else if(ie->type == INEV_TEXTIN && !ie->intercepted)
	{
		if(!bw->opened)
			goto clean;

		ie->intercepted = dtrue;
		
		len = strlen(val);

		if(bw->caret > len)
			bw->caret = len;

		ebwplacestr(eb, ie->text);

		if(bw->changefunc != NULL)
			bw->changefunc();

		if(bw->changefunc2 != NULL)
			bw->changefunc2(param);

		if(bw->changefunc3 != NULL)
			bw->changefunc3(val[0], 0, dtrue, param);

		ie->intercepted = dtrue;
	}
	else if(ie->type == INEV_PASTE && !ie->intercepted)
	{
		if(!bw->opened)
			goto clean;

		ie->intercepted = dtrue;
		
		len = strlen(val);

		if(bw->caret > len)
			bw->caret = len;

		ebwpasteval(eb);
	}
	else if(ie->type == INEV_COPY && !ie->intercepted)
	{
		if(!bw->opened)
			goto clean;

		ie->intercepted = dtrue;
		
		len = strlen(val);

		if(bw->caret > len)
			bw->caret = len;

		ebwcopyval(eb);
	}
	else if(ie->type == INEV_SELALL && !ie->intercepted)
	{
		if(!opened)
			goto clean;

		ie->intercepted = dtrue;
		
		len = strlen(val);
		
		if(bw->caret > len)
			bw->caret = len;

		ebwselectall(eb);
	}
	
clean:
	free(val);
}

void ebwplacestr(ebw *eb, const char *str)
{
	wg *bw;
	int len, addlen, endx;
	char *add, *before, *after, *drawval;
	float *highl, tpos[4], *pos, *scroll;
	unsigned char padi;
 
	bw = &eb->base;
	highl = bw->highl;
	pos = bw->pos;
	scroll = bw->scroll;
	len = strlen(bw->val);

	if(highl[1] > 0 && highl[0] != highl[1])
	{
		len -= highl[1] - highl[0];
	}

	//corpc fix's all around texlen();

	addlen = strlen(str);
	if(addlen + len >= bw->maxlen)
		addlen = bw->maxlen - len;

	/*
	we want to make sure that the UTF8 string
	will be below maxlen, with the icon tags
	shown, NOT the final resulting RichText string.
	*/
	substr(&add, str, addlen);
	for(padi=0; padi<UTF8PAD; ++padi)
		stradd(&add, " ");
	for(padi=0; padi<UTF8PAD; ++padi)
		add[addlen+padi] = 0;

	if(highl[1] > 0 && highl[0] != highl[1])
	{
		substr(&before, bw->val, 0, (int)highl[0]);
		substr(&after, bw->val, (int)highl[0], UTF32Len(bw->val) - (int)highl[1]);
		free(bw->val);
		strset(&bw->val, before);
		stradd(&bw->val, add);
		stradd(&bw->val, after);
		free(before);
		free(after);

		bw->caret = highl[0] + addlen;
		highl[0] = highl[1] = 0;
	}
	else
	{
		if(len >= bw->maxlen)
			goto clean;
		
		substr(&before, bw->val, 0, bw->caret);
		substr(&after, bw->val, (int)highl[0], strlen(bw->val) - bw->caret);	//TODO doesn't work with passwords
		free(bw->val);
		setstr(&bw->val, before);
		addstr(&bw->val, add);
		addstr(&bw->val, after);
		free(before);
		free(after);
		caret += addlen;
	}
	
	if(!bw->passw)
	{
		parsetags(&bw->val, bw->val, &bw->caret);
	}

	drawval = ebwdrawvalue(eb);
	tpos[0] = pos[0] + scroll[0];
	tpos[1] = pos[1];
	endx = endx(drawval, bw->caret, bw->font, tpos, dfalse);
	free(drawval);
	
	if(endx >= pos[2])
		scroll[0] -= endx - pos[2] + 1;
	
clean:
	free(add);
}

void ebwchval(ebw *eb, const char *str)
{
	wg *bw;
	bw = &eb->base;
	bw->caret = 0;
	bw->highl[0] = 0;
	bw->highl[1] = strlen(bw->val);
	ebwplacestr(eb, str);	//does rawlen checks here
}

dbool ebwdelnext(ebw *eb)
{
	int len, endx, nextl;
	wg *bw;
	float *highl, *pos, tpos[4], *scroll;
	char *before, *after, *drawval;
	
	bw = &eb->base;
	highl = bw->highl;
	pos = bw->pos;
	scroll = bw->scroll;
	len = strlen(bw->val);

	if(highl[1] > 0 && highl[0] != highl[1])
	{
		substr(&before, bw->val, 0, (int)highl[0]);
		substr(&after, bw->val, (int)highl[1], len - (int)highl[1]);
		free(bw->val);
		setstr(&bw->val, before);
		stradd(&bw->val, after);
		free(before);
		free(after);

		bw->caret = highl[0];
		highl[0] = highl[1] = 0;
	}
	else if(bw->caret >= len || len <= 0)
		return dtrue;
	else
	{
		nextl = nextlen(bw->val, bw->caret);
		substr(&before, bw->val, 0, bw->caret);
		substr(&after, bw->val, bw->caret + nextl, len - (bw->caret + nextl));	//TODO check if correct
		free(bw->val);
		setstr(&bw->val, before);
		stradd(&bw->val, after);
		free(before);
		free(after);
	}

	drawval = ebwdrawvalue(eb);
	tpos[0] = pos[0] + scroll[0];
	tpos[1] = pos[1];
	tpos[2] = pos[2];
	tpos[3] = pos[3];
	endx = endx(bw->val, bw->caret, bw->font, tpos, eb->multiline);
	free(drawval);

	if(endx <= pos[0])
		scroll[0] += pos[0] - endx + 1;
	else if(endx >= pos[2])
		scroll[0] -= endx - pos[2] + 1;

	return dtrue;
}

dbool ebwdelprev(ebw *eb)
{
	int len, endx, prevl;
	wg *bw;
	float *highl, *pos, tpos[4], *scroll;
	char *before, *after, *drawval;
	
	bw = &eb->base;
	highl = bw->highl;
	pos = bw->pos;
	scroll = bw->scroll;
	len = strlen(bw->val);

	if(highl[1] > 0 && highl[0] != highl[1])
	{
		substr(&before, bw->val, 0, (int)highl[0]);
		substr(&after, bw->val, (int)highl[1], len - (int)highl[1]);
		free(bw->val);
		strset(&bw->val, before);
		stradd(&bw->val, after);
		free(before);
		free(after);
		
		bw->caret = highl[0];
		highl[0] = highl[1] = 0;
	}
	else if(caret <= 0 || len <= 0)
		return dtrue;
	else
	{
		prevl = prevlen(bw->val, bw->caret);
		substr(&before, bw->val, 0, bw->caret - prevl);
		substr(&after, bw->val, bw->caret, len - bw->caret);
		free(bw->val);
		strset(&bw->val, before);
		stradd(&bw->val, after);
		free(before);
		free(after);
		bw->caret -= prevl;
	}
	
	drawval = ebwdrawvalue(eb);
	tpos[0] = pos[0] + scroll[0];
	tpos[1] = pos[1];
	tpos[2] = pos[2];
	tpos[3] = pos[3];
	endx = endx(bw->val, bw->caret, bw->font, tpos, eb->multiline);
	free(drawval);
	
	if(endx <= pos[0])
		scroll[0] += pos[0] - endx + 1;
	else if(endx >= pos[2])
		scroll[0] -= endx - pos[2] + 1;
	
	return dtrue;
}

void ebwcopyval(ebw *eb)
{
	wg *bw;
	float *highl;
	char *highc;
	
	bw = &eb->base;
	highl = bw->highl
	
	if(highl[1] > 0 && highl[0] != highl[1])
	{
		substr(&highc, bw->val, highl[0], highl[1] - highl[0]);
		SDL_SetClipboardText(highc);
		free(highc);
	}
	else
		SDL_SetClipboardText("");
}

void ebwpasteval(ebw *eb)
{
	wg *bw;
	
	bw = &eb->base;
	
	placestr( SDL_GetClipboardText() );
	
	if(!bw->passw)
		parsetags(&bw->val, bw->val, &bw->caret);
}

void ebwselectall(ebw *eb)
{
	wg *bw;
	float *highl, tpos[4], *pos, *scroll;
	char *drawval;
	int endx, len;
	
	bw = &eb->base;
	highl = bw->highl;
	pos = bw->pos;
	scroll = bw->scroll;
	
	len = strlen(bw->val);
	highl[0] = 0;
	highl[1] = len + 1;
	bw->caret = -1;

	drawval = ebwdrawvalue(eb);
	tpos[0] = pos[0] + scroll[0];
	tpos[1] = pos[1];
	endx = endx(val, len, font, tpos, dfalse);
	free(drawval);

	if(endx <= pos[2])
		scroll[0] += pos[2] - endx - 1;

	if(scroll[0] >= 0)
		scroll[0] = 0;
}

void ebwclose(ebw *eb)
{
	ebwlosefocus(eb);
}

void ebwgainfocus(ebw *eb)
{
	wg *bw;
	float *pos;
	
	bw = &eb->base;
	pos = bw->pos;
	
	if(!bw->opened)
	{
		if(g_kbfocus > 0)
		{
			SDL_StopTextInput();
			--g_kbfocus;
		}

		bw->opened = dtrue;
		SDL_StartTextInput();
		SDL_Rect r;
		// TODO ???
		r.x = (int)pos[0];
		r.y = (int)pos[3];
		r.w = (int)(g_winsz.x - pos[0]);
		r.h = (int)(g_winsz.y - pos[3]);
		SDL_SetTextInputRect(&r);
		++g_kbfocus;
	}
}

void ebwlosefocus(ebw *eb)
{
	wg *bw;
	
	bw = &eb->base;
	
	if(bw->opened)
	{
		if(g_kbfocus > 0)
		{
			SDL_StopTextInput();
			--g_kbfocus;
		}

		bw->opened = dfalse;
	}
}
#endif