#include "Draw.h"

NAMESPACE_UPP


#if defined(PLATFORM_WIN32) || defined(PLATFORM_X11)

#define LLOG(x)    // LOG(x)
#define LTIMING(x) // TIMING(x)

bool Draw::sFini;

void Draw::SinCos(int angle, double& sina, double& cosa)
{
	angle = angle % 3600;
	switch(angle) {
	case    0: sina =  0; cosa =  1; break;
	case  900: sina =  1; cosa =  0; break;
	case 1800: sina =  0; cosa = -1; break;
	case 2700: sina = -1; cosa =  0; break;
	default:
		double a = angle * M_PI / 1800.0;
		sina = sin(a);
		cosa = cos(a);
		break;
	}
}

bool Draw::StdFontSizeSet;
Size Draw::StdFontSize;
Font Draw::AStdFont;

INITBLOCK {
	RichValue<Font>::Register();
}

void Draw::InitFonts()
{
	DrawLock __;
	if(sFini) return;
	sFini = true;
	GetFontHash(0);
	GetFontLru();
	InitPlatformFonts();
}

int FontFilter(int c)
{
	return c >= 'a' && c <= 'z' || c >= '0' && c <= '9' ? c : c >= 'A' && c <= 'Z' ? ToLower(c) : 0;
}

int  Font::FindFaceNameIndex(const char *name) {
	if(!Draw::sFini) Draw::InitFonts();
	for(int i = 1; i < GetFaceCount(); i++)
		if(GetFaceName(i) == name)
			return i;
	String n = Filter(name, FontFilter);
	for(int i = 1; i < GetFaceCount(); i++)
		if(Filter(GetFaceName(i), FontFilter) == n)
			return i;
	return 0;
}

Draw& ScreenInfo();

FontInfo Font::Info() const
{
	DrawLock __;
	return ScreenInfo().GetFontInfoW(*this);
}

void Draw::SetStdFont(Font font)
{
	InitFonts();
	AStdFont = font;
	StdFontSizeSet = false;
}

Size Draw::GetStdFontSize()
{
	DrawLock __;
	if(!StdFontSizeSet) {
		StdFontSizeSet = true;
		FontInfo fi = AStdFont.Info();
		FontInfo bfi = AStdFont().Bold().Info();
		StdFontSize = Size(fi.GetAveWidth(), bfi.GetHeight());
	}
	return StdFontSize;
}

Font StdFont()
{
	return Font(0, Draw::AStdFont.GetHeight());
}

String Font::GetFaceName() const {
	if(IsNull()) return String();
	if(GetFace() == 0)
		return "STDFONT";
	return GetFaceName(GetFace());
}

dword Font::GetFaceInfo() const {
	if(IsNull()) return 0;
	return GetFaceInfo(GetFace());
}

Font& Font::FaceName(const String& name) {
	int n = FindFaceNameIndex(name);
	Face(n < 0 ? 0xffff : n);
	return *this;
}

void Font::Serialize(Stream& s) {
	int version = 1;
	s / version;
	if(version >= 1) {
		int f = GetFace();
		if(f > COURIER)
			f = -1;
		s / f;
		String name;
		if(f < 0) {
			name = GetFaceName();
			s % name;
		}
		if(s.IsLoading())
			if(f >= 0)
				Face(f);
			else
				FaceName(name);
	}
	else {
		String name = GetFaceName();
		s % name;
		if(s.IsLoading()) {
			FaceName(name);
			if(IsNull())
				Face(COURIER);
		}
	}
	s % flags % height % width;
}

template<>
String AsString(const Font& f) {
	if(IsNull(f)) return "<null>";
	String s = "<" + f.GetFaceName() + Format(":%d", f.GetHeight());
	if(f.IsBold())
		s += " Bold";
	if(f.IsItalic())
		s += " Italic";
	if(f.IsUnderline())
		s += " Underline";
	if(f.IsStrikeout())
		s += " Strikeout";
	return s + '>';
}

int Draw::FontCacheMax = 32;
int Draw::FontCached;

void Draw::FreeFonts() {
	FontCacheMax = FontCached = 0;
	sFini = false;
	for(int i = 0; i < FONTHASH; i++)
		GetFontHash(i)->DeleteList(HASH);
}

FontInfo::FontInfo(const FontInfo& f)
{
	Retain(f);
}

FontInfo& FontInfo::operator=(const FontInfo& f)
{
	Release();
	Retain(f);
	return *this;
}

