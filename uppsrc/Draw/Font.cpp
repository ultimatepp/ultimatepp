#include "Draw.h"

#define LLOG(x)  // DLOG(x)

namespace Upp {

StaticMutex sFontLock;

bool Replace(Font fnt, int chr, Font& rfnt);

void Std(Font& font)
{
	font.RealizeStd();
}

Size Font::StdFontSize;
Size Font::StdFontSizeA;
Font Font::AStdFont;

INITBLOCK {
	Value::Register<Font>("Font");
}

static bool sListValid;

void InvalidateFontList()
{
	sListValid = false;
}

Vector<FaceInfo>& Font::FaceList()
{
	static Vector<FaceInfo> list;
	ONCELOCK {
		list = GetAllFacesSys();
	}
	return list;
}

void sInitFonts()
{
	Mutex::Lock __(sFontLock);
	Font::FaceList();
	GetStdFont();
}

INITBLOCK {
	sInitFonts();
}

int Font::GetFaceCount()
{
	Mutex::Lock __(sFontLock);
	return FaceList().GetCount();
}

String Font::GetFaceName(int index)
{
	Mutex::Lock __(sFontLock);
	if(index == 0)
		return "STDFONT";
	const Vector<FaceInfo>& l = FaceList();
	if(index >= 0 && index < l.GetCount())
		return l[index].name;
	return Null;
}

dword Font::GetFaceInfo(int index)
{
	Mutex::Lock __(sFontLock);
	const Vector<FaceInfo>& l = FaceList();
	if(index >= 0 && index < l.GetCount())
		return l[index].info;
	return 0;
}

void Font::SetFace(int index, const String& name, dword info)
{
	Mutex::Lock __(sFontLock);
	FaceInfo& f = FaceList().At(index);
	f.name = name;
	f.info = info;
}

void Font::SetFace(int index, const String& name)
{
	int q = FindFaceNameIndex(name);
	q = q >= 0 ? GetFaceInfo(q) : 0;
	SetFace(index, name, q);
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
	Mutex::Lock __(sFontLock);
	StdFontSize = Size(AStdFont.GetAveWidth(), AStdFont().GetCy());
	
	StdFontSizeA = Size(0, 0);
	for(int italic = 0; italic < 2; italic++)
		for(int bold = 0; bold < 2; bold++) {
			Font fnt = AStdFont().Bold(bold).Italic(italic);
			StdFontSizeA.cx = max(StdFontSizeA.cx, fnt.GetAveWidth());
			StdFontSizeA.cy = max(StdFontSizeA.cy, fnt.GetCy());
		}

	LLOG("SyncStdFont " << StdFontSize);
	SyncUHDMode();
}

void (*whenSetStdFont)();

void Font::SetStdFont0(Font font)
{
	LLOG("SetStdFont " << font);
	Mutex::Lock __(sFontLock);
	static int x = 0;
	if(x) return;
	x++;
	InitStdFont();
	AStdFont = font;
	LLOG("AStdFont1: " << AStdFont);
	SyncStdFont();
	LLOG("AStdFont2: " << AStdFont);
	if(whenSetStdFont)
		(*whenSetStdFont)();
	LLOG("AStdFont3: " << AStdFont);
	x--;
	static int w = 0;
	if(w) return;
	w++;
	if(whenSetStdFont)
		(*whenSetStdFont)();
	LLOG("AStdFont4: " << AStdFont);
	w--;
}

bool Font::std_font_override;

void Font::SetDefaultFont(Font font)
{
	LLOG("SetDefaultFont " << font);
	if(!std_font_override)
		SetStdFont0(font);
}

void Font::SetStdFont(Font font)
{
	std_font_override = true;
	SetStdFont0(font);
}

void Font::InitStdFont()
{
	ONCELOCK { // TODO: This is now sort of obsolete function....
	//	Mutex::Lock __(sFontLock);
	//	FaceList();
		AStdFont = Arial(12);
	//	SyncStdFont();
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

Size Font::GetStdFontSizeA()
{
	InitStdFont();
	return StdFontSizeA;
}

Font StdFont()
{
	return Font(0, -32000);
}

void Font::RealizeStd()
{
	if(IsNullInstance())
		*this = GetStdFont();
	if(v.face == STDFONT)
		Face(GetStdFont().GetFace());
	if(v.height == -32000)
		Height(GetStdFont().GetHeight());
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
		if(s.IsLoading()) {
			if(f >= 0)
				Face(f);
			else {
				FaceName(name);
				if(IsNull(name))
					SetNull();
			}
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
	wchar     chr;
};

CharEntry fc_cache_global[16384];

inline hash_t GlyphHash(Font font, int chr)
{
	return FoldHash(CombineHash(font.GetHashValue(), chr));
}

bool IsNormal_nc(Font font, int chr)
{ // do not change cache - to be used in Replace
	Mutex::Lock __(sFontLock);
	font.RealizeStd();
	CharEntry& e = fc_cache_global[GlyphHash(font, chr) & 16383];
	if(e.font == font.AsInt64() && e.chr == chr)
		return e.info.IsNormal();
	return GetGlyphInfoSys(font, chr).IsNormal();
}

struct GlyphInfoMaker : ValueMaker {
	Font font;
	int  chr;

	virtual String Key() const {
		StringBuffer s;
		int64 h = font.AsInt64();
		RawCat(s, h);
		RawCat(s, chr);
		return String(s);
	}
	virtual int    Make(Value& object) const {
		CharEntry& e = CreateRawValue<CharEntry>(object);
		e.font = font.AsInt64();
		e.chr = chr;
		Font rfnt;
		if(PreferColorEmoji(chr) && !(font.GetFaceInfo() & Font::COLORIMG) && !font.IsNoColor()
		   && Replace(font, chr, rfnt) && rfnt != font) {
			e.info.width = (int16)0x8000;
			e.info.lspc = rfnt.GetFace();
			e.info.rspc = rfnt.GetHeight();
		}
		else {
			e.info = GetGlyphInfoSys(font, chr);
			if(!e.info.IsNormal()) {
				ComposedGlyph cg;
				if(Compose(font, chr, cg)) {
					e.info.lspc = -1;
					e.info.rspc = (int16)cg.basic_char;
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
		return sizeof(e);
	}
};

CharEntry GetGlyphEntry(Font font, int chr, hash_t hash)
{
	Mutex::Lock __(sFontLock);
	GlyphInfoMaker m;
	m.font = font;
	m.chr = chr;
	return MakeValue(m).To<CharEntry>();
}

thread_local CharEntry fc_cache[512];

GlyphInfo GetGlyphInfo(Font font, int chr)
{
	font.RealizeStd();
	hash_t hash = GlyphHash(font, chr);
	CharEntry& e = fc_cache[hash & 511];
	if(e.font != font.AsInt64() || e.chr != chr)
		e = GetGlyphEntry(font, chr, hash);
	return e.info;
}

struct FontEntry {
	CommonFontInfo info;
	int64          font;
};

thread_local FontEntry fi_cache[63];

const CommonFontInfo& GetFontInfo(Font font)
{
	font.RealizeStd();
	dword hash = FoldHash(font.GetHashValue()) % 63;
	FontEntry& e = fi_cache[hash];
	if(e.font != font.AsInt64()) {
		Mutex::Lock __(sFontLock);
		e.font = font.AsInt64();
		e.info = GetFontInfoSys(font);
	}
	return e.info;
}

thread_local int64 lastFiFont = INT_MIN;
thread_local CommonFontInfo lastFontInfo;
thread_local int64 lastStdFont = INT_MIN;

const CommonFontInfo& Font::Fi() const
{
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
	font.RealizeStd();
	GlyphInfo f = GetGlyphInfo(font, chr);
	if(f.IsMissing()) {
		Font fnt = Arial(font.GetHeight());
		wchar chr = 0x25a1;
		f = GetGlyphInfo(fnt, chr);
		if(!f.IsNormal()) {
			chr = ' ';
			f = GetGlyphInfo(fnt, chr);
		}
	}
	else
		GlyphMetrics(f, font, chr);
	return f;
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

String Font::GetData(const char *table, int offset, int size) const
{
	Mutex::Lock __(sFontLock);
	ASSERT(!table || strlen(table) == 4);
	return GetFontDataSys(*this, table, offset, size);
}

void Font::Render(FontGlyphConsumer& sw, double x, double y, int ch) const
{
	Mutex::Lock __(sFontLock);
	RenderCharacterSys(sw, x, y, ch, *this);
}

FontInfo Font::Info() const
{
	FontInfo h;
	h.font = *this;
	return h;
}

}
