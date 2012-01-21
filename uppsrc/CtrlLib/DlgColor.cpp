#include "CtrlLib.h"

NAMESPACE_UPP

class FetchColorCtrl : public Button
{
public:
	FetchColorCtrl();

	void          SetData(const Value& v);
	Value         GetData() const;

	virtual void  LeftDown(Point pt, dword keyflags);
	virtual void  MouseMove(Point pt, dword keyflags);
	virtual bool  Key(dword key, int repcnt);
	virtual Image CursorImage(Point pt, dword keyflags);

private:
	Color         user_color;
	Color         std_color;
};

FetchColorCtrl::FetchColorCtrl()
{
	SetImage(CtrlImg::fetch_color());
}

void FetchColorCtrl::SetData(const Value& v)
{
	if(IsNull(user_color))
		std_color = v;
}

Value FetchColorCtrl::GetData() const
{
	return Nvl(user_color, std_color);
}

void FetchColorCtrl::LeftDown(Point pt, dword keyflags)
{
	Button::LeftDown(pt, keyflags);
	SetCapture();
	SetWantFocus();
}

void FetchColorCtrl::MouseMove(Point pt, dword keyflags)
{
	if(keyflags & K_MOUSELEFT)
	{
		if(Rect(GetSize()).Contains(pt))
		{
			user_color = Null;
			Action();
		}
		else
		{
			pt += GetScreenView().TopLeft();
			user_color = GuiPlatformGetScreenPixel(pt.x, pt.y);
			Action();
		}
	}
	Button::MouseMove(pt, keyflags);
}

bool FetchColorCtrl::Key(dword key, int repcnt)
{
	if(key == K_ESCAPE && HasCapture())
	{
		ReleaseCapture();
		user_color = Null;
		Action();
		return true;
	}
	return Button::Key(key, repcnt);
}

Image FetchColorCtrl::CursorImage(Point pt, dword keyflags)
{
	if(keyflags & K_MOUSELEFT)
		return CtrlImg::fetch_color_cursor();
	return Button::CursorImage(pt, keyflags);
}

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
	draw.DrawImage(rc.left - size.cx - 2, y - (size.cy >> 1), CtrlImg::column_cursor());
}

static void PaintArrow(Draw& draw, const Rect& rc, int pos)
{
	PaintArrowRaw(draw, rc, rc.bottom - pos * rc.Height() / 255);
}

static const int std_palette[] =
{
	0x000000, 0x808080,
	0xC0C0C0, 0xFFFFFF,
	0x000080, 0x0000FF,
	0x008000, 0x00FF00,
	0x008080, 0x00FFFF,
	0x800000, 0xFF0000,
	0x800080, 0xFF00FF,
	0x808000, 0xFFFF00,
};

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
	Size size = max(GetSize(), Size(10, 10));
	round_step = 1;
	if(size.cx <= 20)
		column_rect = Null;
	else
	{
		int col_wd = size.cx >> 3;
		column_rect = Rect(size.cx - col_wd, 0, size.cx, size.cy);
		column_rect.Deflate(1, 5);
		size.cx -= col_wd + 8;
		while(round_step < 32768 && round_step * column_rect.Height() <= 65535)
			round_step <<= 1;
	}
	wheel_rect = Rect(size);
	wheel_rect.Deflate(1, 5);
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
			*scan++ = HSV16toRGB(cwb->arg, cwb->l, v16);
			cwb++;
		}
	}

	ImageDraw iw(size);
	iw.DrawImage(size, Rescale(ib, size));
	iw.DrawEllipse(size, Null, 0, Black);
	iw.Alpha().DrawRect(size, GrayColor(0));
	iw.Alpha().DrawEllipse(size, GrayColor(255), 0, GrayColor(255));
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
		draw.DrawRect(column_rect.left, i, size.cx, 1, c);
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

	static Index<PalCtrl *, PtrHash>& GetActive();

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

