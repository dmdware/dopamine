#ifndef APPMAIN_H
#define APPMAIN_H

#define APPVER				0

#define CFGFILE				"config.ini"
#define APPTIT				"Subfield"

#define APPMODE_LOGO		0
#define APPMODE_INTRO		1
#define APPMODE_LOADING		2
#define APPMODE_RELOADING	3
#define APPMODE_MENU		4
#define APPMODE_PLAY		5
#define APPMODE_PAUSE		6
#define APPMODE_EDITOR		7
#define APPMODE_JOINING		8
extern char g_appmode;

#define VIEWMODE_FIRST		0
#define VIEWMODE_THIRD		1
#define VIEWMODES			2
extern char g_viewmode;

#ifdef PLAT_WIN
extern HINSTANCE g_hinst;
#endif

#endif
