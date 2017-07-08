










#include "cursor.h"

unsigned int g_cursor[CU_STATES];

void loadcursors()
{
	SDL_ShowCursor(0);
	loadsp("cursor/default", &g_cursor[CU_DEFAULT], ecfalse, ecfalse);
	loadsp("cursor/move", &g_cursor[CU_MOVE], ecfalse, ecfalse);
	loadsp("cursor/resh", &g_cursor[CU_RESZL], ecfalse, ecfalse);
	loadsp("cursor/resh", &g_cursor[CU_RESZR], ecfalse, ecfalse);
	loadsp("cursor/resv", &g_cursor[CU_RESZT], ecfalse, ecfalse);
	loadsp("cursor/resv", &g_cursor[CU_RESZB], ecfalse, ecfalse);
	loadsp("cursor/resd2", &g_cursor[CU_RESZTL], ecfalse, ecfalse);
	loadsp("cursor/resd1", &g_cursor[CU_RESZTR], ecfalse, ecfalse);
	loadsp("cursor/resd1", &g_cursor[CU_RESZBL], ecfalse, ecfalse);
	loadsp("cursor/resd2", &g_cursor[CU_RESZBR], ecfalse, ecfalse);
	loadsp("cursor/default", &g_cursor[CU_WAIT], ecfalse, ecfalse);
	loadsp("cursor/default", &g_cursor[CU_DRAG], ecfalse, ecfalse);
}