static void InitColor(Color *out)
{
	out[ 0] = Color(0x00, 0x00, 0x00);
	out[ 1] = Color(0x80, 0x80, 0x80);
	out[ 2] = Color(0xC0, 0xC0, 0xC0);
	out[ 3] = Color(0xFF, 0xFF, 0xFF);
	out[ 4] = Color(0x80, 0x00, 0x00);
	out[ 5] = Color(0xFF, 0x00, 0x00);
	out[ 6] = Color(0x00, 0x80, 0x00);
	out[ 7] = Color(0x00, 0xFF, 0x00);
	out[ 8] = Color(0x80, 0x80, 0x00);
	out[ 9] = Color(0xFF, 0xFF, 0x00);
	out[10] = Color(0x00, 0x00, 0x80);
	out[11] = Color(0x00, 0x00, 0xFF);
	out[12] = Color(0x80, 0x00, 0x80);
	out[13] = Color(0xFF, 0x00, 0xFF);
	out[14] = Color(0x00, 0x80, 0x80);
	out[15] = Color(0x00, 0xFF, 0xFF);
};

PalCtrl::Config& PalCtrl::GlobalConfig()
{
	static PalCtrl::Config x;
	return x;
}

typedef Index<PalCtrl *, PtrHash> PalCtrlIndex;

PalCtrlIndex& PalCtrl::GetActive()
{
	static PalCtrlIndex x;
	return x;
}

PalCtrl::PalCtrl()
{
	color = Null;
	color_index = -1;
	swap_index = -1;
	cellcount = Size(4, 4);
	Transparent();
}

PalCtrl::~PalCtrl()
{
	GetActive().RemoveKey(this);
}

Vector<Color> PalCtrl::GetPalette()
{
	int count = MAXSIZE;
	while(count > 0 && IsNull(GlobalConfig().current[count - 1]))
		count--;
	Vector<Color> out;
	out.SetCount(count);
	for(int i = 0; i < count; i++)
		out[i] = GlobalConfig().current[i];
	return out;
}


void PalCtrl::SetPalette(const Vector<Color>& pal)
{
	for(int n = min<int>(pal.GetCount(), MAXSIZE), i = 0; i < n; i++)
		GlobalConfig().current[i] = pal[i];
	PalCtrlIndex& active = GetActive();
	for(int c = 0; c < active.GetCount(); c++)
		active[c]->Refresh();
}

void PalCtrl::SerializePalette(Stream& stream)
{
	int version = StreamHeading(stream, 1, 1, 1, "PalCtrl::Palette");
	if(version >= 1)
	{
		int count = cellcount.cx * cellcount.cy;
		stream / count;
		for(int i = 0; i < count; i++)
			stream % GlobalConfig().current[i];
	}
}

INITBLOCK
{
	RegisterGlobalConfig("PalCtrl", callback(&PalCtrl::FlushConfig));
}

void PalCtrl::Config::Serialize(Stream& stream)
{
	if(stream.IsLoading() && stream.IsEof())
		return;
	int version = 1;
	stream / version;
	if(stream.IsError() || version < 1 || version > 1)
	{
		stream.SetError();
		return;
	}
	int count = __countof(current);
	stream / count;
	count = min<int>(count, MAXSIZE);
	for(int i = 0; i < count; i++)
		stream % current[i];
	stream % lastsize % lastfile;
}

void PalCtrl::Load()
{
	Config& gc = GlobalConfig();
	if(!gc.loaded)
	{
		gc.loaded = true;
		InitColor(gc.current);
		LoadFromGlobal(gc, "PalCtrl");
		cellcount = gc.lastsize;
		Layout();
		UpdateColorIndex();
	}
}

void PalCtrl::SetData(const Value& value)
{
	if(Color(value) != color)
	{
		color = value;
		UpdateColorIndex();
		UpdateRefresh();
	}
}

void PalCtrl::UpdateColorIndex()
{
	Config& gc = GlobalConfig();
	if(gc.loaded)
		color_index =(int)( UPP::Find(gc.current,
			gc.current + __countof(gc.current), color, StdEqual<int>()) - gc.current);
}

void PalCtrl::Layout()
{
	cell = max((GetSize() - 2 * OGAP - (cellcount + 1) * IGAP) / cellcount, Size(2, 2));
	step = cell + IGAP;
	offset = (GetSize() - cellcount * step + IGAP) >> 1;
	Refresh();
}

