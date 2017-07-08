

#include "fixfrac.h"
#include "../sys/utils.h"
#include "fixmath.h"

mp_int g_denom;

#if 0

void fxinit(fx* x)
{
	mp_init_size(&x->r, FX_PREC);
	mp_init_size(&x->i, FX_PREC);
}

void fxfree(fx* x)
{
	mp_clear(&x->r);
	mp_clear(&x->i);
}

void fxsetri(fx* x, int i)
{
	//mp_int m;
	mp_set_int(&x->r, i);
	//mp_init_size(&m, FX_PREC);
	//mp_set_int(&m, FX_PREC);
	mp_mul(&x->r, &x->r, &g_denom);
	//mp_clear(&m);
}

void fxsetii(fx* x, int i)
{
	//mp_int m;
	mp_set_int(&x->i, i);
	//mp_init_size(&m, FX_PREC);
	//mp_read_radix(&m, FX_DENOM, 10);
	mp_mul(&x->i, &x->i, &g_denom);
	//mp_clear(&m);
}

void fxsetrlf(fx* x, double f)
{
	mp_int d;
	mp_set_int(&x->r, (int)(f*15));
	mp_init_size(&d, FX_PREC);
	mp_set_int(&d, 15);
	mp_mul(&x->r, &x->r, &g_denom);
	mp_div(&x->r, &x->r, &d);
	mp_clear(&d);
}

void fxsetilf(fx* x, double f)
{
	mp_int d;
	mp_set_int(&x->i, (int)(f*15));
	mp_init_size(&d, FX_PREC);
	mp_set_int(&d, 15);
	mp_mul(&x->i, &x->i, &g_denom);
	mp_div(&x->i, &x->i, &d);
	mp_clear(&d);
}

void fxsetrli(fx* x, __int64 i)
{
	mp_set_int(&x->r, i);
	mp_mul(&x->r, &x->r, &g_denom);
}

void fxsetili(fx* x, __int64 i)
{
	mp_set_int(&x->i, i);
	//mp_init_size(&m, FX_PREC);
	//mp_read_radix(&m, FX_DENOM, 10);
	mp_mul(&x->i, &x->i, &g_denom);
}

void fxsetrf(fx* x, float f)
{
	mp_int d;
	mp_set_int(&x->i, (int)(f*7));
	mp_init_size(&d, FX_PREC);
	mp_set_int(&d, 7);
	mp_mul(&x->r, &x->r, &g_denom);
	mp_div(&x->r, &x->r, &d);
	mp_clear(&d);
}

void fxsetif(fx* x, float f)
{
	mp_int d;
	mp_set_int(&x->i, (int)(f*7));
	mp_init_size(&d, FX_PREC);
	mp_set_int(&d, 7);
	mp_mul(&x->i, &x->i, &g_denom);
	mp_div(&x->i, &x->i, &d);
	mp_clear(&d);
}

dbool fxeq(fx* xa, fx* xb)
{
	return (mp_cmp(&xa->r, &xb->r) == MP_EQ);
}

dbool fxneq(fx* xa, fx* xb)
{
	return (mp_cmp(&xa->r, &xb->r) != MP_EQ);
}

dbool fxgt(fx* xa, fx* xb)
{
	return (mp_cmp(&xa->r, &xb->r) == MP_GT);
}

dbool fxlt(fx* xa, fx* xb)
{
	return (mp_cmp(&xa->r, &xb->r) == MP_LT);
}

dbool fxgteq(fx* xa, fx* xb)
{
	return (mp_cmp(&xa->r, &xb->r) >= MP_EQ);
}

dbool fxlteq(fx* xa, fx* xb)
{
	return (mp_cmp(&xa->r, &xb->r) <= MP_EQ);
}

void fxmod(fx* xr, fx* xa, fx* xm)
{
	//b%dv = b-(b/dv)*dv
	//better to define b%dv = (0>=|b|>dv)
	//while(|b|>=dv) { if(b>=0) b-=dv else b+=dv }	dv>0
	//mp_int b;
	//mp_init_copy(&b, xa);
	//mp_div(
	//mp_mod(xr, xm, &b);
	mp_mod(&xa->r, &xm->r, &xr->r);
	//mp_mul(&b, &g_denom, &b);
	//mp_clear(xr);
	//mp_init_copy(xr, &b);
	//mp_clear(&b);
}

