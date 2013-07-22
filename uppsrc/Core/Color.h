#ifdef PLATFORM_WIN32
struct RGBA : Moveable<RGBA> {
        byte b, g, r, a;
};
#endif

#ifdef PLATFORM_POSIX
#ifdef CPU_BE
struct RGBA : Moveable<RGBA> {
        byte a, r, g, b;
};
#else
struct RGBA : Moveable<RGBA> {
        byte b, g, r, a;
};
#endif
#endif

template <>
String AsString(const RGBA& c);

#ifndef PLATFORM_WIN32
inline int        GetRValue(dword c)             { return (byte)(c >> 0); }
inline int        GetGValue(dword c)             { return (byte)(c >> 8); }
inline int        GetBValue(dword c)             { return (byte)(c >> 16); }
inline dword      RGB(byte r, byte g, byte b)    { return r | (g << 8) | (b << 16); }
#endif

const int COLOR_V = 39;

class Color : public ValueType<Color, COLOR_V, Moveable<Color> > {
protected:
	dword    color;

	dword Get() const;

public:
	dword    GetRaw() const            { return color; }

	int      GetR() const              { return GetRValue(Get()); }
	int      GetG() const              { return GetGValue(Get()); }
	int      GetB() const              { return GetBValue(Get()); }

	void     SetNull()                 { color = 0xffffffff; }
	bool     IsNullInstance() const    { return color == 0xffffffff; }
	unsigned GetHashValue() const      { return color; }
	bool     operator==(Color c) const { return color == c.color; }
	bool     operator!=(Color c) const { return color != c.color; }

	void     Serialize(Stream& s)      { s % color; }
	void     Jsonize(JsonIO& jio);
	void     Xmlize(XmlIO& xio);

	Color()                            { SetNull(); }
	Color(int r, int g, int b)         { color = RGB(r, g, b); }
	Color(int n, int)                  { color = 0x80000000 | n; }

	Color(const Nuller&)               { SetNull(); }

	operator Value() const             { return SvoToValue(*this); }
	Color(const Value& q)              { color = q.Get<Color>().color; }

	operator RGBA() const;
	Color(RGBA rgba);

	Color(Color (*fn)())               { color = (*fn)().color; }

	static Color FromRaw(dword co)     { Color c; c.color = co; return c; }

#ifdef PLATFORM_WIN32
	operator COLORREF() const          { return (COLORREF) Get(); }
	static  Color FromCR(COLORREF cr)  { Color c; c.color = (dword)cr; return c; }
#else
	operator dword() const             { return Get(); }
#endif

private:
	Color(int);
};

RGBA operator*(int alpha, Color c);

inline Color StraightColor(RGBA rgba)  { return Color(rgba.r, rgba.g, rgba.b); }

typedef Color (*ColorF)();

inline unsigned GetHashValue(Color c)  { return c.GetHashValue(); }
inline Color    Nvl(Color a, Color b)  { return IsNull(a) ? b : a; }

template<>
String AsString(const Color& c);


inline Color GrayColor(int a = 128)    { return Color(a, a, a); }

inline Color Black()     { return Color(0, 0, 0); }
inline Color Gray()      { return Color(128, 128, 128); }
inline Color LtGray()    { return Color(192, 192, 192); }
inline Color WhiteGray() { return Color(224, 224, 224); }
inline Color White()     { return Color(255, 255, 255); }

inline Color Red()       { return Color(128, 0, 0); }
inline Color Green()     { return Color(0, 128, 0); }
inline Color Brown()     { return Color(128, 128, 0); }
inline Color Blue()      { return Color(0, 0, 128); }
inline Color Magenta()   { return Color(128, 0, 255); }
inline Color Cyan()      { return Color(0, 128, 128); }
inline Color Yellow()    { return Color(255, 255, 0); }
inline Color LtRed()     { return Color(255, 0, 0); }
inline Color LtGreen()   { return Color(0, 255, 0); }
inline Color LtYellow()  { return Color(255, 255, 192); }
inline Color LtBlue()    { return Color(0, 0, 255); }
inline Color LtMagenta() { return Color(255, 0, 255); }
inline Color LtCyan()    { return Color(0, 255, 255); }

void   RGBtoHSV(double r, double g, double b, double& h, double& s, double& v);
void   HSVtoRGB(double h, double s, double v, double& r, double& g, double& b);

Color  HsvColorf(double h, double s, double v);

Color  Blend(Color c1, Color c2, int alpha = 128);

String ColorToHtml(Color color);

int  Grayscale(const Color& c);
bool IsDark(Color c);


inline bool operator==(const Value& v, Color x)        { return v == x.operator Value(); }
inline bool operator==(Color x, const Value& v)        { return v == x.operator Value(); }
inline bool operator!=(const Value& v, Color x)        { return v != x.operator Value(); }
inline bool operator!=(Color x, const Value& v)        { return v != x.operator Value(); }

inline bool operator==(const Value& v, Color (*x)())   { return v == (*x)(); }
inline bool operator==(Color (*x)(), const Value& v)   { return v == (*x)(); }
inline bool operator!=(const Value& v, Color (*x)())   { return v != (*x)(); }
inline bool operator!=(Color (*x)(), const Value& v)   { return v != (*x)(); }

inline bool operator==(Color c, Color (*x)())          { return c == (*x)(); }
inline bool operator==(Color (*x)(), Color c)          { return c == (*x)(); }
inline bool operator!=(Color c, Color (*x)())          { return c != (*x)(); }
inline bool operator!=(Color (*x)(), Color c)          { return c != (*x)(); }