void PalCtrl::Paint(Draw& draw)
{
	Load();

	GetActive().FindAdd(this);
	draw.Begin();
	Size tcell = GetTextSize("256", StdFont());
	bool do_text = (tcell.cx <= cell.cx && tcell.cy <= cell.cy);
	for(int i = 0, n = cellcount.cx * cellcount.cy; i < n; i++)
	{
		Rect rc = IndexToClient(i);
		DrawFrame(draw, rc, White, Black);
		rc.Deflate(1);
		Color c = Nvl(GlobalConfig().current[i], SColorFace);
		draw.DrawRect(rc, c);
		if(do_text)
		{
			int brightness = 3 * c.GetR() + 6 * c.GetG() + 1 * c.GetB();
			Color ink = (brightness <= 1280 ? White : Black);
			draw.DrawText(rc.left + 2, rc.top + 2, IntStr(i + 1), StdFont(), ink);
		}
		rc.Inflate(1);
		draw.ExcludeClip(rc);
	}
	if(color_index >= 0)
	{
		Rect selected = IndexToClient(color_index);
		selected.Inflate(3);
		DrawFatFrame(draw, selected, SColorMark(), 2);
	}
	draw.End();
}

void PalCtrl::LeftDown(Point pt, dword keyflags)
{
	swap_index = ClientToIndex(pt);
	if(swap_index < 0)
		return;
	if(keyflags & K_CTRL)
	{
		set_index = swap_index;
		swap_index = -1;
		OnSetColor();
		return;
	}
	color = GlobalConfig().current[swap_index];
	color_index = swap_index;
	UpdateActionRefresh();
	SetCapture();
}

void PalCtrl::LeftUp(Point pt, dword keyflags)
{
	ReleaseCapture();
	int i = ClientToIndex(pt);
	if(i < 0 || swap_index < 0 || i == swap_index)
	{
		swap_index = -1;
		return;
	}
	Swap(GlobalConfig().current[swap_index], GlobalConfig().current[i]);
	color_index = i;
	swap_index = -1;
	UpdateActionRefresh();
}

Image PalCtrl::CursorImage(Point pt, dword keyflags)
{
	if(swap_index >= 0)
		return CtrlImg::swap_color_cursor();
	return Image::Arrow();
}

void PalCtrl::RightDown(Point pt, dword keyflags)
{
	MenuBar bar;
	set_index = ClientToIndex(pt);
	bar.Add(set_index >= 0 && !IsNull(color), t_("Set color"), THISBACK(OnSetColor))
		.Help(t_("Write current color to selected palette slot"));
	bar.Add(t_("Default palette"), THISBACK(OnStandard))
		.Help(t_("Restore default system palette"));
	bar.MenuSeparator();
	bar.Add(t_("Save as.."), THISBACK(OnSave))
		.Help(t_("Store palette information into a disk file"));
	bar.Add(t_("Load"), THISBACK(OnLoad))
		.Help(t_("Load previously saved palette"));
	bar.MenuSeparator();
	bool is_small = (cellcount.cx == 4 && cellcount.cy == 4);
	bool is_medium = (cellcount.cx == 8 && cellcount.cy == 8);
	bool is_large = (cellcount.cx == 16 && cellcount.cy == 16);
	bar.Add(t_("Small"), THISBACK(OnSizeSmall))
		.Check(is_small)
		.Help(t_("Set up palette size 4 times 4 colors"));
	bar.Add(t_("Medium"), THISBACK(OnSizeMedium))
		.Check(is_medium)
		.Help(t_("Set up palette size 8 times 8 colors"));
	bar.Add(t_("Large"), THISBACK(OnSizeLarge))
		.Check(is_large)
		.Help(t_("Set up palette size 16 times 16 colors (maximum size)"));
	bar.Add(t_("Custom..."), THISBACK(OnSizeCustom))
		.Check(!is_small && !is_medium && !is_large)
		.Help(t_("Select custom palette size"));
	bar.Execute();
}

void PalCtrl::OnSetColor()
{
	if(set_index >= 0 && set_index < cellcount.cx * cellcount.cy && !IsNull(color))
	{
		GlobalConfig().current[set_index] = color;
		UpdateRefresh();
		PalCtrlIndex& active = GetActive();
		for(int i = 0; i < active.GetCount(); i++)
		{
			PalCtrl *pal = active[i];
			if(pal->color_index == set_index && pal->color != color)
			{
				pal->UpdateColorIndex();
				pal->Refresh();
			}
		}
	}
}

void PalCtrl::OnStandard()
{
	memcpy(GlobalConfig().current, std_palette, min(sizeof(GlobalConfig().current), sizeof(std_palette)));
	PalCtrlIndex& active = GetActive();
	for(int i = 0; i < active.GetCount(); i++)
	{
		PalCtrl *pal = active[i];
		pal->UpdateColorIndex();
		pal->Refresh();
	}
}

