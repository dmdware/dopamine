#ifndef ICON_H
#define ICON_H

#include "../sys/inc.h"

#define MAX_TAG		31	//not counting null terminator

struct icot
{
	unsigned int texi;
	int w;
	int h;
	char tag[MAX_TAG];
};

typedef struct icot icot;

#define ICON_DOLLARS		0
#define STDICONS			1
#define	ICONS				(STDICONS+256)

extern icot g_icon[ICONS];

void icot_init(icot *i);
void icot_free(icot *i);

#ifndef MATCHMAKER
void defi(int type, char *relative, char *tag);
#endif

#endif
