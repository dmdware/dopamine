

#ifndef FIXTRIG_H
#define FIXTRIG_H

#include "fixmath.h"
#include "fixfrac.h"

#if 01
//#define TABLESZ		1024
#define TABLESZ		RATIO_DENOM

//(0,2*M_PI)->(-1,1)
extern fx COSTABLE[TABLESZ];
extern fx SINTABLE[TABLESZ];
extern fx TANTABLE[TABLESZ];
//(-1,1)->(0,2*M_PI)
extern fx ACOSTABLE[TABLESZ];
extern fx ASINTABLE[TABLESZ];
extern fx ATANTABLE[TABLESZ];

fx cosc(fx rad);
fx sinc(fx rad);
fx tanc(fx rad);
fx acosc(fx ratio);
fx asinc(fx ratio);
fx atanc(fx ratio);
fx atan2c(fx x, fx y);
void maptrig();
#endif

#endif