void fxsub(fx* xc, fx* xa, fx* xb)
{
	mp_sub(&xa->r, &xb->r, &xc->r);
}

void fxadd(fx* xc, fx* xa, fx* xb)
{
	mp_add(&xa->r, &xb->r, &xc->r);
}

void fxdiv(fx* xc, fx* xr, fx* xa, fx* xb)
{
	mp_div(&xa->r, &xb->r, &xc->r, &xr->r);
}

void fxmul(fx* xc, fx* xa, fx* xb)
{
	mp_mul(&xa->r, &xb->r, &xc->r);
}

void fxtoli(char* s, __int64* li)
{
	int j;
	int len;
	__int64 pow;

	len = strlen(s);
	*li = 0;
	pow = 1;

	for(j=FX_DENOM_10POW; j<len; j++)
	{
		*li = *li + (pow * (__int64)(s[j]-'0'));
		pow = pow * 10;
	}
}

void fxrtoli(fx* x, __int64* li)
{
	char s[DMD_MAX_PATH+1] = "";
	mp_toradix(&x->r, s, 10);
	fxtoli(s, li);
}

void fxitoli(fx* x, __int64* li)
{
	char s[DMD_MAX_PATH+1] = "";
	mp_toradix(&x->i, s, 10);
	fxtoli(s, li);
}

void fxtolf(char* s, double* lf)
{
	int j;
	int len;
	double pow;

	len = strlen(s);
	*lf = 0;
	pow = 0.1d;

	for(j=FX_DENOM_10POW-1; j>=0; j--)
	{
		*lf = *lf + (pow * (double)(s[j]-'0'));
		pow = pow / 10.0d;
	}

	pow = 1;

	for(j=FX_DENOM_10POW; j<len; j++)
	{
		*lf = *lf + (pow * (double)(s[j]-'0'));
		pow = pow * 10;
	}
}

void fxrtolf(fx* x, double* lf)
{
	char s[DMD_MAX_PATH+1] = "";
	mp_toradix(&x->r, s, 10);
	fxtolf(s, lf);
}

void fxitolf(fx* x, double* lf)
{
	char s[DMD_MAX_PATH+1] = "";
	mp_toradix(&x->i, s, 10);
	fxtolf(s, lf);
}

void fxtoi(char* s, int* i)
{
	int j;
	int len;
	int pow;

	len = strlen(s);
	*j = 0;
	pow = 1;

	for(j=FX_DENOM_10POW; j<len; j++)
	{
		*i = *i + (pow * (int)(s[j]-'0'));
		pow = pow * 10;
	}
}

void fxrtoi(fx* x, int* i)
{
	char s[DMD_MAX_PATH+1] = "";
	mp_toradix(&x->r, s, 10);
	fxtoi(s, i);
}

void fxitoi(fx* x, int* i)
{
	char s[DMD_MAX_PATH+1] = "";
	mp_toradix(&x->i, s, 10);
	fxtoi(s, i);
}

void fxtof(char* s, float* f)
{
	int j;
	int len;
	float pow;

	len = strlen(s);
	*f = 0;
	pow = 0.1f;

	for(j=FX_DENOM_10POW-1; j>=0; j--)
	{
		*f = *f + (pow * (float)(s[j]-'0'));
		pow = pow / 10.0f;
	}

	pow = 1;

	for(j=FX_DENOM_10POW; j<len; j++)
	{
		*f = *f + (pow * (float)(s[j]-'0'));
		pow = pow * 10;
	}
}

void fxrtof(fx* x, float* f)
{
	char s[DMD_MAX_PATH+1] = "";
	mp_toradix(&x->r, s, 10);
	fxtof(s, f);
}	

void fxitof(fx* x, float* f)
{
	char s[DMD_MAX_PATH+1] = "";
	mp_toradix(&x->i, s, 10);
	fxtof(s, f);
}	

void fxabs(fx* xc, fx* xa)
{
}

void fxsqrt(fx* xc, fx* xa)
{
}

void fxsign(fx* xs, fx* xa)
{
}

void fxmin(fx* xc, fx* xa, fx* xb)
{
}

void fxmax(fx* xc, fx* xa, fx* xb)
{
}

void fxpow(fx* xc, fx* xb, fx* xp)
{
}

void fxfac(fx* xc, fx* xa);
{
}


