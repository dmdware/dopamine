

#ifndef FBO_H
#define FBO_H

void makefbo(unsigned int* rendertex, unsigned int* renderrb, unsigned int* renderfb, unsigned int* renderdepthtex, int w, int h);
void delfbo(unsigned int* rendertex, unsigned int* renderrb, unsigned int* renderfb, unsigned int* renderdepthtex);

#endif