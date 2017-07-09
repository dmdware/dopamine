
/*
 * Copyright (c) 2007 Alexey Vatchenko <av@bsdua.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * utf8: implementation of UTF-8 charset encoding (RFC3629).
 */

#ifndef UTF8_H
#define UTF8_H

#include "../sys/inc.h"


#define _NXT	0x80
#define _SEQ2	0xc0
#define _SEQ3	0xe0
#define _SEQ4	0xf0
#define _SEQ5	0xf8
#define _SEQ6	0xfc

#define _BOM	0xfeff


#define UTF8_IGNORE_ERROR		0x01
#define UTF8_SKIP_BOM			0x02

int		utf8_to_wchar(const char *in, int insize, unsigned int *out,
		    int outsize, int flags);
int		wchar_to_utf8(const unsigned int *in, int insize, char *out,
		    int outsize, int flags);



//static 
int __wchar_forbitten(unsigned int sym);
//static 
int __utf8_forbitten(unsigned char octet);

#endif