#endif
#if 0

fx pow(fx x, fx y)
{
	//y must be whole integer
	fx i = ToFrac(0);
	fx r = x;

	while(i < y)
	{
		r = r * x;
		i = i + ToFrac(1);
	}

	return r;
}

fx fac(fx x)
{
	//x must be whole integer
	fx i = (x);
	fx r = ToFrac(1);

	while(i > 0)
	{
		r = r * i;
		i = i - ToFrac(1);
	}

	return r;
}

#if 01
INTBASIS igcd(INTBASIS num1, INTBASIS num2)
{
	INTBASIS tmp;
	//num1 = Abs(num1);
	//num2 = Abs(num2);
	num1 = (num1);
	num2 = (num2);
	while (num1 > INTBASIS(0)) {
		tmp = num1;
		num1 = num2 % num1;
		num2 = tmp;
	}
	return num2;
}
#endif

void reduce(fx *f)
{
#if 01
	INTBASIS gcd;
	gcd = igcd(f->n, f->d);
	f->n = f->n / gcd;
	f->d = f->d / gcd;
#endif
}
#if 01
fx fsign(fx f)
{
	fx s = INTBASIS(1);

	//if(f.d < 0)
	//	s = -s;
	//if(f.n < 0)
	//	s = -s;
	//if(f.neg)
	//	s = -s;
	s.neg = f.neg;
	//if(s < 0)
	//	s = -s;

	return s;
}
#endif
//DBL_DECIMAL_DIG=10
#if 1
fx ToFrac(double d)
{
	//if(d < 0)
	//exit(0);
		//infm("<","<");
#if 01
	fx f;
	//f.n = INTBASIS(d*(-1+2*(d>=0)) * RATIO_DENOM);
	if((d)<(0))
		f.neg = true;
#elif 0
	fx f = d;
#endif
	f.n = INTBASIS( ((f.neg)?-1:1)*d * 10000000000);
	f.d = INTBASIS(10000000000);
	return f;
}
#endif
fx ToFrac(int64_t d)
{
#if 0
	fx f;
	f.n = INTBASIS(d*(-1+2*(d>=0)) * RATIO_DENOM);
	if((d)<(0))
		f.neg = true;
#else
	fx f;
	if(d<0)
		f.neg = true;
	f.n = INTBASIS( ((f.neg)?-1:1)*d );
	//fx f = d;
#endif
	//f.n = d * 10000000000;
	//f.d = 10000000000;
	return f;
}

fx ToFrac(int d)
{
	fx f;
	if(d<0)
		f.neg = true;
	f.n = INTBASIS( ((f.neg)?-1:1)*d );
	return f;
}
#if 1
fx ToFrac(INTBASIS d)
{
	fx f;
	//f.n = d * INTBASIS(RATIO_DENOM);
	f.n = d;
	f.d = INTBASIS(1);
	f.neg = false;
	//if(fsign(d)<0)
	//	f.neg = true;
	//f.n = d * 10000000000;
	//f.d = 10000000000;
	return f;
}
#endif
fx Abs(fx f)
{
	//if(f.n < 0)
	//	f.n = -f.n;
	//if(f.d < 0)
	//	f.d = -f.d;
	//f.n = INTBASIS(1);
	f.neg = false;
	//if(f < 0)
	//	f = -f;
	return f;
}

int64_t ToBasis(fx f)
{
	//removes n/0 possibility
	//f = simplify(f);
	//return (int64_t)((f.n / INTBASIS(RATIO_DENOM))/*.toDouble()*/) * (-1+2*(int)f.neg);
	return ((f.neg?-1:1)*f.n.toDouble()/f.d.toDouble());
	//return f;
}

//preserves fractional
double ToBasis2(fx f)
{
	return ((f.neg?-1:1)*f.n.toDouble()/f.d.toDouble());
	//return (double)f.n/*.toDouble()*/ / (double)RATIO_DENOM * (-1+2*(int)f.neg);
}

fx llmin(fx a, fx b)
{
#ifdef PLAT_WIN
	return enmin(a,b);
#else
	return std::min(a,b);
#endif
	//return (a.n<b.n)?(((int)a.neg>(int)b.neg)?a:b):(((int)b.neg>(int)a.neg)?a:b);
}

