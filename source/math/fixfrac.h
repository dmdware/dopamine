

#ifndef FIXFRAC_H
#define FIXFRAC_H

#include "../sys/inc.h"
#include "../algo/vec.h"
#include "../algo/bool.h"

#define FX_PREC				67
#define FX_DENOM			"1000000000"
#define FX_DENOM_10POW		9

extern mp_int g_denom;

struct fx
{
	mp_int r;
	mp_int i;
};

typedef struct fx fx;

void fxinit(fx* x);
void fxfree(fx* x);
void fxsetri(fx* x, int i);
void fxsetii(fx* x, int i);
void fxsetrlf(fx* x, double f);
void fxsetilf(fx* x, double f);
void fxsetrli(fx* x, __int64 i);
void fxsetili(fx* x, __int64 i);
void fxsetrf(fx* x, float f);
void fxsetif(fx* x, float f);
dbool fxeq(fx* xa, fx* xb);
dbool fxneq(fx* xa, fx* xb);
dbool fxgt(fx* xa, fx* xb);
dbool fxlt(fx* xa, fx* xb);
dbool fxgteq(fx* xa, fx* xb);
dbool fxlteq(fx* xa, fx* xb);
void fxmod(fx* xr, fx* xa, fx* xm);	
//b%dv = b-(b/dv)*dv
//better to define b%dv = (0>=|b|>dv)
//while(|b|>=dv) { if(b>=0) b-=dv else b+=dv }	dv>0
void fxsub(fx* xc, fx* xa, fx* xb);	
void fxadd(fx* xc, fx* xa, fx* xb);	
void fxdiv(fx* xc, fx* xr, fx* xa, fx* xb);
void fxmul(fx* xc, fx* xa, fx* xb);	
void fxtoli(char* s, __int64* li);	
void fxrtoli(fx* x, __int64* li);	
void fxitoli(fx* x, __int64* li);
void fxtolf(char* s, double* lf);	
void fxrtolf(fx* x, double* lf);	
void fxitolf(fx* x, double* lf);
void fxtoi(char* s, float* i);
void fxrtoi(fx* x, int* i);	
void fxitoi(fx* x, int* i);
void fxtof(char* s, float* f);
void fxrtof(fx* x, float* f);	
void fxitof(fx* x, float* f);
void fxabs(fx* xc, fx* xa);
void fxsqrt(fx* xc, fx* xa);
void fxsign(fx* xs, fx* xa);
void fxmin(fx* xc, fx* xa, fx* xb);
void fxmax(fx* xc, fx* xa, fx* xb);
void fxpow(fx* xc, fx* xb, fx* xp);
void fxfac(fx* xc, fx* xa);

#endif

