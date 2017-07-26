

#include "richtext.h"
#include "icon.h"
#include "../sys/utils.h"

static char temputf8[7];

char *fromglyph(const unsigned int in, int *adv)
{
	const unsigned int *w = &in;
	unsigned int ch;
	unsigned char *p, *oc;
	int n;
	
	*adv = 0;
	
	p = (unsigned char *)temputf8;
	
	if (in == _BOM /*&& (flags & UTF8_SKIP_BOM) != 0 */ )
	{
		p[0] = 0;
		return temputf8;
	}
	
	if (in < 0) {
		/*if ((flags & UTF8_IGNORE_ERROR) == 0)
			return (0); */
		p[0] = 0;
		return temputf8;
	} else if (*w <= 0x0000007f)
		n = 1;
	else if (*w <= 0x000007ff)
		n = 2;
	else if (*w <= 0x0000ffff)
		n = 3;
	else if (*w <= 0x001fffff)
		n = 4;
	else if (*w <= 0x03ffffff)
		n = 5;
	else /*if (*w <= 0x7fffffff) */
		n = 6;
	
	/*make it work under different endians */
	ch = htonl(in);
	oc = (unsigned char *)&ch;
	switch (n) {
		case 1:
			p[1] = 0;
			p[0] = oc[3];
			break;
			
		case 2:
			p[2] = 0;
			p[1] = _NXT | oc[3] & 0x3f;
			p[0] = _SEQ2 | (oc[3] >> 6) | ((oc[2] & 0x07) << 2);
			break;
			
		case 3:
			p[3] = 0;
			p[2] = _NXT | oc[3] & 0x3f;
			p[1] = _NXT | (oc[3] >> 6) | ((oc[2] & 0x0f) << 2);
			p[0] = _SEQ3 | ((oc[2] & 0xf0) >> 4);
			break;
			
		case 4:
			p[4] = 0;
			p[3] = _NXT | oc[3] & 0x3f;
			p[2] = _NXT | (oc[3] >> 6) | ((oc[2] & 0x0f) << 2);
			p[1] = _NXT | ((oc[2] & 0xf0) >> 4) |
			((oc[1] & 0x03) << 4);
			p[0] = _SEQ4 | ((oc[1] & 0x1f) >> 2);
			break;
			
		case 5:
			p[5] = 0;
			p[4] = _NXT | oc[3] & 0x3f;
			p[3] = _NXT | (oc[3] >> 6) | ((oc[2] & 0x0f) << 2);
			p[2] = _NXT | ((oc[2] & 0xf0) >> 4) |
			((oc[1] & 0x03) << 4);
			p[1] = _NXT | (oc[1] >> 2);
			p[0] = _SEQ5 | oc[0] & 0x03;
			break;
			
		case 6:
			p[6] = 0;
			p[5] = _NXT | oc[3] & 0x3f;
			p[4] = _NXT | (oc[3] >> 6) | ((oc[2] & 0x0f) << 2);
			p[3] = _NXT | (oc[2] >> 4) | ((oc[1] & 0x03) << 4);
			p[2] = _NXT | (oc[1] >> 2);
			p[1] = _NXT | oc[0] & 0x3f;
			p[0] = _SEQ6 | ((oc[0] & 0x40) >> 6);
			break;
	}
	
	*adv = n;

	return temputf8;
}

unsigned int toglyph(const char *in, int *index)
{
	unsigned char *p;//, *lim;
	unsigned int high;
	int n, 
		//total, 
		i, n_bits;
	
	unsigned int out;
	
	if (in == NULL || *in == 0)
		return 0;
	
	p = (unsigned char *)in;
	
	/*
	 * Get number of bytes for one wide character.
	 */
	n = 1;	/*default: 1 byte. Used when skipping bytes. */
	if ((*p & 0x80) == 0)
		high = (unsigned int)*p;
	else if ((*p & 0xe0) == _SEQ2) {
		n = 2;
		high = (unsigned int)(*p & 0x1f);
	} else if ((*p & 0xf0) == _SEQ3) {
		n = 3;
		high = (unsigned int)(*p & 0x0f);
	} else if ((*p & 0xf8) == _SEQ4) {
		n = 4;
		high = (unsigned int)(*p & 0x07);
	} else if ((*p & 0xfc) == _SEQ5) {
		n = 5;
		high = (unsigned int)(*p & 0x03);
	} else if ((*p & 0xfe) == _SEQ6) {
		n = 6;
		high = (unsigned int)(*p & 0x01);
	} else {
		return 0;
	}
	
	out = 0;
	n_bits = 0;
	for (i = 1; i < n; i++) {
		out |= (unsigned int)(p[n - i] & 0x3f) << n_bits;
		n_bits += 6;		/*6 low bits in every byte */
	}
	out |= high << n_bits;
	
	if(index)
		(*index) += n;
	
	return out;
}