//# Pretty ugly code....
FontInfo::Data *Draw::GetFontHash(int i) {
	static byte buff[FONTHASH * sizeof(FontLink)];
	static FontLink *fonthash;
	if(!fonthash) {
		fonthash = (FontLink *)buff;
		for(int i = 0; i < FONTHASH; i++)
			fonthash[i].LinkSelfAll();
	}
	ASSERT(i >= 0 && i < FONTHASH);
	return (FontInfo::Data *)&fonthash[i];
}

//# Pretty ugly code....
FontInfo::Data *Draw::GetFontLru() {
	static byte buff[sizeof(FontLink)];
	static FontLink *fontlru;
	if(!fontlru)
		fontlru = new(buff) FontLink;
	return (FontInfo::Data *)fontlru;
}

bool FontInfo::IsEqual(byte _charset, Font f, int angle, int device) const
{
	return ptr && ptr->font == f && ptr->angle == angle && ptr->device == device &&
	       charset == _charset;
}

FontInfo::CharMetrics FontInfo::GetCM(int c) const
{
//	CharMetrics mm;
//	ptr->GetMetrics(&mm, c, 1);
//	return mm;

	if(c < 0) c = (byte)c;
	if(charset != CHARSET_UNICODE)
		c = ToUnicode(c, charset);
	ASSERT(c < 65536);
	if(c >= 65536) {
		CharMetrics h;
		h.width = h.lspc = h.rspc = 0;
		return h;
	}
	if(c < 2048)
		return GetPage(c >> 5)[c & 31];
	Mutex::Lock __(ptr->xmutex);
	Kinfo& ki = ptr->kinfo.At((c >> 10) - 2);
	if(!ki.flags) {
		ki.flags = new byte[128];
		memset(ki.flags, 0, 128);
		ptr->GetMetrics(&ki.std, c, 1);
	}
	int fi = (c >> 3) & 127;
	int fm = 1 << (c & 7);
	if(ki.flags[fi] & fm)
		return ki.std;
	int q = ptr->xx.Find(c);
	if(q >= 0)
		return ptr->xx[q];
	CharMetrics m;
	ptr->GetMetrics(&m, c, 1);
	if(m == ki.std)
		ki.flags[fi] |= fm;
	else
		ptr->xx.Add(c, m);
	return m;
}

int FontInfo::GetWidth(int c) const {
	return GetCM(c).width;
}

int FontInfo::GetLeftSpace(int c) const {
	return GetCM(c).lspc;
}

int FontInfo::GetRightSpace(int c) const {
	return GetCM(c).rspc;
}

void Draw::Release(FontInfo::Data *font) {
	DrawLock __;
	ASSERT(font->refcount > 0);
	LLOG("Release " << font->font << " count:" << font->count);
	if(--font->refcount == 0) {
		if(FontCacheMax == 0) {
			delete font;
			return;
		}
		FontInfo::Data *lru = GetFontLru();
		font->LinkAfter(lru, LRU);
		FontCached++;
		LLOG("Placed to cache " << font->font << " cached:" << FontCached);
		while(FontCached > FontCacheMax) {
			ASSERT(lru->GetPrev(LRU) != lru);
			FontCached--;
			LLOG("Deleting from cache " << lru->GetPrev(LRU)->font <<
				        " cached: " << FontCached);
			delete lru->GetPrev(LRU);
		}
	}
}

void FontInfo::Release()
{
	if(ptr) Draw::Release(ptr);
}

void FontInfo::Retain(const FontInfo& f)
{
	DrawLock __;
	ptr = f.ptr;
	ptr->refcount++;
	charset = f.charset;
}


FontInfo Draw::GetFontInfoW(Font font) {
	DrawLock __;
	LTIMING("GetFontInfo");
	byte charset = CHARSET_UNICODE;
	if(lastFont.IsEqual(charset, font, 0, device))
		return lastFont;
#ifdef PLATFORM_WIN32
	HDC hdc = GetHandle();
	FontInfo fi = Acquire(font, hdc ? hdc : ScreenHDC(), 0, device);
#endif
#ifdef PLATFORM_X11
	FontInfo fi = Acquire(font, 0, device);
#endif
	fi.charset = charset;
	return fi;
}

