#include "CtrlLib.h"

namespace Upp {
	
const Display& StdColorDisplayNull()
{
	static ColorDisplayNull display(t_("(no color)"));
	return display;
}

const int *Gamma16()
{
	static int table[65536];
	if(table[65535] == 0)
	{
		enum { STEP = 4 };
		int prev = 0;
		for(int i = 0; i < 65536; i += STEP)
		{
			int next = minmax<int>(fround(pow((i + STEP) / 65535.0, 1.5) * 65535.0), 0, 65535);
			for(int t = 0; t < STEP; t++)
				table[i + t] = prev + iscale(next - prev, t, STEP);
			prev = next;
		}
	}
	return table;
}

inline int Gamma16(int a)
{
	return Gamma16()[a];
}

const int *DeGamma16()
{
	static int table[65536];
	if(table[65535] == 0)
	{
		enum { STEP = 4 };
		int prev = 0;
		for(int i = 0; i < 65536; i += STEP)
		{
			int next = minmax<int>(fround(pow((i + STEP) / 65535.0, 1.0 / 1.5) * 65535.0), 0, 65535);
			for(int t = 0; t < STEP; t++)
				table[i + t] = prev + iscale(next - prev, t, STEP);
			prev = next;
		}
	}
	return table;
}

inline int DeGamma16(int a)
{
	return DeGamma16()[a];
}

const int *Sin16()
{
	static int table[65536];
	if(table[16384] == 0)
	{
		enum { STEP = 4 };
		int prev = 0;
		for(int i = 0; i < 65536; i += STEP)
		{
			int next = minmax<int>(fround(sin((i + STEP) * (M_PI / 32768.0)) * 65536.0), -65536, +65536);
			for(int t = 0; t < STEP; t++)
				table[i + t] = prev + iscale(next - prev, t, STEP);
			prev = next;
		}
	}
	return table;
}

inline int Sin16(int i)
{
	return Sin16()[i & 65535];
}

inline int Cos16(int i)
{
	return Sin16()[(i + 16384) & 65535];
}

inline byte GetRRaw(Color rgb)
{
	return byte(rgb.GetRaw() >> 0);
}

inline byte GetGRaw(Color rgb)
{
	return byte(rgb.GetRaw() >> 8);
}

inline byte GetBRaw(Color rgb)
{
	return byte(rgb.GetRaw() >> 16);
}

inline int GetRGamma(Color rgb)
{
	return Gamma16(GetRRaw(rgb) * 257);
}

inline int GetGGamma(Color rgb)
{
	return Gamma16(GetGRaw(rgb) * 257);
}

inline int GetBGamma(Color rgb)
{
	return Gamma16(GetBRaw(rgb) * 257);
}

inline int GetV16(Color rgb)
{
	return Gamma16(max(GetRRaw(rgb), max(GetGRaw(rgb), GetBRaw(rgb))) * 257);
}

inline int GetL16(Color rgb)
{
	return Gamma16(min(GetRRaw(rgb), min(GetGRaw(rgb), GetBRaw(rgb))) * 257);
}

int GetS16(Color rgb)
{
	int hi = GetV16(rgb);
	if(hi <= 0)
		return 0;
	return iscale((hi - GetL16(rgb)), 0xFFFF, hi);
}

//inline int GetS(Color rgb) { return GetS16(rgb) >> 8; }

int GetH16(Color rgb)
{
	int l = GetL16(rgb), v = GetV16(rgb), d = v - l;
	if(d == 0)
		return 0;
	int r = GetRGamma(rgb);
	int g = GetGGamma(rgb);
	int b = GetBGamma(rgb);
	int h;
	if(g == v)
		h = 0x5555 + (b - r) * 0x2AAA / d;
	else if(b == v)
		h = 0xAAAA + (r - g) * 0x2AAA / d;
	else
		h = (g - b) * 0x2AAA / d;
	return h & 0xFFFF;
}

//inline int GetHValue(Color rgb) { return GetH16(rgb) >> 8; }

Color HSV16toRGB(int h16, int s16, int v16)
{
	const int *degamma = DeGamma16();
	int v = degamma[v16] >> 8;
	if(s16 == 0)
		return GrayColor(v);
	h16 &= 0xFFFF;
	unsigned rem = (h16 *= 6) & 0xFFFF;
	int p = degamma[iscale(v16, 0xFFFF - s16, 65536)] >> 8;
	int q = degamma[iscale(v16, 0xFFFF - iscale(s16, rem, 65536), 65536)] >> 8;
	int t = degamma[iscale(v16, 0xFFFF - iscale(s16, 0xFFFFu - rem, 65536), 65536)] >> 8;
	switch(h16 >> 16)
	{
	default: NEVER(); // invalid color!
	case 0: return Color(v, t, p);
	case 1: return Color(q, v, p);
	case 2: return Color(p, v, t);
	case 3: return Color(p, q, v);
	case 4: return Color(t, p, v);
	case 5: return Color(v, p, q);
	}
}

static void PaintArrowRaw(Draw& draw, const Rect& rc, int y)
{
	Size size = CtrlImg::column_cursor().GetSize();
	draw.DrawImage(rc.left - size.cx - DPI(2), y - (size.cy >> 1), CtrlImg::column_cursor());
}

static void PaintArrow(Draw& draw, const Rect& rc, int pos)
{
	PaintArrowRaw(draw, rc, rc.bottom - pos * rc.Height() / 255);
}

WheelRampCtrl::WheelRampCtrl(bool r)
: ramp(r)
{
//	gamma = 2.5;
	style = S_WHEEL;
	color = Black;
	normalized_color = White;
	h16 = s16 = v16 = 0;
	Transparent();
}

WheelRampCtrl::~WheelRampCtrl()
{
}

void WheelRampCtrl::Layout()
{
	Size size = max(GetSize(), DPI(Size(10, 10)));
	round_step = 1;
	if(size.cx <= DPI(20))
		column_rect = Null;
	else
	{
		int col_wd = size.cx >> 3;
		column_rect = Rect(size.cx - col_wd, 0, size.cx, size.cy);
		column_rect.Deflate(1, DPI(5));
		size.cx -= col_wd + DPI(8);
		while(round_step < 32768 && round_step * column_rect.Height() <= 65535)
			round_step <<= 1;
	}
	wheel_rect = Rect(size);
	wheel_rect.Deflate(1, DPI(5));
	Refresh();
}

static byte GetColorLevel(Color color)
{
	return IsNull(color) ? 255 : max(color.GetR(), max(color.GetG(), color.GetB()));
}

static Color SetColorLevel(Color color, int level)
{
	int m = GetColorLevel(color);
	if(!m)
		return Color(level, level, level);
	return Color(
		color.GetR() * level / m,
		color.GetG() * level / m,
		color.GetB() * level / m);
}

int WheelRampCtrl::ClientToLevel(int y) const
{
	if(column_rect.IsEmpty())
		return 65535;
	return minmax<int>(iscale(column_rect.bottom - y, 65535, column_rect.Height()), 0, 65535);
}

int WheelRampCtrl::LevelToClient(int l) const
{
	return column_rect.bottom - iscale(l, column_rect.Height(), 65535);
}

void WheelRampCtrl::Paint(Draw& draw)
{
	if(!cache || cache.GetSize() != wheel_rect.GetSize() || cache_level != (ramp ? h16 : v16)) {
		cache = ramp ? PaintRamp(wheel_rect.GetSize()) : PaintWheel(wheel_rect.GetSize());
		cache_level = (ramp ? h16 : v16);
	}

	draw.DrawImage(wheel_rect.left, wheel_rect.top, cache);
	PaintColumn(draw);

	Point mark = wheel_rect.CenterPoint();
	if(ramp)
	{
		mark.x = wheel_rect.left + iscale(s16, wheel_rect.Width(), 65536);
		mark.y = wheel_rect.bottom - iscale(v16, wheel_rect.Height(), 65536);
	}
	else
	{
		Point c = wheel_rect.CenterPoint();
		Size r = wheel_rect.Size() >> 1;
		mark.x = c.x + iscale(iscale(r.cx, s16, 65536), Cos16(h16), 65536);
		mark.y = c.y - iscale(iscale(r.cy, s16, 65536), Sin16(h16), 65536);
	}
	Size size = CtrlImg::wheel_cursor().GetSize();
	mark -= size >> 1;
	draw.DrawImage(mark.x, mark.y, v16 >= 0x8000 ? CtrlImg::wheel_cursor() : CtrlImg::white_wheel_cursor());
	if(!column_rect.IsEmpty())
		PaintArrowRaw(draw, column_rect, LevelToClient(ramp ? h16 : v16));
}

void WheelRampCtrl::SetColor(Color _color, bool set_norm, bool set_hsv)
{
	Color new_color = Nvl(_color, White);
	Refresh();
	color = new_color;
	if(set_norm)
		normalized_color = SetColorLevel(_color, 255);
	if(set_hsv)
	{
		h16 = GetH16(color);
		v16 = GetV16(color);
		s16 = GetS16(color);
	}
}

void WheelRampCtrl::SetData(const Value& value)
{
	if(Color(value) != color)
		SetColor(value, true, true);
}

void WheelRampCtrl::LeftUp(Point pt, dword keyflags)
{
	firstclick = 0;
	ReleaseCapture();
}

#ifdef PLATFORM_WINCE
inline double hypot(double a, double b) { return _hypot(a, b); }
#endif

void WheelRampCtrl::LeftDown(Point pt, dword keyflags)
{
	if(!HasCapture())
	{
		SetCapture();
		if(!column_rect.IsEmpty() && pt.x >= column_rect.left)
			firstclick = 1;
		else if (pt.x < wheel_rect.right)
			firstclick = 2;
		else
			firstclick = 0;
	}

	Color new_color = Null;
	if(firstclick == 1)
	{
		if(ramp)
			new_color = HSV16toRGB(h16 = ClientToLevel(pt.y), s16, v16);
		else
			new_color = HSV16toRGB(h16, s16, v16 = ClientToLevel(pt.y));
		SetColor(new_color, false, false);
	}
	else if(firstclick == 2)
	{ // set location on color wheel
		if(ramp)
		{
			s16 = minmax<int>(iscale(pt.x - wheel_rect.left, 65535, wheel_rect.Width()), 0, 65535);
			v16 = minmax<int>(iscale(wheel_rect.bottom - pt.y, 65535, wheel_rect.Height()), 0, 65535);
			new_color = HSV16toRGB(h16, s16, v16);
		}
		else
		{
			Point c = wheel_rect.CenterPoint();
			Size r = wheel_rect.Size() >> 1;
			double x = (pt.x - c.x) / (double)r.cx;
			double y = (c.y - pt.y) / (double)r.cy;
			double s = min<double>(hypot(x, y), 1);
			h16 = s16 = 0;
			if(x || y)
			{
				double a = fmod(atan2(y, x) / (2 * M_PI) + 1, 1);
				h16 = minmax<int>(fround(a * 65536), 0, 65535);
				s16 = minmax<int>(fround(s * 65536), 0, 65535);
			}
			new_color = HSV16toRGB(h16, s16, v16);
		}
		SetColor(new_color, true, false);
	}
	Action();
}

void WheelRampCtrl::LeftDouble(Point pt, dword keyflags)
{
	WhenLeftDouble();
}

void WheelRampCtrl::MouseMove(Point pt, dword keyflags)
{
	if(keyflags & (K_MOUSELEFT | K_MOUSERIGHT))
		LeftDown(pt, keyflags);
}

enum { PREC = 64 };

Image WheelRampCtrl::PaintRamp(Size size)
{
	ImageDraw iw(size);
	ImageBuffer ib(PREC, PREC);
	for(int y = 0; y < PREC; y++) {
		RGBA *scan = ib[y];
		int v16 = iscale(PREC - y - 1, 65535, PREC - 1);
		for(int x = 0; x < PREC; x++) {
			int s16 = iscale(x, 65535, PREC - 1);
			Color c = HSV16toRGB(h16, s16, v16);
			if(IsDarkContent())
				c = DarkTheme(c);
			scan->r = GetRRaw(c);
			scan->g = GetGRaw(c);
			scan->b = GetBRaw(c);
			scan->a = 255;
			scan++;
		}
	}
	iw.DrawImage(0, 0, Rescale(ib, size));
	DrawFrame(iw, size, Black, Black);
	return iw;
}

Image WheelRampCtrl::PaintWheel(Size size)
{
	ImageBuffer ib(PREC, PREC);
	static WheelBuff wb[PREC * PREC];
	ONCELOCK {
		int i = 0;
		for(int y = 0; y < PREC; y++) {
			double ny = (PREC / 2 - y) / (double)(PREC / 2);
			for(int x = 0; x < PREC; x++) {
				double nx = (x - (PREC / 2)) / (double)(PREC / 2);
				double arg = fmod(atan2(ny, nx) / (2 * M_PI) + 1, 1);
				double l = min<double>(hypot(nx, ny), 1);
				wb[i].arg = fround(arg * 65535);
				wb[i].l = fround(l * 65535);
				i++;
			}
		}
	}

	WheelBuff *cwb = wb;
	for(int y = 0; y < PREC; y++) {
		RGBA *scan = ib[y];
		for(int x = 0; x < PREC; x++) {
			Color c = HSV16toRGB(cwb->arg, cwb->l, v16);
			if(IsDarkContent())
				c = DarkTheme(c);
			*scan++ = c;
			cwb++;
		}
	}

	ImagePainter iw(size);
	iw.Clear();
	Sizef s2 = (Sizef)(size) / 2;
	iw.Begin();
	iw.Ellipse(s2.cx, s2.cy, s2.cx, s2.cy).Clip();
	iw.DrawImage(size, Rescale(ib, size));
	iw.End();
	iw.Ellipse(s2.cx, s2.cy, s2.cx - 1, s2.cy - 1).Stroke(1, SBlack());
	return iw;
}

void WheelRampCtrl::PaintColumn(Draw& draw)
{
	if(column_rect.IsEmpty())
		return;
	Rect rc = column_rect;
	rc.Inflate(1, 1);
	DrawFrame(draw, rc, Black, Black);
	Size size = column_rect.Size();
//	int nr = GetRRaw(normalized_color);
//	int ng = GetGRaw(normalized_color);
//	int nb = GetBRaw(normalized_color);
	for(int i = column_rect.top; i < column_rect.bottom; i++)
	{
		int factor = ClientToLevel(i);
		Color c = ramp ? HSV16toRGB(factor, 65535, 65535) : HSV16toRGB(h16, s16, factor);
//			: Color(iscale(nr, factor, 65536), iscale(ng, factor, 65536), iscale(nb, factor, 65536));
		draw.DrawRect(column_rect.left, i, size.cx, 1, IsDarkContent() ? DarkThemeCached(c) : c);
	}
}

//////////////////////////////////////////////////////////////////////
// RGBCtrl::

class RGBCtrl : public Ctrl
{
public:
	RGBCtrl();

