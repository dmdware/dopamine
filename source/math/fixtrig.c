

#include "fixtrig.h"


#if 0
//(0,2*M_PI)->(-1,1)
fx COSTABLE[TABLESZ];
fx SINTABLE[TABLESZ];
fx TANTABLE[TABLESZ];
//(-1,1)->(0,2*M_PI)
fx ACOSTABLE[TABLESZ];
fx ASINTABLE[TABLESZ];
fx ATANTABLE[TABLESZ];

int FromRadToIndex(double r)
{
	return ((TABLESZ+(int)(r/M_PI/2.0*TABLESZ)%TABLESZ)%TABLESZ);
}

int FromRatioToIndex(double r)
{
	return ((TABLESZ+(int)((1.0+r/2.0)*TABLESZ)%TABLESZ)%TABLESZ);
}

void maptrig()
{
	for(int i=0; i<TABLESZ; ++i)
	{
		COSTABLE[i] = ToFrac(cos((double)i/(double)TABLESZ*M_PI*2.0));
		SINTABLE[i] = ToFrac(sin((double)i/(double)TABLESZ*M_PI*2.0));
		TANTABLE[i] = ToFrac(tan((double)i/(double)TABLESZ*M_PI*2.0));

		ACOSTABLE[i] = ToFrac(acos((double)(i-(TABLESZ/2))/(double)(TABLESZ/2)));
		ASINTABLE[i] = ToFrac(asin((double)(i-(TABLESZ/2))/(double)(TABLESZ/2)));
		ATANTABLE[i] = ToFrac(atan((double)(i-(TABLESZ/2))/(double)(TABLESZ/2)));
	}
}

fx cos(fx rad)
{
	//return ToBasis((fx(TABLESZ)-((rad*fx(TABLESZ))/ToFrac(M_PI*2.0)%fx(TABLESZ))));
	
	//return (rad*fx(TABLESZ))/ToFrac(M_PI*2.0)%fx(TABLESZ);
	return COSTABLE[
	FromRadToIndex(ToBasis2(rad))
	];
}

fx sin(fx rad)
{
	return SINTABLE[
	FromRadToIndex(ToBasis2(rad))
	];
}

fx tan(fx rad)
{
	return TANTABLE[
	FromRadToIndex(ToBasis2(rad))
	];
}

fx acos(fx ratio)
{
	return ACOSTABLE[
	FromRatioToIndex(ToBasis2(ratio))
	];
}

fx asin(fx ratio)
{
	return ASINTABLE[
	FromRatioToIndex(ToBasis2(ratio))
	];
}

fx atan(fx ratio)
{
	return ATANTABLE[
	FromRatioToIndex(ToBasis2(ratio))
	];
}

fx atan2(fx x, fx y)
{
	fx ratio = x/y;
	return atan(ratio);
}
#endif

#if 0
//https://en.wikibooks.org/wiki/Trigonometry/Power_Series_for_Cosine_and_Sine
fx cosx(fx rad)
{
	fx r = ToFrac(1);
	fx end = ToFrac(1)/ToFrac(100000);
	fx last = ToFrac(1);
	fx i = ToFrac(2);

	while(last >= end)
	{
		r = r - powx(rad, i) / facx(i);
		i = i * ToFrac(2);
		
		last = powx(rad, i) / facx(i);
		r = r + last;
		i = i * ToFrac(2);
	}

	return r;
}

fx sinx(fx rad)
{
	fx r = ToFrac(1);
	fx end = ToFrac(1)/ToFrac(100000);
	fx last = ToFrac(1);
	fx i = ToFrac(2);

	while(last >= end)
	{
		r = r - powx(rad, i) / facx(i);
		i = (i-ToFrac(1)) * ToFrac(2) + ToFrac(1);
		
		last = powx(rad, i) / facx(i);
		r = r + last;
		i = (i-ToFrac(1)) * ToFrac(2) + ToFrac(1);
	}

	return r;
}

//http://math.stackexchange.com/questions/1238965/working-out-tan-x-using-sin-and-cos-expansion
fx tanx(fx rad)
{
	//sinx = cosx tanx
	//sinx/cosx = tanx

	return (sinx(rad)/cosx(rad));
}

#define ATRIGSTEPS	10000000
fx acosx(fx ratio)
{
	//range: 0, pi

/*
Find: acos(a) = Φ, a>=0

Given: a
a = xf
yf = √(1 - xf^2)
0≤Φ≤π/2
x0 = 1
y0 = 0

Then:

lim n→∞ ( Σi=1 to n ( √( (xi - xi-1)^2 + (yi - yi-1)^2 ) ) )

x(i) = x0 - xf*i/n

y(i) = √( 1 - x(i)^2 )
*/

	if(ratio >= fx(0))
	{
		fx n = fx(ATRIGSTEPS);
		fx i = fx(0);
		fx a = fx(a);

		fx lastx = fx(1);
		fx lasty = fx(0);

		while(i < n)
		{
			fx cury = i/n;
			fx curx = sqrtx( fx(1) - cury*cury );

			a = a + sqrtx( (curx-lastx)*(curx-lastx) + (cury-lasty)*(cury-lasty) );

			i=i+fx(1);

			lastx = curx;
			lasty = cury;
		}

		return a;
	}

}

fx asinx(fx ratio)
{
	//range: -pi/2 to pi/2

/*
Find: asin(o) = Φ, o>=0

Given: o
o = yf
xf = √(1 - yf^2)
0≤Φ≤π/2
x0 = 1
y0 = 0

Then:

lim n→∞ ( Σi=1 to n ( √( (xi - xi-1)^2 + (yi - yi-1)^2 ) ) )

y(i) = y0 + yf*i/n

x(i) = √( 1 - y(i)^2 )
*/

	if(ratio >= fx(0))
	{
		fx n = fx(ATRIGSTEPS);
		fx i = fx(0);
		fx a = fx(a);

		fx lastx = fx(1);
		fx lasty = fx(0);

		while(i < n)
		{
			fx cury = i/n;
			fx curx = sqrtx( fx(1) - cury*cury );

			a = a + sqrtx( (curx-lastx)*(curx-lastx) + (cury-lasty)*(cury-lasty) );

			i=i+fx(1);

			lastx = curx;
			lasty = cury;
		}

		return a;
	}
/*
Find: asin(o) = Φ, o<0

Given: o
o = yf
xf = √(1 - yf^2)
-π/2≤Φ≤0
x0 = 1
y0 = 0

Then:

lim n→∞ ( Σi=1 to n ( -√( (xi - xi-1)^2 + (yi - yi-1)^2 ) ) )

y(i) = y0 + yf*i/n

x(i) = √( 1 - y(i)^2 )
*/

	else
	{
		fx n = fx(ATRIGSTEPS);
		fx i = fx(0);
		fx a = fx(a);

		fx lastx = fx(1);
		fx lasty = fx(0);

		while(i < n)
		{
			fx cury = fx(0) - i/n;
			fx curx = sqrtx( fx(1) - cury*cury );

			a = a - sqrtx( (curx-lastx)*(curx-lastx) + (cury-lasty)*(cury-lasty) );

			i=i+fx(1);

			lastx = curx;
			lasty = cury;
		}

		return a;
	}
}

fx atanx(fx ratio)
{
	//range: -pi/2 to pi/2

	return asinx(ratio);
}


fx atan2x(fx x, fx y)
{
	//range: -pi to pi

	return atanx(x);
}

#endif