void PalCtrl::OnSave()
{
	recent_file = SelectFileSaveAs("Palette (*.pal)\n*.pal");
	if(recent_file.GetCount())
	{
		StringStream stream;
		SerializePalette(stream);
		if(!SaveFile(recent_file, stream))
			Exclamation(NFormat(t_("Error writing file [* \1%s\1]."), recent_file));
	}
}

void PalCtrl::OnLoad()
{
	recent_file = SelectFileOpen("Palette (*.pal)\n*.pal");
	if(recent_file.GetCount())
	{
		FileIn fi(recent_file);
		if(!fi.IsOpen())
		{
			Exclamation(NFormat(t_("Error opening file [* \1%s\1]."), recent_file));
			return;
		}
		SerializePalette(fi);
		fi.Close();
		if(fi.IsError())
		{
			Exclamation(NFormat(t_("Error reading palette from file [* \1%s\1]."), recent_file));
			return;
		}
		Refresh();
	}
}

void PalCtrl::OnSizeSmall()
{
	GlobalConfig().lastsize = cellcount = Size(4, 4);
	Layout();
}

void PalCtrl::OnSizeMedium()
{
	GlobalConfig().lastsize = cellcount = Size(8, 8);
	Layout();
}

void PalCtrl::OnSizeLarge()
{
	GlobalConfig().lastsize = cellcount = Size(16, 16);
	Layout();
}

//RegisterHelpTopicObjectTitle(DlgPalCtrlSize, s_(PalCtrlSizeCustomDlgTitle))

void PalCtrl::OnSizeCustom()
{
	WithPalCtrlSizeLayout<TopWindow> dlg;
//	CtrlLayoutOKCancel(dlg, DlgPalCtrlSizeHelpTitle());
	CtrlLayoutOKCancel(dlg, t_("Palette dimensions"));
	dlg.HelpTopic("DlgPalCtrlSize");
	dlg.rows <<= cellcount.cy;
	dlg.columns <<= cellcount.cx;
	dlg.rows.NotNull().MinMax(1, 16);
	dlg.columns.NotNull().MinMax(1, 16);
	if(dlg.Run() == IDOK)
	{
		cellcount.cx = ~dlg.columns;
		cellcount.cy = ~dlg.rows;
		GlobalConfig().lastsize = cellcount;
		Layout();
	}
}

int PalCtrl::ClientToIndex(Point pt) const
{
	Size rel = pt - offset + (IGAP / 2);
	Point div = idivfloor(rel, step);
	if(div.x < 0 || div.x >= cellcount.cx || div.y < 0 || div.y >= cellcount.cy)
		return -1;
	return div.x * cellcount.cy + div.y;
}

Rect PalCtrl::IndexToClient(int index) const
{
	if(index < 0 || index >= cellcount.cx * cellcount.cy)
		return Rect(Null);
	return Rect(Size(index / cellcount.cy, index % cellcount.cy) * step + offset, cell);
}

//////////////////////////////////////////////////////////////////////
// ColorSelectorImpl::

class ColorSelectorImpl : public WithPaletteLayout<TopWindow>, public ColorSelector::Impl
{
public:
	typedef ColorSelectorImpl CLASSNAME;
	ColorSelectorImpl(ColorSelector& parent);
	virtual ~ColorSelectorImpl();

	void                  Attach(Ctrl& parent);

	virtual void          Set(Color color);
	virtual Color         Get() const;

	virtual void          NotNull(bool _nn = true);
	virtual bool          IsNotNull() const;

	virtual Vector<Color> GetPalette() const;
	virtual void          SetPalette(const Vector<Color>& pal);

	virtual void          SerializeConfig(Stream& stream);

	virtual Ctrl&         GetCtrl() { return *this; }

public:
	FetchColorCtrl        fetch_color;

private:
	void                  SetAction(Color color);
	void                  SetRaw();

	void                  UpdateRGB();
	void                  UpdateHSV();

	void                  OnRGB();
	void                  OnHSV();

//	void                  OnColor();
	void                  OnSetTransparent();

	void                  OnWheelCtrl();
	void                  OnRampCtrl();
	void                  OnRGBCtrl();
	void                  OnHSVCtrl();
	void                  OnPalCtrl();

	void                  OnFetchColor();

protected:
//	WithPaletteLayout<Ctrl> dialog;
	ColorDisplayNull      paint_color_display;

private:
	ColorSelector&        parent;
	Color                 colorval;
	WheelRampCtrl         wheel;
	WheelRampCtrl         ramp;
	RGBCtrl               rgb;
	HSVCtrl               hsv;
	PalCtrl               pal;
};

