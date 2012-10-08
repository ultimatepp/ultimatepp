#include "Draw.h"

#define LLOG(x)

NAMESPACE_UPP

CommonFontInfo   GetFontInfoSys(Font font);
GlyphInfo        GetGlyphInfoSys(Font font, int chr);
void             GetStdFontSys(String& name, int& height);
Vector<FaceInfo> GetAllFacesSys();

bool Replace(Font fnt, int chr, Font& rfnt);

void Std(Font& font)
{
	if(IsNull(font))
		font = StdFont();
	if(font.GetFace() == 0)
		font.Face(GetStdFont().GetFace());
	if(font.GetHeight() == 0)
		font.Height(GetStdFont().GetHeight());
}

Size Font::StdFontSize;
Font Font::AStdFont;

INITBLOCK {
	Value::Register<Font>("Font");
}

const Vector<FaceInfo>& Font::List()
{
	static Vector<FaceInfo> *q;
	ONCELOCK {
		static Vector<FaceInfo> x;
		x = GetAllFacesSys();
		q = &x;
	}
	return *q;
}

void sInitFonts()
{
	Font::List();
	GetStdFont();
}

INITBLOCK {
	sInitFonts();
}

int Font::GetFaceCount()
{
	return List().GetCount();
}

String Font::GetFaceName(int index)
{
	if(index == 0)
		return "STDFONT";
	const Vector<FaceInfo>& l = List();
	if(index >= 0 && index < l.GetCount())
		return l[index].name;
	return Null;
}

dword Font::GetFaceInfo(int index)
{
	const Vector<FaceInfo>& l = List();
	if(index >= 0 && index < l.GetCount())
		return l[index].info;
	return 0;
}

int FontFilter(int c)
{
	return c >= 'a' && c <= 'z' || c >= '0' && c <= '9' ? c : c >= 'A' && c <= 'Z' ? ToLower(c) : 0;
}

int  Font::FindFaceNameIndex(const String& name) {
	if(name == "STDFONT")
		return 0;
	for(int i = 1; i < GetFaceCount(); i++)
		if(GetFaceName(i) == name)
			return i;
	String n = Filter(name, FontFilter);
	for(int i = 1; i < GetFaceCount(); i++)
		if(Filter(GetFaceName(i), FontFilter) == n)
			return i;
	if(n == "serif")
		return SERIF;
	if(n == "sansserif")
		return SANSSERIF;
	if(n == "monospace")
		return MONOSPACE;
	if(n == "stdfont")
		return STDFONT;
	return 0;
}

void Font::SyncStdFont()
{
	DrawLock __;
	StdFontSize = Size(AStdFont.GetAveWidth(), AStdFont().Bold().GetCy());
}

void (*whenSetStdFont)();

void Font::SetStdFont(Font font)
{
	DrawLock __;
	static int x = 0;
	if(x) return;
	x++;
	InitStdFont();
	AStdFont = font;
	SyncStdFont();
	if(whenSetStdFont)
		(*whenSetStdFont)();
	x--;
}

void Font::InitStdFont()
{
	ONCELOCK {
		DrawLock __;
		List();
		AStdFont = Arial(12);
		String name;
		int    height = 0;
		GetStdFontSys(name, height);
#ifdef PLATFORM_WIN32
#ifdef flagWINGL
		AStdFont = Font(FindFaceNameIndex("Tahoma"), 12);
#else
		int q = FindFaceNameIndex(name);
		if(q <= 0)
			q = FindFaceNameIndex("Tahoma");
		if(q <= 0)
			q = FindFaceNameIndex("Microsoft Sans Serif");
		if(q <= 0)
			q = FindFaceNameIndex("MS Sans Serif");
		if(q > 0)
			AStdFont = Font(q, max(height, 1));
#endif
		SyncStdFont();
#endif
	}
}

Font Font::GetStdFont()
{
	InitStdFont();
	return AStdFont;
}

Size Font::GetStdFontSize()
{
	InitStdFont();
	return StdFontSize;
}

Font StdFont()
{
	return Font(0, -32000);
}

int Font::GetHeight() const
{
	return v.height == -32000 ? GetStdFont().GetHeight() : v.height;
}

