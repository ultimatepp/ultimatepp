#include <Core/Core.h>

using namespace Upp;

bool Add(word *target, const word *s1, const word *s2, int len)
{
	int carry = 0;
	while(--len >= 0) {
		int x = s1[len] + s2[len] + carry;
		if(x > 1000) {
			carry = 1;
			x -= 1000;
		}
		target[len] = x;
	}
	return carry;
}

bool Sub(word *target, const word *s1, const word *s2, int len)
{
	int borrow = 0;
	while(--len >= 0) {
		int x = s1[len] - s2[len] - borrow;
		if(x < 0) {
			borrow = 1;
			x += 1000;
		}
		target[len] = x;
	}
	return borrow;
}

bool Mul(word *target

CONSOLE_APP_MAIN {
	word a[2];
	word b[2];
	word c[2];
	
	for(int i = 0; i < 1000; i++) {
		int x = Random(500000);
		int y = Random(500000);
		a[0] = x / 1000;
		a[1] = x % 1000;
		b[0] = y / 1000;
		b[1] = y % 1000;
		Add(c, a, b, 2);
		LOG(x << "+" << y << "=" << x + y << " " << c[0] << c[1]);
		if(x > y) {
			Sub(c, a, b, 2);
			LOG(x << "-" << y << "=" << x - y << " " << c[0] << c[1]);
		}
	}
}
