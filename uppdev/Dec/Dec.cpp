#include <Core/Core.h>

using namespace Upp;

typedef __int128_t int128;

String Raw(int128 x);
String Format128(int128 a);

double pow2(int x)
{
	double b = 1;
	while(x--)
		b *= 2;
	return b;
}

inline byte& MSB(int128& data)
{
#ifdef CPU_LE
	byte *h = (byte *)&data + 15
	;
#else
	byte *h = (byte *)&data;
#endif
}

String Format128(int128 a)
{
	RTIMING("Format128");

	if(a < 0)
		a = -a;
	char b[50];
	char *p = b + 50;
	do {
		*--p = a % 10 + '0';
		a = a / 10;
	}
	while(a);
	return String(p, b + 50);
}

inline byte FetchPrec(int128& data)
{
	byte& msb = MSB(data);
	byte p = msb >> 4;
	msb &= 15;
	return p;
}

inline void SetPrec(int128& data, byte b)
{
	byte& msb = MSB(data);
	msb = (msb & 15) | (b << 4);
}

String Raw(int128 x)
{
	FetchPrec(x);
	return Format128(x);
}

struct Decimal {
	int128 data;
	
	static int128 e10[38];
	
public:
	void Add(Decimal b) {
		byte sa = FetchPrec(data);
		byte sb = FetchPrec(b.data);
		data += b.data;
		SetPrec(data, max(sa, sb));
	}
	void Mul(Decimal b) {
		byte sa = FetchPrec(data);
		byte sb = FetchPrec(b.data);
/*		DUMP(Raw(data));
		DUMP(Raw(b.data));
		DUMP(Raw(data * b.data));*/
//		DUMP(Format128(data * b.data));
//		DUMP(Raw(data * b.data / e10[16]));
		data = data * b.data / e10[16];
		SetPrec(data, min(sa + sb, 16));
	}
	
	String ToString() const;
	const char *Scan(const char *s);
	
	Decimal();
};

int128 Decimal::e10[38];

Decimal::Decimal()
{
	static bool init;
	if(!init) {
		int128 q = 1;
		for(int i = 0; i < 38; i++) {
			e10[i] = q;
			q = q * 10;
		}
		DUMP(Raw(e10[16]));
		init = true;
	}
	data = 0;
}

const char *Decimal::Scan(const char *s)
{
	while(*s == ' ')
		s++;
	bool neg = false;
	if(*s == '-') {
		neg = true;
		s++;
	}
	while(*s == ' ')
		s++;
	if(!IsDigit(*s))
		return NULL;
	data = 0;
	while(IsDigit(*s))
		data = *s++ - '0' + 10 * data;
	int digits = 0;
	if(*s == '.') {
		s++;
		while(IsDigit(*s) && digits < 16) {
			data = *s++ - '0' + 10 * data;
			digits++;
		}
		while(IsDigit(*s)) s++;
	}
	data *= e10[16 - digits];
	if(neg)
		data = -data;
	SetPrec(data, digits);
	return s;
}

String Decimal::ToString() const
{
	int128 d = data;
	int digits = FetchPrec(d);
	String x = Format128(d);
//	DUMP(x);
	int n = x.GetLength();
	if(digits == 0) {
		if(n < 16)
			return "0";
		return x.Mid(0, n - 16);
	}
	if(n <= 16)
		return "0." + String('0', 16 - n) + x.Mid(0, digits);
	return x.Mid(0, n - 16) + "." + x.Mid(n - 16, digits);
}

Decimal Dec(const char *s)
{
	Decimal d;
	d.Scan(s);
	return d;
}

inline Decimal operator+(const Decimal& a, const Decimal& b)
{
	Decimal r = a;
	r.Add(b);
	return r;
}

inline Decimal operator*(const Decimal& a, const Decimal& b)
{
	Decimal r = a;
	r.Mul(b);
	return r;
}

CONSOLE_APP_MAIN
{
	{
		int128 x = 10;
		for(int i = 0; i < 50; i++) {
			x = 10 * x;
			MSB(x) = 0;
			DUMP(Format128(x));
//			DUMP(Raw(x));
		}
	}
	DUMP(Dec("1.20"));
	DUMP(Dec("1.20") + Dec("2"));	
	DUMP(Dec("1.02") + Dec("1.005"));
	DUMP(Dec("1.02") * Dec("1"));
	DUMP(Dec("2") * Dec("2"));
	DUMP(Dec("2") * Dec("2.00"));
	DUMP(Dec("1.02") * Dec("0.50"));
	DUMP(Dec("1.02") * Dec("1.005"));
	Decimal x = Dec("1");
	for(int i = 0; i < 100; i++) {
//		DUMP(Raw(x.data));
		DUMP(x);
		x = x * Dec("0.1");
	}
}
