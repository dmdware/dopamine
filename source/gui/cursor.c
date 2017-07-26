










#include "cursor.h"

unsigned int g_cursor[CU_STATES];

void loadcursors()
{
	SDL_ShowCursor(0);
	loadsp("cursor/default", &g_cursor[CU_DEFAULT], dfalse, dfalse);
	loadsp("cursor/move", &g_cursor[CU_MOVE], dfalse, dfalse);
	loadsp("cursor/reszh", &g_cursor[CU_RESZL], dfalse, dfalse);
	loadsp("cursor/reszh", &g_cursor[CU_RESZR], dfalse, dfalse);
	loadsp("cursor/reszv", &g_cursor[CU_RESZT], dfalse, dfalse);
	loadsp("cursor/reszv", &g_cursor[CU_RESZB], dfalse, dfalse);
	loadsp("cursor/reszd2", &g_cursor[CU_RESZTL], dfalse, dfalse);
	loadsp("cursor/reszd1", &g_cursor[CU_RESZTR], dfalse, dfalse);
	loadsp("cursor/reszd1", &g_cursor[CU_RESZBL], dfalse, dfalse);
	loadsp("cursor/reszd2", &g_cursor[CU_RESZBR], dfalse, dfalse);
	loadsp("cursor/default", &g_cursor[CU_WAIT], dfalse, dfalse);
	loadsp("cursor/default", &g_cursor[CU_DRAG], dfalse, dfalse);
}