ColorSelectorImpl::ColorSelectorImpl(ColorSelector& parent)
: paint_color_display(t_("(transparent)"))
, parent(parent), wheel(false), ramp(true)
{
	Transparent();
	CtrlLayout(*this);
	parent << SizePos();

//	dialog.palette_tab.Background(SLtGray);

	palette_tab.Add(pal.HSizePos(4, 4).VSizePos(4, 4), t_("Palette"));
	pal <<= THISBACK(OnPalCtrl);

	palette_tab.Add(wheel.HSizePos(4, 4).VSizePos(4, 4), t_("Wheel"));
	wheel <<= THISBACK(OnWheelCtrl);

	palette_tab.Add(ramp.HSizePos(4, 4).VSizePos(4, 4), t_("Hue"));
	ramp <<= THISBACK(OnRampCtrl);

	palette_tab.Add(rgb.HSizePos(4, 4).VSizePos(4, 4), t_("RGB"));
	rgb <<= THISBACK(OnRGBCtrl);

	palette_tab.Add(hsv.HSizePos(4, 4).VSizePos(4, 4), t_("HSV"));
	hsv <<= THISBACK(OnHSVCtrl);

	color.SetFrame(InsetFrame());
	color.SetDisplay(paint_color_display);
	color.NoWantFocus();
	color.SetReadOnly();
//	dialog.color.WhenAction = THISBACK(OnColor);

	fetch_color <<= THISBACK(OnFetchColor);

	set_transparent <<= THISBACK(OnSetTransparent);
	set_transparent.SetImage(CtrlImg::set_transparent());

	EditIntSpin *spins[6] =
	{
		&r, &g, &b,
		&h, &s, &v,
	};

	int i;
	for(i = 0; i < __countof(spins); i++)
	{
		spins[i] ->	MinMax(0, 255);
		spins[i]->WhenAction = (i < 3 ? THISBACK(OnRGB) : THISBACK(OnHSV));
	}

	Set(Black);
}

ColorSelectorImpl::~ColorSelectorImpl()
{
}

void ColorSelectorImpl::SerializeConfig(Stream& stream)
{
	int version = 2;
	stream / version;
	if(version < 2)
		stream % palette_tab;
	else {
		int t = palette_tab.Get();
		stream / t;
		palette_tab.Set(t);
	}
	Color c = colorval;
	stream % c;
	if(stream.IsLoading())
		Set(c);
	PalCtrl::SerializeConfig(stream);
}

void ColorSelectorImpl::NotNull(bool _nn)
{
	set_transparent.Enable(!_nn);
}

bool ColorSelectorImpl::IsNotNull() const
{
	return !set_transparent.IsEnabled();
}

Vector<Color> ColorSelectorImpl::GetPalette() const
{
	return pal.GetPalette();
}

void ColorSelectorImpl::SetPalette(const Vector<Color>& p)
{
	pal.SetPalette(p);
}

void ColorSelectorImpl::SetRaw()
{
	wheel <<= ramp <<= rgb <<= hsv <<= pal <<= colorval;
	parent.WhenSetColor();
}

void ColorSelectorImpl::SetAction(Color color)
{
	Set(color);
	parent.Action();
}

Color ColorSelectorImpl::Get() const
{
	return colorval;
}

void ColorSelectorImpl::Set(Color c)
{
	colorval = c;
	color <<= c;
	fetch_color <<= c;
	UpdateRGB();
	UpdateHSV();
	wheel <<= ramp <<= rgb <<= hsv <<= pal <<= c;
	parent.Update();
	parent.WhenSetColor();
}

void ColorSelectorImpl::UpdateRGB()
{
	Color c = Nvl(colorval, Black);
	r <<= c.GetR();
	g <<= c.GetG();
	b <<= c.GetB();
}

void ColorSelectorImpl::UpdateHSV()
{
	Color c = Nvl(colorval, Black);
	double vh, vs, vv;
	RGBtoHSV(c.GetR() / 255.0, c.GetG() / 255.0, c.GetB() / 255.0, vh, vs, vv);
	h <<= fround(vh * 255);
	s <<= fround(vs * 255);
	v <<= fround(vv * 255);
}