String Font::GetFaceName() const {
	if(IsNullInstance()) return String();
	if(GetFace() == 0)
		return "STDFONT";
	return GetFaceName(GetFace());
}

dword Font::GetFaceInfo() const {
	if(IsNullInstance()) return 0;
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
    	enum {
			OLD_STDFONT, OLD_SCREEN_SERIF, OLD_SCREEN_SANS, OLD_SCREEN_FIXED,
			OLD_ROMAN,
			OLD_ARIAL,
			OLD_COURIER,
		};
		int f = GetFace();
		if(f > COURIER)
			f = -1;
		s / f;
		String name;
		if(f == OLD_ROMAN)
			f = ROMAN;
		if(f == OLD_ARIAL)
			f = ARIAL;
		if(f == OLD_COURIER)
			f = COURIER;
		if(f < 0) {
			name = GetFaceName();
			s % name;
		}
		if(s.IsLoading())
			if(f >= 0)
				Face(f);
			else {
				FaceName(name);
				if(IsNull(name))
					SetNull();
			}
	}
	else {
		String name = GetFaceName();
		s % name;
		if(s.IsLoading()) {
			FaceName(name);
			if(IsNullInstance())
				Face(COURIER);
		}
	}
	s % v.flags % v.height % v.width;
}

String Font::GetTextFlags() const
{
	String txt;
	if(IsBold())
		txt << "bold ";
	if(IsItalic())
		txt << "italic ";
	if(IsUnderline())
		txt << "underline ";
	if(IsStrikeout())
		txt << "strikeout ";
	if(IsNonAntiAliased())
		txt << "noaa ";
	if(IsTrueTypeOnly())
		txt << "ttonly ";
	if(txt.GetCount())
		txt.Trim(txt.GetCount() - 1);
	return txt;
}

void Font::ParseTextFlags(const char *s)
{
	CParser p(s);
	v.flags = 0;
	while(!p.IsEof()) {
		if(p.Id("bold"))
			Bold();
		else
		if(p.Id("italic"))
			Italic();
		else
		if(p.Id("underline"))
			Underline();
		else
		if(p.Id("strikeout"))
			Strikeout();
		else
		if(p.Id("noaa"))
			NonAntiAliased();
		else
		if(p.Id("ttonly"))
			TrueTypeOnly();
		else
			p.SkipTerm();
	}
}

String Font::GetFaceNameStd() const
{
	switch(GetFace()) {
	case STDFONT:   return "STDFONT";
	case SERIF:     return "serif";
	case SANSSERIF: return "sansserif";
	case MONOSPACE: return "monospace";
	}
	return GetFaceName();
}

void Font::Jsonize(JsonIO& jio)
{
	String n, tf;
	if(jio.IsStoring()) {
		n = GetFaceNameStd();
		tf = GetTextFlags();
		if(IsNullInstance())
			n.Clear();
	}
	jio("face", n)("height", v.height)("width", v.width)("flags", tf);
	if(IsNull(n))
		SetNull();
	else {
		FaceName(n);
		ParseTextFlags(tf);
	}
}