fx llmax(fx a, fx b)
{
#ifdef PLAT_WIN
	return enmax(a,b);
#else
	return std::max(a,b);
#endif
	//return (a.n>b.n)?(((int)a.neg<(int)b.neg)?a:b):(((int)b.neg<(int)a.neg)?a:b);
}

//http://stackoverflow.com/questions/19611198/finding-square-root-without-using-sqrt-function
#if 01
fx sqrtf(fx num)
{
#if 1
	fx lower_bound=0; 
	fx upper_bound=num;
	fx temp=0;                    /* ek edited this line */

	int nCount = 5000;

	while(nCount != 0)
	{
		temp=(lower_bound+upper_bound)/2;
		if(temp*temp==num)
		{
			return temp;
		}
		else if(temp*temp > num)
		{
			upper_bound = temp;
		}
		else
		{
			lower_bound = temp;
		}
		nCount--;
	}
	return temp;
#else
	return ToFrac(isqrt(ToBasis(num)));
#endif
}
#endif
#if 0
fx sqrtf(fx x) {
    if (x <= 0)
        return 0;       // if negative number throw an exception?
    int exp = 0;
    x = frexp(x, &exp); // extract binary exponent from x
    if (exp & 1) {      // we want exponent to be even
        exp--;
        x *= 2;
    }
    fx y = (1+x)/2; // first approximation
    fx z = 0;
    while (y != z) {    // yes, we CAN compare fxs here!
        z = y;
        y = (y + x/y) / 2;
    }
    return ldexp(y, exp/2); // multiply answer by 2^(exp/2)
}
#endif

#if 0
//https://people.freebsd.org/~jake/frexp.c
/*
 * Split the given value into a fx in the range [0.5, 1.0) and
 * an exponent, such that frac * (2^exp) == value.  If value is 0,
 * return 0.
 */
fx
frexp(fx value, int* eptr)
{
	union {
                fx v;
		struct ieee_double s;
	} u;

	if (value) {
		/*
		 * fxs in [0.5..1.0) have an exponent of 2^-1.
		 * Leave Inf and NaN alone, however.
		 * WHAT ABOUT DENORMS?
		 */
		u.v = value;
		//if (u.s.dbl_exp != DBL_EXP_INFNAN)
		{
			//DBL_EXP_BIAS=1023
			*eptr = u.s.dbl_exp - (1023 - 1);
			u.s.dbl_exp = 1023 - 1;
		}
		return (u.v);
	} else {
		*eptr = 0;
		return (0.0);
	}
}
#endif

#if 01
fx simplify(fx f)
{
	if(f.d == INTBASIS(0))
	{
		f.d = INTBASIS(1);
		//if(f.n > INTBASIS(0))
		//	f.n = BIGN-1;
		//else
		//	f.n = -BIGN+1;
		return f;
	}
	reduce(&f);
	while( (Abs(f.n) > BIGN || Abs(f.d) > BIGN) && 
		(Abs(f.n) > 2 && Abs(f.d) > 2) )
	{
		f.n >>= 1;
		f.d >>= 1;
	}
	return f;	
}
#endif

#if 01
bool _isnan(fx f)
{
	return false;
}
#endif

	//fx::fx(fx f)
	//{
	//	*this = f;
	//}
	#if 01
	fx::fx(int64_t i)
	{
		*this = ToFrac((i));
	}
#if 1
	fx::fx(INTBASIS i)
	{
		*this = ToFrac(i);
	}