	virtual void  SetData(const Value& value);
	virtual Value GetData() const { return color; }

	virtual void  Layout();
	virtual void  Paint(Draw& draw);

	virtual void  LeftDown(Point pt, dword keyflags);
	virtual void  MouseMove(Point pt, dword keyflags);

private:
	int           ClientToLevel(int y) const;

private:
	enum { BAR = 6 };
	Rect          rect[3];
	int           round_step;
	Color         color;
};

RGBCtrl::RGBCtrl()
: color(Black)
{
	Transparent();
}

void RGBCtrl::SetData(const Value& value)
{
	Color new_color = value;
	if(color != new_color)
		Refresh();
	color = new_color;
}

void RGBCtrl::Layout()
{
	Size size = GetSize();
	int third = max(size.cx / 3 - 3, 10);
	rect[0] = Rect(8, 5, third - 2, max(size.cy - 5, 6));
	rect[2] = rect[1] = rect[0];
	rect[1].Offset((size.cx - third) >> 1, 0);
	rect[2].Offset(size.cx - third, 0);
	round_step = 1;
	while(round_step < 256 && round_step * rect[0].Height() <= 255)
		round_step <<= 1;
}

int RGBCtrl::ClientToLevel(int y) const
{
	y = (rect[0].bottom - y) * 255 / rect[0].Height();
	return minmax(y & ~(round_step - 1), 0, 255);
}

void RGBCtrl::Paint(Draw& draw)
{
	int comp[3] = { GetRValue(color), GetGValue(color), GetBValue(color) };
	int w = rect[0].Width() >> 1;
	int i;
	for(i = rect[0].top; i < rect[0].bottom; i++)
	{
		int   level = ClientToLevel(i);
		Color colors[6] =
		{
			Color(level, comp[1], comp[2]), Color(level, 0, 0),
			Color(comp[0], level, comp[2]), Color(0, level, 0),
			Color(comp[0], comp[1], level), Color(0, 0, level),
		};
		for(int j = 0; j < 3; j++)
		{
			draw.DrawRect(rect[j].left, i, w, 1, colors[2 * j + 0]);
			draw.DrawRect(rect[j].left + w + 1, i, w, 1, colors[2 * j + 1]);
		}
	}
	for(i = 0; i < 3; i++)
	{
		Rect rc = rect[i];
		draw.DrawRect(rc.left + w, rc.top, 1, rc.Height(), Black);
		rc.Inflate(1);
		DrawFrame(draw, rc, Black, Black);
		draw.ExcludeClip(rc);
	}
	for(i = 0; i < 3; i++)
		PaintArrow(draw, rect[i], comp[i]);
}

void RGBCtrl::LeftDown(Point pt, dword keyflags)
{
	int level = ClientToLevel(pt.y);
	int r = GetRValue(color), g = GetGValue(color), b = GetBValue(color);
	Color new_color = color;
	if(pt.x <= rect[0].right)
		new_color = Color(level, g, b);
	else if(pt.x <= rect[1].right)
		new_color = Color(r, level, b);
	else
		new_color = Color(r, g, level);
	SetData(new_color);
	Action();
}

void RGBCtrl::MouseMove(Point pt, dword keyflags)
{
	if(keyflags & (K_MOUSELEFT | K_MOUSERIGHT))
		LeftDown(pt, keyflags);
}

//////////////////////////////////////////////////////////////////////
// HSVCtrl::

class HSVCtrl : public Ctrl
{
public:
	HSVCtrl();

