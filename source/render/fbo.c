

#include "../sys/inc.h"
#include "../sys/utils.h"
#include "fbo.h"

void makefbo(unsigned int* rendertex, unsigned int* renderrb, unsigned int* renderfb, unsigned int* renderdepthtex, int w, int h)
{
	/* OpenGL 1.4 way */
	GLenum DrawBuffers[2];

	glGenTextures(1, rendertex);
	glBindTexture(GL_TEXTURE_2D, *rendertex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, renderdepthtex);
	glBindTexture(GL_TEXTURE_2D, *renderdepthtex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(1, renderfb);
	glBindFramebuffer(GL_FRAMEBUFFER, *renderfb);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *renderdepthtex, 0);

	DrawBuffers[0] = GL_COLOR_ATTACHMENT0;
	DrawBuffers[1] = GL_DEPTH_ATTACHMENT;
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		errm("Error", "Couldn't create framebuffer for render.");
		return;
	}
}

void delfbo(unsigned int* rendertex, unsigned int* renderrb, unsigned int* renderfb, unsigned int* renderdepthtex)
{
	/* delete resources */
	glDeleteTextures(1, rendertex);
	glDeleteTextures(1, renderdepthtex);
	/* Bind 0, which means render to back buffer, as a result, fb is unbound */
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, renderfb);
	CHECKGL();
}