void Draw::SetFont(Font font, int angle) {
	DrawLock __;
	LLOG("Set font: " << font << " face: " << font.GetFaceName());
	if(lastFont && lastFont.IsEqual(CHARSET_UNICODE, font, angle, device))
		return;
#ifdef PLATFORM_WIN32
	lastFont = Acquire(font, GetHandle(), angle, device);
	HFONT h = (HFONT) SelectObject(handle, lastFont.ptr->hfont);
	if(!orgFont) orgFont = h;
#endif
#ifdef PLATFORM_X11
	lastFont = Acquire(font, angle, device);
#endif
}

WString TextUnicode(const char *s, int n, byte cs, Font font)
{
	if(n < 0)
		n = (int)strlen(s);
	if(font.GetFace() == Font::SYMBOL) {
		WStringBuffer b(n);
		wchar *t = b;
		while(n > 0) {
			*t++ = *s++;
			n--;
		}
		return b;
	}
	else
		return ToUnicode(s, n, cs);
}

FontInfo::CharMetrics *FontInfo::CreateMetricsPage(int page) const
{
	DrawLock __;
	CharMetrics *cm;
	cm = new CharMetrics[32];
	ptr->GetMetrics(cm, page << 5, 32);
	if(page >= 8 && page < 12)
		ComposeMetrics(ptr->font, cm, page);
	return cm;
}

FontInfo::CharMetrics *FontInfo::GetPage(int page) const
{
	ASSERT(page >= 0 && page < 64);
	ONCELOCK_PTR(ptr->base[page], CreateMetricsPage(page));
	return ptr->base[page];
}

void Draw::DrawText(int x, int y, int angle, const wchar *text, Font font,
		            Color ink, int n, const int *dx)
{
	if(IsNull(ink)) return;
	if(n < 0)
		n = wstrlen(text);
	ComposeText(x, y, angle, text, font, ink, n, dx);
}

// ----------------------------

void Draw::DrawText(int x, int y, const wchar *text, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, 0, text, font, ink, n, dx);
}

// ---------------------------

void Draw::DrawText(int x, int y, int angle, const WString& text, Font font,
                    Color ink, const int *dx)
{
	DrawText(x, y, angle, ~text, font, ink, text.GetLength(), dx);
}

void Draw::DrawText(int x, int y, const WString& text, Font font, Color ink, const int *dx)
{
	DrawText(x, y, 0, text, font, ink, dx);
}

// ---------------------------

void Draw::DrawText(int x, int y, int angle, const char *text, byte charset, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, angle, TextUnicode(text, n, charset, font), font, ink, dx);
}

void Draw::DrawText(int x, int y, const char *text, byte charset, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, 0, text, charset, font, ink, n, dx);
}

// ---------------------------

void Draw::DrawText(int x, int y, int angle, const char *text,
                    Font font, Color ink, int n, const int *dx)
{
	DrawText(x, y, angle, text, CHARSET_DEFAULT, font, ink, n, dx);
}

void Draw::DrawText(int x, int y, const char *text, Font font,
                    Color ink, int n, const int *dx)
{
	DrawText(x, y, text, CHARSET_DEFAULT, font, ink, n, dx);
}

// ---------------------------

void Draw::DrawText(int x, int y, int angle, const String& text, Font font,
                    Color ink, const int *dx)
{
	DrawText(x, y, angle, text, font, ink, text.GetLength(), dx);
}

void Draw::DrawText(int x, int y, const String& text, Font font, Color ink, const int *dx)
{
	WString h = TextUnicode(text, text.GetLength(), CHARSET_DEFAULT, font);
	DrawText(x, y, h, font, ink, h.GetLength(), dx);
}

// --------------------------

Size GetTextSize(const wchar *text, Font font, int n)
{
	FontInfo fi = font.Info();
	if(n < 0)
		n = wstrlen(text);
	Size sz;
	sz.cx = 0;
	const wchar *wtext = (const wchar *)text;
	while(n > 0) {
		sz.cx += fi[*wtext++];
		n--;
	}
	sz.cy = fi.GetHeight();
	return sz;
}

Size GetTextSize(const WString& text, Font font)
{
	return GetTextSize(text, font, text.GetLength());
}

Size GetTextSize(const char *text, byte charset, Font font, int n)
{
	return GetTextSize(TextUnicode(text, n, charset, font), font);
}

Size GetTextSize(const char *text, Font font, int n)
{
	return GetTextSize(text, CHARSET_DEFAULT, font, n);
}

Size GetTextSize(const String& text, Font font)
{
	return GetTextSize(text, font, text.GetLength());
}

Font Draw::GetStdFont()
{
	return AStdFont;
}

#endif

END_UPP_NAMESPACE
