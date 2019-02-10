#include "Core.h"
	
namespace Upp {

void RGBtoHSV(double r, double g, double b, double& h, double& s, double& v)
{
	double delta;
	if((v = max(r, max(g, b))) == 0 || (delta = v - min(r, min(g, b))) == 0)
	{
		h = s = 0;
		return;
	}
	s = delta / v;
	delta *= 6;
	if(g == v)
		h = 1 / 3.0 + (b - r) / delta;
	else if(b == v)
		h = 2 / 3.0 + (r - g) / delta;
	else
		if((h = (g - b) / delta) < 0)
			h += 1;
}

void HSVtoRGB(double h, double s, double v, double& r, double& g, double& b)
{
	if(s == 0)
	{
		r = g = b = v;
		return;
	}
	double rem = fmod(h *= 6, 1);
	double p = v * (1 - s);
	double q = v * (1 - s * rem);
	double t = v * (1 - s * (1 - rem));
	switch(ffloor(h))
	{
	default: NEVER(); // invalid color!
	case 6:
	case 0: r = v; g = t; b = p; break;
	case 1: r = q; g = v; b = p; break;
	case 2: r = p; g = v; b = t; break;
	case 3: r = p; g = q; b = v; break;
	case 4: r = t; g = p; b = v; break;
	case -1:
	case 5: r = v; g = p; b = q; break;
	}
}

Color HsvColorf(double h, double s, double v)
{
	double r, g, b;
	HSVtoRGB(h, s, v, r, g, b);
	return Color(min(int(r * 255), 255), min(int(g * 255), 255), min(int(b * 255), 255));
}

dword Color::Get() const
{
	if(IsNullInstance()) return 0;
	dword c = color;
	return c & 0xffffff;
}

template <>
String AsString(const RGBA& c)
{
	return Format("RGBA(%d, %d, %d, %d)", (int)c.r, (int)c.g, (int)c.b, (int)c.a);
}

Color::operator RGBA() const
{
	RGBA color;
	if(IsNullInstance())
		Zero(color);
	else {
		color.r = GetR();
		color.g = GetG();
		color.b = GetB();
		color.a = 255;
	}
	return color;
}

Color::Color(RGBA rgba)
{
	if(rgba.a == 0)
		color = 0xffffffff;
	else {
		if(rgba.a == 255)
			color = RGB(rgba.r, rgba.g, rgba.b);
		else {
			int alpha = 65536 / rgba.a;
			color = RGB((alpha * rgba.r) >> 8, (alpha * rgba.g) >> 8, (alpha * rgba.b) >> 8);
		}
	}
}

void Color::Jsonize(JsonIO& jio)
{
	int r, g, b;
	if(IsNullInstance()) {
		r = g = b = Null;
	}
	else {
		r = GetR();
		g = GetG();
		b = GetB();
	}
	jio("red", r)("green", g)("blue", b);
	if(IsNull(r))
		*this = Null;
	else
		*this = Color(r, g, b);
}

void Color::Xmlize(XmlIO& xio)
{
	int r, g, b;
	if(IsNullInstance()) {
		r = g = b = Null;
	}
	else {
		r = GetR();
		g = GetG();
		b = GetB();
	}
	xio
		.Attr("red", r)
		.Attr("green", g)
		.Attr("blue", b)
	;
	if(IsNull(r))
		*this = Null;
	else
		*this = Color(r, g, b);	
}

RGBA operator*(int alpha, Color c)
{
	RGBA r;
	r.a = alpha;
	alpha += (alpha >> 7);
	r.r = (alpha * c.GetR()) >> 8;
	r.g = (alpha * c.GetG()) >> 8;
	r.b = (alpha * c.GetB()) >> 8;
	return r;
}

template<>
String AsString(const Color& c) {
	if(IsNull(c))
		return "Color(Null)";
	if(c.GetRaw() & 0x80000000)
		return Format("Color(%d, 0)", int(c.GetRaw() & ~0x80000000));
	return Format("Color(%d, %d, %d)", c.GetR(), c.GetG(), c.GetB());
}

String ColorToHtml(Color color)
{
	return IsNull(color) ? Null : Format("#%02X%02X%02X",  color.GetR(), color.GetG(), color.GetB());
}

static int sCharFilterNoDigit(int c)
{
	return IsDigit(c) ? 0 : c;
}

static int sCharFilterHex(int c)
{
	return c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F' || IsDigit(c) ? c : 0;
}

Color ColorFromText(const char *s)
{
	Vector<String> h = Split(s, sCharFilterNoDigit);
	if(h.GetCount() == 3 && (strchr(s, ',') || strchr(s, ';') || strchr(s, '.') || strchr(s, ' '))) {
		int r = atoi(h[0]);
		int g = atoi(h[1]);
		int b = atoi(h[2]);
		if(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255)
			return Color(r, g, b);
	}
	String hex = Filter(s, sCharFilterHex);
	if(hex.GetCount() == 6 || hex.GetCount() == 8) {
		dword w = (dword)ScanInt64(~hex, NULL, 16);
		return Color(byte(w >> 16), byte(w >> 8), byte(w));
	}
	return Null;
}

Color Blend(Color c1, Color c2, int alpha)
{
	int a = (alpha >> 7) + alpha;
	return Color(min(((a * (c2.GetR() - c1.GetR())) >> 8) + c1.GetR(), 255),
	             min(((a * (c2.GetG() - c1.GetG())) >> 8) + c1.GetG(), 255),
	             min(((a * (c2.GetB() - c1.GetB())) >> 8) + c1.GetB(), 255));
}

INITBLOCK {
	Value::SvoRegister<Color>("Color");
}

int  Grayscale(const Color& c)
{
	return (77 * c.GetR() + 151 * c.GetG() + 28 * c.GetB()) >> 8;
}

bool IsDark(Color c)
{
	return Grayscale(c) < 80;
}

bool IsLight(Color c)
{
	return Grayscale(c) > 255 - 80;
}

int  Grayscale2(const Color& c)
{
	return (c.GetR() + c.GetG() + c.GetB()) / 3;
}

Color DarkTheme(Color c)
{
	if(IsNull(c))
		return Null;
	
	DLOG("=====");
	DDUMP(c);
	
#if 1
	int v[3];
	v[0] = c.GetR();
	v[1] = c.GetG();
	v[2] = c.GetB();

	int m = 3*256 - v[0] - v[1] - v[2];
	
	int i0 = 0;
	int i1 = 1;
	int i2 = 2;
	
	if(v[i0] > v[i1])
		Swap(i0, i1);
	if(v[i1] > v[i2])
		Swap(i1, i2);
	if(v[i0] > v[i1])
		Swap(i0, i1);
	
	DDUMP(v[i0]);
	DDUMP(v[i1]);
	DDUMP(v[i2]);
	DDUMP(m);

	if(m > 128) {
		int a = min(v[i2] + m / 3, 255) - v[i2];
		DDUMP(v[i2] + m);
		DDUMP(a);
		v[i0] += a;
		v[i1] += a;
		v[i2] += a;
		m -= 3 * a;

		a = min(v[i1] + m / 2, 255) - v[i1];
		DDUMP(m);
		DDUMP(a);
		v[i0] += a;
		v[i1] += a;
		m -= 2 * a;

		DDUMP(m);
		DDUMP(v[i0] - m);
		v[i0] = min(v[i0] + m, 255);
		DDUMP(v[i0]);
		DDUMP((v[i0] + v[i1] + v[i2]) / 3);
	}
	else {
		int a = max(v[i0] - m, 0) - v[i0];
		DDUMP(v[i0] + m);
		DDUMP(a);
		v[i0] += a;
		v[i1] += a;
		v[i2] += a;
		m += a;

		a = max(v[i1] - m, 0) - v[i1];
		DDUMP(m);
		DDUMP(a);
		v[i1] += a;
		v[i2] += a;
		m += 2 * a / 3;

		DDUMP(m);
		DDUMP(v[i2] - m);
		v[i2] = max(v[i2] - m, 0);
		DDUMP(v[i2]);
	}
	
	DDUMP(Color(v[0], v[1], v[2]));
	
	return Color(v[0], v[1], v[2]);
#else
	DDUMP(m);
	int m = 255 - (c.GetR() + c.GetG() + c.GetB()) / 3;
	
	int r = c.GetR();
	int g = c.GetG();
	int b = c.GetB();
	
	if(m < 128)
		while(Grayscale2(Color(r, g, b)) > m) {
			r = max(r - 1, 0);
			g = max(g - 1, 0);
			b = max(b - 1, 0);
		}
	else
		while(Grayscale2(Color(r, g, b)) < m - 1) {
			r = min(r + 1, 255);
			g = min(g + 1, 255);
			b = min(b + 1, 255);
		}
	
	DDUMP(Grayscale2(Color(r, g, b)));
	return Color(r, g, b);
#endif

/*	
	double h, s, v;
	const double m = 1/255.0;
#if 0
	RGBtoHSV(m * c.GetR(), m * c.GetG(), m * c.GetB(), h, s, v);
	return HsvColorf(h, s, (1 + s * 0.5) - v);
#endif
	RGBtoHSV(m * c.GetR(), m * c.GetG(), m * c.GetB(), h, s, v);
	v = m * Grayscale(c);
	return HsvColorf(h, 1 - v, 1 - v);
*/
}

Color DarkThemeCached(Color c)
{
	const int N = 8;
	thread_local struct {
		Color icolor[N];
		Color ocolor[N];
		int   ii = 0;
	} cache;
	ONCELOCK {
		for(int i = 0; i < N; i++) {
			cache.icolor[i] = Color(0, 0, 0);
			cache.ocolor[i] = Color(255, 255, 255);
		}
	}
	#define DO(i) if(cache.icolor[i] == c) return cache.ocolor[i];
	DO(0); DO(1); DO(2); DO(3); DO(4); DO(5); DO(6); DO(7);
	cache.ii = (cache.ii + 1) & (N - 1);
	cache.icolor[cache.ii] = c;
	c = DarkTheme(c);
	cache.ocolor[cache.ii] = c;
	return c;
}

}