void parsetags(char **ndest, char *src, int *caret)
{
	short icon;
	int len, taglen, srci, desti, iconi, ci, adv;
	dbool match;
	char *tag, *iconc, *tempdest, tempc[2];
	unsigned int k;
	
	len = strlen(src);
	pstrset(&tempdest, "");
	
	for(srci=0, desti=0; srci<len; )
	{
		for(icon=0; icon<ICONS; ++icon)
		{
			match = dtrue;
			tag = g_icon[icon].tag;
			taglen = strlen(tag);
			if(!taglen)
				continue;
			for(iconi=0; iconi<taglen && iconi+srci<len; ++iconi)
			{
				if(tag[iconi] == src[iconi+srci])
					continue;
				match = dfalse;
				break;
			}
			if(!match)
				continue;
			k = RICH_ICON_START + icon;
			iconc = fromglyph(k, &adv);
			pstradd(&tempdest, iconc);
			if(caret && *caret > ci)
				(*caret) -= mini(taglen,*caret-ci);
			srci+=taglen;
			ci+=taglen;
			desti+=strlen(iconc);
			break;
		}
		
		if(match)
			continue;
		
		tempc[0] = src[srci];
		tempc[1] = 0;
		pstradd(&tempdest, tempc);
		++srci;
		//++glyphi;
		++desti;
	}
	
	free(*ndest);
	*ndest = tempdest;
}

void pwver(char **ndest, char *src)	//asterisk-mask password string
{
	int len;
	len = richlen(src);
	*ndest = (char*)malloc(len+1);
	(*ndest)[len] = 0;
	for(--len; len>=0; --len)
		(*ndest)[len] = '*';
}


int richlen(const char *in)
{
	unsigned int out;
	unsigned char *p, *lim;
	unsigned int //*wlim, 
		high;
	int n, total, i, n_bits;
	int insize;
	int outsize;

	insize = strlen(in);
	outsize = insize;

	if (in == NULL)
		return (0);

	total = 0;
	p = (unsigned char *)in;
	lim = p + insize;
	//wlim = out + outsize;

	for (; p < lim; p += n) {
		//if (__utf8_forbitten(*p) != 0 &&
		  //  (flags & UTF8_IGNORE_ERROR) == 0)
			//return (0);

		/*
		 * Get number of bytes for one wide character.
		 */
		n = 1;	/*default: 1 byte. Used when skipping bytes. */
		if ((*p & 0x80) == 0)
			high = (unsigned int)*p;
		else if ((*p & 0xe0) == _SEQ2) {
			n = 2;
			high = (unsigned int)(*p & 0x1f);
		} else if ((*p & 0xf0) == _SEQ3) {
			n = 3;
			high = (unsigned int)(*p & 0x0f);
		} else if ((*p & 0xf8) == _SEQ4) {
			n = 4;
			high = (unsigned int)(*p & 0x07);
		} else if ((*p & 0xfc) == _SEQ5) {
			n = 5;
			high = (unsigned int)(*p & 0x03);
		} else if ((*p & 0xfe) == _SEQ6) {
			n = 6;
			high = (unsigned int)(*p & 0x01);
		} else {
			//if ((flags & UTF8_IGNORE_ERROR) == 0)
			//	return (0);
			total++;
			continue;
		}

		/*does the sequence header tell us truth about length? */
		if (lim - p <= n - 1) {
			//if ((flags & UTF8_IGNORE_ERROR) == 0)
			//	return (0);
			n = 1;
			total++;
			continue;	/*skip */
		}

		/*
		 * Validate sequence.
		 * All symbols must have higher bits set to 10xxxxxx
		 */
		if (n > 1) {
			for (i = 1; i < n; i++) {
				if ((p[i] & 0xc0) != _NXT)
					break;
			}
			if (i != n) {
	//			if ((flags & UTF8_IGNORE_ERROR) == 0)
	//				return (0);
				n = 1; 
				total++;
				continue;	/*skip */
			}
		}

		total++;

		//if (out == NULL)
		//	continue;

		//if (out >= wlim)
		//	return (0);		/*no space left */

		out = 0;
		n_bits = 0;
		for (i = 1; i < n; i++) {
			out |= (unsigned int)(p[n - i] & 0x3f) << n_bits;
			n_bits += 6;		/*6 low bits in every byte */
		}
		out |= high << n_bits;

		if (__wchar_forbitten(out) != 0) {
	//		if ((flags & UTF8_IGNORE_ERROR) == 0)
	//			return (0);	/*forbitten character */
	//		else 
			{
				total--;
				//out--;
			}
		}
		else if (out == _BOM 
			//&& (flags & UTF8_SKIP_BOM) != 0
			) 
		//else
		{
			total--;
			//out--;
		}

		//out++;
	}

	return (total);
}


