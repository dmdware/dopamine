











#include "../sys/inc.h"
#include "../sys/syswin.h"
#include "../sys/utils.h"
#include "../sys/texture.h"
#include "../sys/debug.h"

void savesc()
{
	texdata screenshot;
	char relative[256];
	char fullpath[DMD_MAX_PATH + 1];
	char fdt[123];

	screenshot.channels = 3;
	screenshot.sizex = g_width;
	screenshot.sizey = g_height;
	screenshot.data = (unsigned char*)malloc( sizeof(unsigned char) * g_width * g_height * 3 );

	NOMEM(screenshot.data);

	//memset(screenshot.data, 0, g_width * g_height * 3);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glReadPixels(0, 0, g_width, g_height, GL_RGB, GL_UNSIGNED_BYTE, screenshot.data);
	
	fliptex(&screenshot);

	filedatetime(fdt, 122);
	//sprintf(relative, "screenshots/%s.jpg", fdt);
	sprintf(relative, "screenshots/%s.png", fdt);
	fullwrite(relative, fullpath);

	fprintf(g_applog, "writing screenshot %s\r\n", fullpath);
	
	//savejpg(fullpath, &screenshot, 0.9f);
	savepng(fullpath, &screenshot);

	free(screenshot.data);
}