	virtual void  SetData(const Value& value);
	virtual Value GetData() const { return color; }

	virtual void  Layout();
	virtual void  Paint(Draw& draw);

	virtual void  LeftDown(Point pt, dword keyflags);
	virtual void  MouseMove(Point pt, dword keyflags);

	int           GetHValue() const { return h16 >> 8; }
	int           GetSValue() const { return s16 >> 8; }
	int           GetVValue() const { return v16 >> 8; }

private:
	int           ClientToLevel16(int y) const;
	void          SetDataRaw(Color color);

private:
	enum { BAR = 6 };
	Rect          rect[3];
	int           round_step;
	Color         color;
	int           h16, s16, v16;
};

HSVCtrl::HSVCtrl()
: color(Black)
, h16(0), s16(0), v16(0)
{
	Transparent();
}

void HSVCtrl::SetDataRaw(Color new_color)
{
	if(color != new_color)
	{
		color = new_color;
		Refresh();
	}
}

void HSVCtrl::SetData(const Value& value)
{
	Color i = Nvl(Color(value), Black);
	if(i != color)
	{
		SetDataRaw(i);
		h16 = GetH16(i);
		s16 = GetS16(i);
		v16 = GetV16(i);
		Refresh();
	}
}

void HSVCtrl::Layout()
{
	Size size = GetSize();
	int third = max(size.cx / 3 - 3, 10);
	rect[0] = Rect(8, 5, third - 2, max(size.cy - 5, 6));
	rect[2] = rect[1] = rect[0];
	rect[1].OffsetHorz((size.cx - third) >> 1);
	rect[2].OffsetHorz(size.cx - third);
	round_step = 1;
	while(round_step < 32768 && round_step * rect[0].Height() <= 65535)
		round_step <<= 1;
}

int HSVCtrl::ClientToLevel16(int y) const
{
	y = (rect[0].bottom - y) * 65535 / rect[0].Height();
	return minmax(y & ~(round_step - 1), 0, 65535);
}

void HSVCtrl::Paint(Draw& draw)
{
	int w = rect[0].Width() >> 1;
	int i;
	for(i = rect[0].top; i < rect[0].bottom; i++)
	{
		int l16 = ClientToLevel16(i);
		Color colors[6] =
		{
			HSV16toRGB(l16, s16, v16), HSV16toRGB(l16, 0xFFFF, 0xFFFF),
			HSV16toRGB(h16, l16, v16), HSV16toRGB(0xAAAA, l16, 0xFFFF),
			HSV16toRGB(h16, s16, l16), HSV16toRGB(0, 0, l16),
		};
		for(int j = 0; j < 3; j++)
		{
			draw.DrawRect(rect[j].left, i, w, 1, colors[2 * j + 0]);
			draw.DrawRect(rect[j].left + w + 1, i, w, 1, colors[2 * j + 1]);
		}
	}
	for(i = 0; i < 3; i++)
	{
		Rect rc = rect[i];
		draw.DrawRect(rc.left + w, rc.top, 1, rc.Height(), Black);
		rc.Inflate(1);
		DrawFrame(draw, rc, Black, Black);
		draw.ExcludeClip(rc);
	}
	int comp[] = { h16 >> 8, s16 >> 8, v16 >> 8 };
	for(i = 0; i < 3; i++)
		PaintArrow(draw, rect[i], comp[i]);
}

void HSVCtrl::LeftDown(Point pt, dword keyflags)
{
	int level = ClientToLevel16(pt.y);
	if(pt.x <= rect[0].right)
		h16 = level;
	else if(pt.x <= rect[1].right)
		s16 = level;
	else
		v16 = level;
	Color new_color = HSV16toRGB(h16, s16, v16);
	SetDataRaw(new_color);
	Refresh();
	Action();
}

void HSVCtrl::MouseMove(Point pt, dword keyflags)
{
	if(keyflags & (K_MOUSELEFT | K_MOUSERIGHT))
		LeftDown(pt, keyflags);
}

//////////////////////////////////////////////////////////////////////
// PalCtrl::

//#ifdef COMPILER_MSC
//class PalCtrl;
//unsigned GetHashValue(PalCtrl *x) { return (unsigned) x; }
//#endif

class PalCtrl : public Ctrl
{
public:
	typedef PalCtrl CLASSNAME;
	PalCtrl();
	virtual ~PalCtrl();