/* only use with dynamic array string */

void pstrset(char **out, const char *in)
{
	int len = strlen(in);
	
	*out = (char*)malloc(len+1);
	memcpy(*out, in, len+1);
}

void pstradd(char **out, const char *in)
{
	int addlen = strlen(in);
	int len = 0;
	
	len = strlen(*out);
	*out = (char*)realloc(*out, len + addlen + 1);
	
	memcpy(&(*out)[len], in, addlen + 1);
}

void psubstr(char **out, const char *in, int beg, int len)
{
	*out = (char*)malloc(len + 1);
	memcpy(*out, &in[beg], len);
	(*out)[len] = 0;
}

void delprev(char **s, int *caret)
{
	int glyphi, ci, adv;
	unsigned int k;
	char *sub1, *sub2;
	
	for(glyphi=0, ci=0; (*s)[ci]; ci+=adv, glyphi++)
	{
		adv = 0;
		k = toglyph(&(*s)[ci], &adv);
		
		if(ci > 0 && adv + ci >= *caret)
		{
			psubstr(&sub1, *s, 0, ci);
			psubstr(&sub2, *s, adv+ci, strlen(*s)-adv-ci);
			free(*s);
			pstrset(s, sub1);
			pstradd(s, sub2);
			free(sub1);
			free(sub2);
			(*caret) -= (*caret)-ci;
			return;
		}
	}
}

void delnext(char **s, int *caret)
{
	int glyphi, ci, adv;
	unsigned int k;
	char *sub1, *sub2;
	
	for(glyphi=0, ci=0; (*s)[ci]; ci+=adv, glyphi++)
	{
		adv = 0;
		k = toglyph(&(*s)[ci], &adv);
		
		if(adv + ci > *caret)
		{
			psubstr(&sub1, *s, 0, ci);
			psubstr(&sub2, *s, adv+ci, strlen(*s)-adv-ci);
			free(*s);
			pstrset(s, sub1);
			pstradd(s, sub2);
			free(sub1);
			free(sub2);
			(*caret) -= (*caret)-ci;
			return;
		}
	}
}

int prevlen(char *s, int caret)
{
	int glyphi, ci, adv;
	unsigned int k;
	
	for(glyphi=0, ci=0; s[ci]; ci+=adv, glyphi++)
	{
		adv = 0;
		k = toglyph(&s[ci], &adv);
		
		if(ci > 0 && adv + ci >= caret)
			return adv;
	}
	
	return 0;
}

int nextlen(char *s, int caret)
{
	int glyphi, ci, adv;
	unsigned int k;
	
	for(glyphi=0, ci=0; s[ci]; ci+=adv, glyphi++)
	{
		adv = 0;
		k = toglyph(&s[ci], &adv);
		
		if(adv + ci > caret)
			return adv;
	}
	
	return 0;
}