void ColorSelectorImpl::OnRGB()
{
	int vr = Nvl((int)~r, 0), vg = Nvl((int)~g, 0), vb = Nvl((int)~b, 0);
	colorval = Color(vr, vg, vb);
	color <<= colorval;
	UpdateHSV();
	SetRaw();
}

void ColorSelectorImpl::OnHSV()
{
	int vh = Nvl((int)~h, 0), vs = Nvl((int)~s, 0), vv = Nvl((int)~v, 0);
	double r, g, b;
	HSVtoRGB(vh / 255.0, vs / 255.0, vv / 255.0, r, g, b);
	colorval = Color(fround(255 * r), fround(255 * g), fround(255 * b));
	color <<= colorval;
	UpdateRGB();
	SetRaw();
}

/*
void ColorSelectorImpl::OnColor()
{
	SetColor(~dialog.color);
}
*/

void ColorSelectorImpl::OnSetTransparent()
{
	SetAction(Null);
}

void ColorSelectorImpl::OnWheelCtrl()
{
	SetAction(~wheel);
}

void ColorSelectorImpl::OnRampCtrl()
{
	SetAction(~ramp);
}

void ColorSelectorImpl::OnRGBCtrl()
{
	SetAction(~rgb);
}

void ColorSelectorImpl::OnHSVCtrl()
{
	SetAction(~hsv);
	h <<= hsv.GetHValue();
	s <<= hsv.GetSValue();
	v <<= hsv.GetVValue();
}

void ColorSelectorImpl::OnPalCtrl()
{
	SetAction(~pal);
}

void ColorSelectorImpl::OnFetchColor()
{
	SetAction(~fetch_color);
}

//////////////////////////////////////////////////////////////////////
// ColorSelector::

ColorSelector::ColorSelector(bool not_null)
{
	Transparent();
	impl = new ColorSelectorImpl(*this);
	impl->NotNull(not_null);
}

class DlgSelectColor : public WithPaletteSelectorLayout<TopWindow>
{
public:
	typedef DlgSelectColor CLASSNAME;
	DlgSelectColor();

	virtual void  Serialize(Stream& stream);

	Color         Run(Color init_color, bool not_null, const char *title, bool *ok);

private:
	ColorSelector selector;
};

Color RunDlgSelectColor(Color init_color, bool not_null, const char *title, bool *ok)
{ return DlgSelectColor().Run(init_color, not_null, title, ok); }

//RegisterHelpTopicObjectTitle(DlgSelectColor, s_(DlgColorDefaultTitle));

DlgSelectColor::DlgSelectColor()
{
	palette.Transparent().NoWantFocus() << selector.SizePos();
//	CtrlLayoutOKCancel(*this, DlgSelectColorHelpTitle());
	CtrlLayoutOKCancel(*this, t_("Select color"));
//RegisterHelpTopicObjectTitle(DlgSelectColor, );
	HelpTopic("DlgSelectColor");
	Sizeable().MaximizeBox();
}

INITBLOCK
{
	RegisterGlobalConfig("DlgSelectColor");
}

void DlgSelectColor::Serialize(Stream& stream)
{
	if(stream.IsLoading() && stream.IsEof())
		return;
	int version = 1;
	stream / version;
	if(stream.IsError() || version < 1 || version > 1) {
		stream.SetError();
		return;
	}
	SerializePlacement(stream);
}

Color DlgSelectColor::Run(Color init_color, bool not_null, const char *title, bool *ok)
{
	if(title)
		Title(title);
	selector.NotNull(not_null).Set(init_color);
	LoadFromGlobal(*this, "DlgSelectColor");
	bool is_ok = (TopWindow::Run() == IDOK);
	StoreToGlobal(*this, "DlgSelectColor");
	if(ok)
		*ok = is_ok;
	return is_ok ? selector.Get() : init_color;
}

ColorCtrl::ColorCtrl(bool not_null)
: DataPusher(NoConvert(), StdColorDisplayNull())
{
	SetFrame(EditFieldFrame());
	AddFrame(empty);
	empty.SetImage(CtrlImg::cross());
	empty <<= THISBACK(OnClear);
	NotNull(not_null);
}

ColorCtrl::~ColorCtrl() {}

void ColorCtrl::DoAction()
{
	bool ok;
	Color new_color = RunDlgSelectColor(GetConvert().Format(GetData()), IsNotNull(), 0, &ok);
	if(ok)
		SetDataAction(GetConvert().Scan(new_color));
}

END_UPP_NAMESPACE
