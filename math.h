/* date = June 29th 2026 2:09 pm */

#ifndef MATH_H
#define MATH_H
#include "./math/costable_0_01.h"
#include "./math/sintable_0_01.h"
#define m_modd(x, y) ((x) - (int)((x) / (y)) * (y))
#define CONST_2PI 6.28318530718

#define V2(a, b) (v2){.x=a,.y=b}
typedef struct v2 v2;
struct v2 {
	float x,y;
};

double 
m_absd(double a) {
	union {
		double d;
		unsigned long u;
	} conv;
	conv.d = a;
	conv.u &= ~(1UL << 63);
	return conv.d;
}

double 
m_cos_table_0_01(double x)
{
	x = m_absd(x);
	x = m_modd(x, CONST_2PI);
	return costable_0_01[(int)(x * 100 + 0.5)];
}

double 
m_sin_table_0_01(double x)
{
	x = m_absd(x);
	x = m_modd(x, CONST_2PI);
	return sintable_0_01[(int)(x * 100 + 0.5)];
}

int32_t
m_abs(int32_t a)
{
	return (a > 0) ? a : -a;
}

int64_t
m_pow32(int32_t mantissa, int32_t exp)
{
	int64_t ans = 1;
	for(int32_t i = 0; i < exp; i++)
	{
		ans *= mantissa;
	}
	return ans;
}

#endif //MATH_H
