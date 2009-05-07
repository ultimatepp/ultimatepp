#include "Draw.h"

#define LLOG(x)

NAMESPACE_UPP

void InitPlatformFonts();

Size FontInfo::StdFontSize;
Font FontInfo::AStdFont;

int FontInfo::FontCacheMax = 32;
int FontInfo::FontCached;

void FreeFonts();

enum FontHashConst { FONTHASH = 97 };

//# Pretty ugly code....
FontInfo::Data *FontInfo::GetFontHash(int i) {
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
FontInfo::Data *FontInfo::GetFontLru() {
	static byte buff[sizeof(FontLink)];
	static FontLink *fontlru;
	if(!fontlru)
		fontlru = new(buff) FontLink;
	return (FontInfo::Data *)fontlru;
}

INITBLOCK {
	RichValue<Font>::Register();
}

void FontInfo::InitFonts()
{
	ONCELOCK {
		DrawLock __;
		GetFontHash(0);
		GetFontLru();
		InitPlatformFonts();
	}
}

int FontFilter(int c)
{
	return c >= 'a' && c <= 'z' || c >= '0' && c <= '9' ? c : c >= 'A' && c <= 'Z' ? ToLower(c) : 0;
}

int  Font::FindFaceNameIndex(const char *name) {
	FontInfo::InitFonts();
	for(int i = 1; i < GetFaceCount(); i++)
		if(GetFaceName(i) == name)
			return i;
	String n = Filter(name, FontFilter);
	for(int i = 1; i < GetFaceCount(); i++)
		if(Filter(GetFaceName(i), FontFilter) == n)
			return i;
	return 0;
}

FontInfo::FontInfo()
{
	ptr = NULL;
	charset = CHARSET_UNICODE;
	InitFonts();
}

SystemDraw& ScreenInfo();

FontInfo Font::Info() const
{
	DrawLock __;
	return FontInfo::AcquireFontInfo(*this, 0);
}

void FontInfo::SyncStdFont()
{
	FontInfo fi = AStdFont.Info();
	FontInfo bfi = AStdFont().Bold().Info();
	StdFontSize = Size(fi.GetAveWidth(), bfi.GetHeight());
}

void FontInfo::SetStdFont(Font font)
{
	DrawLock __;
	InitFonts();
	AStdFont = font;
	SyncStdFont();
}

Size FontInfo::GetStdFontSize()
{
	ONCELOCK {
		SyncStdFont();
	}
	return StdFontSize;
}

Font StdFont()
{
	return Font(0, FontInfo::GetStdFont().GetHeight());
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

void FontInfo::FreeFonts() {
	FontCacheMax = FontCached = 0;
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

bool FontInfo::IsEqual(byte _charset, Font f, int angle, int device) const
{
	return ptr && ptr->font == f && ptr->angle == angle && charset == _charset;
}

FontInfo::CharMetrics FontInfo::GetCM(int c) const
{
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

void FontInfo::Release()
{
	DrawLock __;
	if(ptr) {
		ASSERT(ptr->refcount > 0);
		LLOG("Release " << (void *)ptr << " count:" << ptr->count);
		if(--ptr->refcount == 0) {
			if(FontCacheMax == 0) {
				delete ptr;
				return;
			}
			FontInfo::Data *lru = GetFontLru();
			ptr->LinkAfter(lru, LRU);
			FontCached++;
			LLOG("Placed to cache " << ptr->ptr << " cached:" << FontCached);
			while(FontCached > FontCacheMax) {
				ASSERT(lru->GetPrev(LRU) != lru);
				FontCached--;
				LLOG("Deleting from cache " << lru->GetPrev(LRU)->ptr <<
					        " cached: " << FontCached);
				delete lru->GetPrev(LRU);
			}
		}
	}
}

void FontInfo::Retain(const FontInfo& f)
{
	DrawLock __;
	ptr = f.ptr;
	ptr->refcount++;
	charset = f.charset;
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

END_UPP_NAMESPACE
