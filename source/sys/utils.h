

#ifndef UTILS_H
#define UTILS_H

#include "inc.h"

extern FILE *g_applog;

void openlog(const char* file, int ver);
int max2pow(int lowerbound);
int maxpow32(int lowerbound);
unsigned __int64 getticks();
void fullwrite(const char* file, char* full);
void fullpath(const char* file, char* full);
void makedir(const char* full);
void errm(const char* title, const char* m);
void infm(const char* title, const char* m);
void warm(const char* title, const char* m);
void stripfile(char* in, char* out);
void strippath(char* in, char* out);
void stripext(char* in, char* out);
void corslash(char *s);
void exepath(char* exe);
void makerel(const char* full, char* rel);

float maxf(const float a, const float b);
float minf(const float a, const float b);

int maxi(const int x, const int y);
int mini(const int x, const int y);

#define enmax(a,b) (((a)>(b))?(a):(b))
#define enmin(a,b) (((a)<(b))?(a):(b))

int iabs(int v);
int isign(int x);
double fsign(double x);
int iceil(const int num, const int denom);

#endif