#endif
	fx::fx(double i)
	{
		*this = ToFrac(i);
	}
	fx::fx(int i)
	{
		neg = (i<0)?true:false;
		n = INTBASIS(i);
		d = INTBASIS(1);
	}
	bool fx::operator==(const fx b) const
	{
		//return (n == b.n && d == b.d);
		return (n == b.n && neg == b.neg) || (n==INTBASIS(0) && b.n == INTBASIS(0));
	}
	bool fx::operator!=(const fx b) const
	{
		//return (n != b.n || d != b.d);
		return (n != b.n || ( n==b.n && n!=INTBASIS(0) && neg!=b.neg));
	}
	
	bool fx::operator>(const fx b) const
	{
#if 0
		fx a2 = simplify(*this);
		fx b2 = simplify(b);
		INTBASIS neara = a2.n/a2.d;
		INTBASIS nearb = b2.n/b2.d;
		INTBASIS mul = 2;
		while(neara == 0 && nearb == 0 && mul < BIGN)
		{
			fx ch;
			ch.n = mul;
			ch.d = 1;
			ch = simplify(ch);
			a2 = a2 * ch;
			b2 = b2 * ch;
			neara = a2.n/a2.d;
			nearb = b2.n/b2.d;
			mul <<= 1;
		}
		return (neara > nearb);
#else
		return (n>(b.n)&&!neg&&!b.neg) ||
			(neg&&b.neg&&n<b.n) ||
			(!neg&&b.neg);
		//return (llmin(*this,b)==b);
		//return (this->n > b.n);
#endif
	}
	
	bool fx::operator>=(const fx b) const
	{
		//return (*this > b || *this == b);
		//return (this->n >= b.n);
		return ((*this)>b)||(*this)==b;
	}
	
	bool fx::operator<(const fx b) const
	{
#if 0
		fx a2 = simplify(*this);
		fx b2 = simplify(b);
		INTBASIS neara = a2.n/a2.d;
		INTBASIS nearb = b2.n/b2.d;
		INTBASIS mul = 2;
		while(neara == 0 && nearb == 0 && mul < BIGN)
		{
			fx ch;
			ch.n = mul;
			ch.d = 1;
			ch = simplify(ch);
			a2 = a2 * ch;
			b2 = b2 * ch;
			neara = a2.n/a2.d;
			nearb = b2.n/b2.d;
			mul <<= 1;
		}
		return (neara < nearb);
#else
		//return (this->n < b.n);
		//return (llmax(*this,b)==b);
		return (n<(b.n)&&!neg&&!b.neg) ||
			(neg&&b.neg&&n>b.n) ||
			(neg&&!b.neg);
#endif
	}

	bool fx::operator<=(const fx b) const
	{
		//return (*this < b || *this == b);
		//return (this->n <= b.n);
		return (*this)<b||(*this)==b;
	}

	fx operator*(const fx a, const fx b)
	{
#if 0
		fx a2 = simplify(*this);
		fx b2 = simplify(b);
		fx c;
		c.n = b2.n * a2.n;
		c.d = b2.d * a2.d;
		return simplify(c);
#endif	//TODO all these:
		fx r;
		r.n = a.n * b.n / INTBASIS(RATIO_DENOM);
		r.d = a.d * b.d;
		r.neg = (a.neg != b.neg);
		return r;
	}
	//http://stackoverflow.com/questions/32199574/error-c2678-binary-no-operator-found-which-takes-a-left-hand-operand-of
	fx operator*(const fx a, const double b)
	{
		fx r;
		r.n = INTBASIS( a.n.toDouble() * b / INTBASIS(RATIO_DENOM).toDouble() );
		r.d = INTBASIS( a.d.toDouble() * b );
		r.neg = (a.neg != (b<0));
		return r;
		//return (*this)*fx(b);
	}
	fx operator/(const fx a, const fx b)
	{
#if 0
		fx a2 = simplify(*this);
		fx b2 = simplify(b);
		fx c;
		c.n = b2.d * a2.n;
		c.d = b2.n * a2.d;
		return simplify(c);
#endif
		fx r;
		if(b.n == INTBASIS(0) || a.n == INTBASIS(0))
			//r.n = BIGN * fsign(r.n);
			r.n = INTBASIS(0);
		else
			r.n = a.n * RATIO_DENOM / b.n;
		r.d = a.d * RATIO_DENOM / b.d;
		r.neg = (a.neg != b.neg);
		return r;
	}
	//b%dv = b-(b/dv)*dv
	//better to define b%dv = (0>=|b|>dv)
	//while(|b|>=dv) { if(b>=0) b-=dv else b+=dv }	dv>0
	fx fx::operator%(const fx b) const
	{
#if 0
		fx a2 = simplify(*this);
		fx b2 = simplify(Abs(b));
		//fx c = a2;
		fx z;
		z.n = 0;
		z.d = 1;
		while(Abs(a2)>=b2)
		{
			if(b2>=z)
				a2 = a2 - b2;
			else
				a2 = a2 + b2;
		}
		return simplify(a2);
#else
		fx a2 = *this;	//simplify(*this);
		fx b2 = Abs(b);	//simplify(Abs(b));
		//fx c = a2;
		fx z;
		z.n = INTBASIS(0);
		//z.d = 1;
		while(Abs(a2)>=b2)
		{
			if(b2>=z)
				a2 = a2 - b2;
			else
				a2 = a2 + b2;
		}
		return a2;
		//return simplify(a2);
#endif
	}
	fx fx::operator-(const fx b) const
	{
#if 0
		fx a2 = simplify(*this);
		fx b2 = simplify(b);
		fx c;
		c.n = (b2.d * a2.n) - (a2.d * b2.n);
		c.d = b2.d * a2.d;
		return simplify(c);
#endif
		fx r;
		//r.n = n - b.n;

#if 0
		if(*this > b)
		{
			if(this->neg)
			{
				if(b.neg)
				{
					r.n = this->n + b.n;
					r.neg = true;
				}
				else
				{
					//this->n = -this->n;
					r.n = -(this->n) + b.n;
					r.neg = true;
					//this->n = -this->n;
				}
			}
			else
			{
				if(b.neg)
				{
					//b.n = -b.n;
					r.neg = false;
					r.n = this->n + -(b.n);
				}
				else
				{
					r.neg = false;
					r.n = this->n + b.n;
				}
			}	
		}
		else	//b>this
		{
			if(this->neg)
			{
				if(b.neg)
				{
					r.n = -(n) + b.n;
					r.neg = true;
				}
				else
				{
					
					r.neg = false;
				}
			}
		}

		int agreater = (*this>b);
		int aneg = (-1+2*neg);
		int bneg = (-1+2*b.neg);
#endif
		//-3 - -1 = -2
		//-3 - +1 = -4
		//+3 - -1 = +4
		//+3 - +1 = +2
		//-1 - -3 = +2
		//-1 - +3 = -4
		//+1 - -3 = +4
		//+1 - +3 = -2
		//r.neg = (Abs(b)>Abs(*this)&&(!b.neg)||((b.neg)&&(Abs(*this)>Abs(b)))||
		//	(this->neg&&());
		r.neg = (this->neg&&((this->n)>(b.n))) || (!b.neg&&((b.n)>(this->n)));

		fx aadd = *this;
		fx badd = b;

		//if(neg&&(this->n<b.n))
		if(neg)
			aadd.n = -aadd.n;
		//if(b.neg&&(b.n<this->n))
		if(b.neg)
			badd.n = -badd.n;
		badd.n = -badd.n;

		r.n = aadd.n+badd.n;

		if(r.neg)
			r.n = -r.n;
		return r;
		return r;



	}
	fx operator-(const fx a)
	{
		return (fx(0)-a);
	}
	fx fx::operator+(const fx b) const
	{
#if 0
		fx a2 = simplify(*this);
		fx b2 = simplify(b);
		fx c;
		c.n = (b2.d * a2.n) + (a2.d * b2.n);
		c.d = b2.d * a2.d;
		return simplify(c);
#endif
		fx r;
		//r.n = n + b.n;


		//-3 + -1 = -4
		//-3 + +1 = -2
		//+3 + -1 = +2
		//+3 + +1 = +4
		//-1 + -3 = -4
		//-1 + +3 = +2
		//+1 + -3 = -2
		//+1 + +3 = +4

		//r.neg = (neg&&b.neg)||(this->neg&&(this->n>b.n))||(b.neg&&(b.n>this->n));
		r.neg = (neg&&b.neg)||(neg&&(this->n)>(b.n))||(b.neg&&(b.n)>(this->n));

		fx aadd = *this;
		fx badd = b;

		//if(neg&&(this->n<b.n))
		if(neg)
			aadd.n = -aadd.n;
		//if(b.neg&&(b.n<this->n))
		if(b.neg)
			badd.n = -badd.n;

		r.n = aadd.n+badd.n;

		if(r.neg)
			r.n = -r.n;
		return r;
	}

	fx fx::operator+=( const fx b)
	{
	//	return (*this) = (*this)+(b);
		fx r;
		r = *this + b;
		return (*this) = r;
	}
	fx fx::operator*=( const fx b)
	{
		return (*this) = (*this)*(b);
	}
	fx fx::operator*=( const double b)
	{
		return (*this) = (*this)*ToFrac(b);
	}
	
	fx fx::operator/=( const fx b)
	{
		return (*this) = (*this)/(b);
	}
#endif

#endif