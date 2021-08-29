#include "Core.h"

namespace Upp {

// iscale: computes x * y / z.

int iscale(int x, int y, int z)
{
#if CPU_32 && COMPILER_MSC
	__asm {
		mov		eax, [x]
		imul	[y]
		idiv	[z]
	}
#else
	int64 res = x;
	res *= y;
	res /= z;
	return (int)res;
#endif
}

// iscalefloor: computes x * y / z, rounded towards -infty.

int iscalefloor(int x, int y, int z)
{
#if CPU_32 && COMPILER_MSC
	__asm {
		mov		eax, [x]
		imul	[y]
		idiv	[z]
		and		edx, edx
		jge		__1
		dec		eax
	__1:
	}
#else
	int64 res = x;
	int64 mulres = res * y;
	res = mulres / z;
	if(res * z != mulres)
		res--;
	return (int)res;
#endif
}

// iscaleceil: computes x * y / z, rounded towards +infty.

int iscaleceil(int x, int y, int z)
{
#if CPU_32 && COMPILER_MSC
	__asm {
		mov		eax, [x]
		imul	[y]
		idiv	[z]
		and		edx, edx
		jle		__1
		inc		eax
	__1:
	}
#else
	int64 res = x;
	int64 mulres = res * y;
	res = mulres / z;
	if(res * z != mulres)
		res++;
	return (int)res;
#endif
}

#ifdef COMPILER_MSC
#pragma warning(default: 4035)
#endif

// idivfloor: returns x / y, truncated towards -infinity.

int idivfloor(int x, int y)
{
	return x / y - (x % y < 0);
}

// idivceil: returns x / y, truncated towards +infinity.

int idivceil(int x, int y)
{
	return x / y + (x % y > 0);
}

// itimesfloor: moves x to nearest lower multiple of y.

int itimesfloor(int x, int y)
{
	return y * idivfloor(x, y);
}

// itimesceil: moves x to nearest higher multiple of y.

int itimesceil(int x, int y)
{
	return y * idivceil(x, y);
}

// fround: converts double to nearest integer. Checks integer limits.

int fround(double value)
{
	return value >= INT_MAX ? INT_MAX : value <= INT_MIN ? INT_MIN
		: (int)floor(value + 0.5);
}

// ffloor: converts double to integer rounding towards -infinity. Checks integer limits.

int ffloor(double value)
{
	return value >= INT_MAX ? INT_MAX : value <= INT_MIN ? INT_MIN
		: (int)floor(value);
}

// fceil: converts double to integer rounding towards +infinity. Checks integer limits.

int fceil(double value)
{
	return value >= INT_MAX ? INT_MAX : value <= INT_MIN ? INT_MIN
		: (int)ceil(value);
}

// fround: converts double to nearest integer. Checks integer limits.

int64 fround64(double value)
{
	return value >= (double)INT64_MAX ? INT64_MAX : value <= (double)INT64_MIN ? INT64_MIN
		: (int64)floor(value + 0.5);
}

// ffloor: converts double to integer rounding towards -infinity. Checks integer limits.

int64 ffloor64(double value)
{
	return value >= (double)INT64_MAX ? INT64_MAX : value <= (double)INT64_MIN ? INT64_MIN
		: (int64)floor(value);
}

// fceil: converts double to integer rounding towards +infinity. Checks integer limits.

int64 fceil64(double value)
{
	return value >= (double)INT64_MAX ? INT64_MAX : value <= (double)INT64_MIN ? INT64_MIN
		: (int64)ceil(value);
}

// asString: converts double numbers to strings.

String AsString(double x, int digits)
{
	return FormatG(x, digits);
}

// modulo: working version of math function `fmod'.

double modulo(double x, double y)
{
	return y ? (x - y * floor(x / y)) : x;
}

int ilog10(double d)
{
	if(IsNull(d) || d == 0)
		return Null;
	return fceil(log10(fabs(d)));
}

double ipow10(int i)
{
	static const double pow_tbl[] =
	{
		1e00,
		1e01, 1e02, 1e03, 1e04, 1e05, 1e06, 1e07, 1e08, 1e09, 1e10,
		1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20,
		1e21, 1e22, 1e23, 1e24, 1e25, 1e26, 1e27, 1e28, 1e29, 1e30,
		1e31, 1e32, 1e33, 1e34, 1e35, 1e36, 1e37, 1e38, 1e39, 1e40,
		1e41, 1e42, 1e43, 1e44, 1e45, 1e46, 1e47, 1e48, 1e49, 1e50,
	};
	if(i >= 0 && i < __countof(pow_tbl))
		return pow_tbl[i];
	else if(i < 0 && i > -__countof(pow_tbl))
		return 1 / pow_tbl[-i];
	return pow(10.0, (double)i);
}

double normalize(double d, int& exp)
{
	if(IsNull(d) || d == 0)
	{
		exp = Null;
		return d;
	}
	bool sign = (d < 0);
	if(sign) d = -d;
	exp = minmax<int>(ilog10(d), -308, +308); // 8-byte double!
	d /= ipow10(exp);
	if(d >= 10) { d /= 10; exp++; }
	if(d < 1)   { d *= 10; exp--; }
	return sign ? -d : d;
}

double roundr(double d, int digits)
{
	int i = ilog10(d);
	if(IsNull(i))
		return d;
	double fac = ipow10(i - digits);
	return floor(d / fac + 0.5) * fac;
}

double floorr(double d, int digits)
{
	int i = ilog10(d);
	if(IsNull(i))
		return d;
	double fac = ipow10(i - digits);
	return floor(d / fac) * fac;
}

double ceilr(double d, int digits)
{
	int i = ilog10(d);
	if(IsNull(i))
		return d;
	double fac = ipow10(i - digits);
	return ceil(d / fac) * fac;
}

int SolveQuadraticEquation(double a, double b, double c, double *r)
{
	if(a == 0) {
		if(b == 0)
			return 0;
		r[0] = r[1] = -c / b;
		return 1;
    }
	double d = b * b - 4 * a * c;
	if (d < 0)
		return 0;
	a *= 2;
	if (d == 0) {
		r[0] = r[1] = -b / a;
		return 1;
	}
	d = sqrt(d);
	if(b == 0) {
		r[0] = d / a;
		r[1] = -r[0];
    }
	else {
		r[0] = (-b + d) / a;
		r[1] = (-b - d) / a;
    }
	return 2;
}

}