void Font::Xmlize(XmlIO& xio)
{
	String n, tf;
	if(xio.IsStoring()) {
		n = GetFaceNameStd();
		tf = GetTextFlags();
		if(IsNullInstance())
			n.Clear();
	}
	xio.Attr("face", n)
	   .Attr("height", v.height)
	   .Attr("width", v.width)
	   .Attr("flags", tf);
	if(IsNull(n))
		SetNull();
	else {
		FaceName(n);
		ParseTextFlags(tf);
	}
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

struct CharEntry {
	int64     font;
	GlyphInfo info;
	word      chr;
};

CharEntry fc_cache_global[4093];

bool IsNormal(Font font, int chr)
{
	DrawLock __;
	CharEntry& e = fc_cache_global[CombineHash(font.GetHashValue(), chr) % 4093];
	if(e.font == font.AsInt64() || e.chr == chr)
		return e.info.IsNormal();
	return GetGlyphInfoSys(font, chr).IsNormal();
}

CharEntry GetGlyphEntry(Font font, int chr, unsigned hash)
{
	DrawLock __;
	CharEntry& e = fc_cache_global[hash % 4093];
	if(e.font != font.AsInt64() || e.chr != chr) {
		e.font = font.AsInt64();
		e.chr = chr;
		e.info = GetGlyphInfoSys(font, chr);
		if(!e.info.IsNormal()) {
			ComposedGlyph cg;
			Font rfnt;
			if(Compose(font, chr, cg)) {
				e.info.lspc = -1;
				e.info.rspc = cg.basic_char;
			}
			else
			if(Replace(font, chr, rfnt)) {
				e.info.lspc = rfnt.GetFace();
				e.info.rspc = rfnt.GetHeight();
			}
			else
				e.info.lspc = -2;
		}
	}
	return e;
}

thread__ CharEntry fc_cache[512];

GlyphInfo GetGlyphInfo(Font font, int chr)
{
	Std(font);
	unsigned hash = CombineHash(font.GetHashValue(), chr);
	CharEntry& e = fc_cache[hash & 511];
	if(e.font != font.AsInt64() || e.chr != chr)
		e = GetGlyphEntry(font, chr, hash);
	return e.info;
}

bool Font::IsNormal(int ch) const
{
	return GetGlyphInfo(*this, ch).IsNormal();
}

bool Font::IsComposed(int ch) const
{
	return GetGlyphInfo(*this, ch).IsComposed();
}

bool Font::IsReplaced(int ch) const
{
	return GetGlyphInfo(*this, ch).IsReplaced();
}

bool Font::IsMissing(int ch) const
{
	return GetGlyphInfo(*this, ch).IsMissing();
}

int Font::HasChar(int ch) const
{
	return !GetGlyphInfo(*this, ch).IsMissing();
}

void GlyphMetrics(GlyphInfo& f, Font font, int chr)
{
	if(f.IsReplaced())
		f = GetGlyphInfo(font().Face(f.lspc).Height(f.rspc), chr);
	if(f.IsComposed()) {
		f = GetGlyphInfo(font, f.rspc);
		if(f.IsComposedLM())
			f.rspc += f.width / 2;
	}
}

GlyphInfo GetGlyphMetrics(Font font, int chr)
{
	GlyphInfo f = GetGlyphInfo(font, chr);
	if(f.IsMissing())
		f = GetGlyphInfo(font, '?');
	GlyphMetrics(f, font, chr);
	return f;
}

struct FontEntry {
	CommonFontInfo info;
	int64          font;
};

thread__ FontEntry fi_cache[64];

const CommonFontInfo& GetFontInfo(Font font)
{
	Std(font);
	unsigned hash = font.GetHashValue() & 63;
	FontEntry& e = fi_cache[hash];
	if(e.font != font.AsInt64()) {
		DrawLock __;
		e.font = font.AsInt64();
		e.info = GetFontInfoSys(font);
	}
	return e.info;
}

int Font::GetWidth(int c) const {
	return GetGlyphMetrics(*this, c).width;
}

int Font::GetLeftSpace(int c) const {
	return GetGlyphMetrics(*this, c).lspc;
}

int Font::GetRightSpace(int c) const {
	return GetGlyphMetrics(*this, c).rspc;
}

thread__ int64 lastFiFont = INT_MIN;
thread__ CommonFontInfo lastFontInfo;
thread__ int64 lastStdFont = INT_MIN;


const CommonFontInfo& Font::Fi() const
{
#ifdef PLATFORM_OSX11
// known leak on MacOSX here: getAllCarbonLazyValues2000 calls Core.h op new()
// should not call UPP op new()
// from GetFontInfo() ... XftFontOpenPattern() ... getAllCarbonLazyValues2000() -> new()
	MemoryIgnoreLeaksBlock __;
#endif

	if(lastStdFont != AStdFont.AsInt64()) {
		lastFiFont = INT_MIN;
		lastStdFont = AStdFont.AsInt64();
	}
	if(AsInt64() == lastFiFont)
		return lastFontInfo;
	lastFontInfo = GetFontInfo(*this);
	lastFiFont = AsInt64();
	return lastFontInfo;
}

FontInfo Font::Info() const
{
	FontInfo h;
	h.font = *this;
	return h;
}

END_UPP_NAMESPACE
