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

void CMYKtoRGB(double c, double m, double y, double k, double& r, double& g, double& b)
{
	k = clamp(k, 0.0, 1.0);
	r = (1 - c) * (1 - k);
	g = (1 - m) * (1 - k);
	b = (1 - y) * (1 - k);
}

void RGBtoCMYK(double r, double g, double b, double& c, double& m, double& y, double& k)
{
	k = 1 - max(max(r, g), b);
	c = (1 - r - k) / (1 - k);
	m = (1 - g - k) / (1 - k);
	y = (1 - b - k) / (1 - k);
}

Color CmykColorf(double c, double m, double y, double k)
{
	double r, g, b;
	CMYKtoRGB(c, m, y, k, r, g, b);
	return Color(min(int(r * 255), 255), min(int(g * 255), 255), min(int(b * 255), 255));
}

// Formula from https://www.w3.org/TR/2008/REC-WCAG20-20081211/#relativeluminancedef

double RelativeLuminance(Color color) {
	auto comp = [&] (double c){
		c /= 255;
		return (c <= 0.03928) ? c / 12.92 : pow((c + 0.055) / 1.055, 2.4);
	};
	return comp(color.GetR()) * 0.2126 + comp(color.GetG()) * 0.7152 + comp(color.GetB()) * 0.0722;
}

// Formula from https://www.w3.org/TR/2008/REC-WCAG20-20081211/#contrast-ratiodef

double ContrastRatio(Color c1, Color c2) {
	double rl1 = RelativeLuminance(c1);
	double rl2 = RelativeLuminance(c2);
	return (max(rl1, rl2) + 0.05) / (min(rl1, rl2) + 0.05);
}

static const int s_Max = 1024;
static std::atomic<int> s_color_ii;
static std::atomic<dword> s_color[s_Max];
static Color (*s_color_fn[s_Max])();
static Mutex sColorMutex;

SColor::SColor(Color (*fn)())
{
	int ii = s_color_ii++;
	ASSERT(ii < s_Max); // number of SColors is limited
	ii = min(ii, s_Max - 1);
	s_color_fn[ii] = fn;
	if(fn) {
		Mutex::Lock __(sColorMutex);
		s_color[ii] = (*fn)();
	}
	color = ii | SCOLOR;
}

void SColor::Refresh()
{
	Mutex::Lock __(sColorMutex);
	int n = min((int)s_color_ii, s_Max - 1);
	for(int i = 0; i < n; i++)
		if(s_color_fn[i])
			s_color[i] = SCOLOR;
}

void SColor::Write(Color c, Color val)
{
	int ii = c.GetRaw() & VBITS;
	ASSERT((c.GetRaw() & SCOLOR) && ii >= 0 && ii < s_Max);
	if(ii >= 0 && ii < s_Max) {
		ASSERT(!s_color_fn[ii]);
		s_color[ii] = val.Resolved();
	}
}

bool AColor_dark_mode__;

dword Color::Get() const
{
	if(IsNullInstance()) return 0;
	if(color & SPECIAL)
		return 0;
	dword val = color & VBITS;
	if(color & ACOLOR) {
		Color c = FromRaw(val);
		if(AColor_dark_mode__)
			return DarkThemeCached(c).color;
		return val;
	}
	if(color & SCOLOR) {
		if(val < s_Max) {
			std::atomic<dword>& r = s_color[val];
			if(r & SCOLOR) {
				Mutex::Lock __(sColorMutex);
				if(s_color_fn[val])
					r = (*s_color_fn[val])() & VBITS;
				else
					r = 0;
			}
			return r;
		}
		return 0;
	}
	return color & VBITS;
}

String Color::ToString() const {
	if(IsNull(*this))
		return "Color(Null)";
	if(color & SCOLOR)
		return Format("SColor(%d) -> Color(%d, %d, %d)", int(color & VBITS), GetR(), GetG(), GetB());
	int ii = GetSpecial();
	if(ii >= 0)
		return Format("Color::Special(%d)", ii);
	return Format("Color(%d, %d, %d)", GetR(), GetG(), GetB());
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

Color Lerp(Color a, Color b, double t)
{
	auto Ch = [&](byte a, byte b) {
		return (byte)clamp(Lerp((double)a, (double)b, t), 0., 255.);
	};
	return Color(Ch(a.GetR(), b.GetR()), Ch(a.GetG(), b.GetG()), Ch(a.GetB(), b.GetB()));
}

INITBLOCK {
	Value::SvoRegister<Color>("Color");
}

int  Grayscale(const Color& c)
{
	return (77 * c.GetR() + 151 * c.GetG() + 28 * c.GetB()) >> 8;
}

double Difference(Color c1, Color c2)
{
	return 2.75 * abs(c2.GetR() - c1.GetR()) + 5.4 * abs(c2.GetG() - c1.GetG()) + abs(c2.GetB() - c1.GetB());
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

double C_R = 0.32;
double C_G = 0.5;
double C_B = 0.2;

Color DarkTheme(Color color)
{
	if(IsNull(color))
		return Null;

	double r = color.GetR();
	double g = color.GetG();
	double b = color.GetB();
	
	double saturation = min(r, g, b);
	
	r -= saturation;
	g -= saturation;
	b -= saturation;
	
	auto Val = [&] {
		return C_R * r + C_G * g + C_B * b;
	};
	
	double target = 255 - Saturate255(int(Val() + saturation));
	if(target < 30 && target >= 0) { // increase luminance of near black colors
		static const double tab[] = { // 29 * log(target + 1) / log(30)
			0, 5.91005636562468, 9.36721771666348, 11.8201127312494, 13.7227259177118,
			15.2772740822882, 16.5916258276743, 17.730169096874, 18.734435433327,
			19.6327822833365, 20.4454358591706, 21.1873304479128, 21.8698073118102,
			22.501682193299, 23.0899436343753, 23.6402254624987, 24.1571357841859,
			24.6444917989516, 25.1054910415395, 25.5428386489612, 25.9588435443378,
			26.3554922247953, 26.7345061336924, 27.0973868135375, 27.4454518354237,
			27.7798636774348, 28.1016531499904, 28.4117385589237, 28.7109415043374, 29 };
		target = tab[(int)target];
	}
		//target *= (1 / 1.5 + (29 - target) / 29) * 1.5;
	double ratio = target / 128;
	
	double m = max(r, g, b);
	
	if(m * ratio >= 255)
		ratio = 255 / m;
	
	r *= ratio;
	g *= ratio;
	b *= ratio;
	saturation = target - Val();
	
	return Color(Saturate255(int(r + saturation)),
	             Saturate255(int(g + saturation)),
	             Saturate255(int(b + saturation)));
}

Color DarkThemeCached(Color c)
{
	const int N = 256; // must be 2^N
	thread_local struct Cache {
		Color icolor[N];
		Color ocolor[N];
		
		Cache() {
			for(int i = 0; i < N; i++) {
				icolor[i] = Color(0, 0, 0);
				ocolor[i] = Color(255, 255, 255);
			}
		}
	} cache;
	
	int i = FoldHash32(c.GetRaw()) & (N - 1);
	if(cache.icolor[i] == c)
		return cache.ocolor[i];
	cache.icolor[i] = c;
	return cache.ocolor[i] = DarkTheme(cache.icolor[i]);
}

}