	virtual void  SetData(const Value& value);
	virtual Value GetData() const { return color; }

	virtual void  Layout();
	virtual void  Paint(Draw& draw);

	virtual void  LeftDown(Point pt, dword keyflags);
	virtual void  LeftUp(Point pt, dword keyflags);
	virtual void  RightDown(Point pt, dword keyflags);
	virtual Image CursorImage(Point pt, dword keyflags);

	static Vector<Color> GetPalette();
	static void   SetPalette(const Vector<Color>& pal);
	void          SerializePalette(Stream& stream);

	static void   FlushConfig() { if(GlobalConfig().loaded) StoreToGlobal(GlobalConfig(), "PalCtrl"); }
	static void   SerializeConfig(Stream& stream) { stream % GlobalConfig(); }

private:
	void          Load();

	int           ClientToIndex(Point pt) const;
	Rect          IndexToClient(int index) const;

	void          OnSetColor();
	void          OnStandard();
	void          OnSave();
	void          OnLoad();
	void          OnSizeSmall();
	void          OnSizeMedium();
	void          OnSizeLarge();
	void          OnSizeCustom();
	void          UpdateColorIndex();

	static Index<PalCtrl *>& GetActive();

public:
	enum
	{
		XMAXSIZE = 16,
		YMAXSIZE = 16,
		MAXSIZE = XMAXSIZE * YMAXSIZE,
	};

private:
	enum
	{
		OGAP = 2,
		IGAP = 1,
	};
	struct Config
	{
		Config() : lastsize(4, 4), loaded(false) {}
		void   Serialize(Stream& stream);

		Color  current[MAXSIZE];
		Size   lastsize;
		String lastfile;
		bool   loaded;
	};
	static Config& GlobalConfig();

	Point         offset;
	Size          cell;
	Size          step;
	Size          cellcount;
	Color         color;
	int           color_index;
	int           swap_index;
	int           set_index;
	String        recent_file;
};

}
