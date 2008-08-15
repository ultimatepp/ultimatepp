#include <Core/Core.h>

using namespace Upp;

inline int UnSgn(int64& c)
{
	if(c < 0) {
		c = -c;
		return -1;
	}
	return 1;
}

int64 F2Div100(int64 c)
{
	int sgn = UnSgn(c);
	int64 rm = c % 100;
	c /= 100;
	if(rm >= 50)
		c++;
	return sgn * c;
}

int64 F2Div10000(int64 c)
{
	int sgn = UnSgn(c);
	int64 rm = c % 10000;
	c /= 10000;
	if(rm >= 5000)
		c++;
	return sgn * c;
}

int64 F2Mul(int64 a, int64 b)
{
	return F2Div100(a * b);
}

int64 F2MulPercent(int64 a, int64 b)
{
	int64 sgn = UnSgn(a) * UnSgn(b);
	int64 c = a * b;
	int64 rm = c % 10000;
	c /= 10000;
	if(rm >= 5000)
		c++;
	return sgn * c;
}

int64 F2Div(int64 a, int64 b)
{
	int64 sgn = UnSgn(a) * UnSgn(b);
	int64 c = a * 10000 / b;
	int64 rm = c % 100;
	c /= 100;
	if(rm >= 50)
		c++;
	return sgn * c;
}

CONSOLE_APP_MAIN
{
	DUMP(-380 % 100);
	DUMP(-380 / 100);
	DUMP(F2Div100(-380));
	DUMP(F2Div100(-340));
	DUMP(F2Div100(380));
	DUMP(F2Div100(340));

	return;	
	int tax = 700;
	for(int i = 0; i < 10000; i++) {
		int64 netto = F2Netto(i, tax);
		LOG(F2Format(i) << " netto: " << F2Format(netto) << " brutto: "
		    << F2Format(netto + F2MulPercent(netto, tax)));
	}
}
