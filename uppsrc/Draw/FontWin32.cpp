#include "Draw.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#define LLOG(x)     //  LOG(x)
#define LTIMING(x)  //  TIMING(x)

void GetStdFontSys(String& name, int& height)
{
#ifdef PLATFORM_WINCE
	name = "Arial";
	height = 10;
#else
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
	name = ncm.lfMenuFont.lfFaceName;
	height = abs((int)ncm.lfMenuFont.lfHeight);
#endif
}

#define FONTCACHE 96

struct HFontEntry {
	Font    font;
	HFONT   hfont;
	int     angle;
};

HFONT GetWin32Font(Font fnt, int angle)
{
	LTIMING("GetWin32Font");
	static HFontEntry cache[FONTCACHE];
	ONCELOCK {
		for(int i = 0; i < FONTCACHE; i++)
			cache[i].font.Height(-30000);
	}
	HFontEntry be;
	be = cache[0];
	for(int i = 0; i < FONTCACHE; i++) {
		HFontEntry e = cache[i];
		if(i)
			cache[i] = be;
		if(e.font == fnt && e.angle == angle) {
			if(i)
				cache[0] = e;
			return e.hfont;
		}
		be = e;
	}
	LTIMING("GetWin32Font2");
	if(be.hfont)
		DeleteObject(be.hfont);

	be.font = fnt;
	be.angle = angle;
#ifdef PLATFORM_WINCE
	LOGFONT lfnt;
	Zero(lfnt);
	lfnt.lfHeight = fnt.GetHeight() ? -abs(fnt.GetHeight()) : -12;
	lfnt.lfWeight = fnt.IsBold() ? FW_BOLD : FW_NORMAL;
	lfnt.lfItalic = fnt.IsItalic();
	lfnt.lfUnderline = fnt.IsUnderline();
	lfnt.lfStrikeOut = fnt.IsStrikeout();
	wcscpy(lfnt.lfFaceName, ToSystemCharset(fnt.GetFaceName()));
	be.hfont = CreateFontIndirect(&lfnt);
#else
	be.hfont = CreateFont(
		fnt.GetHeight() ? -abs(fnt.GetHeight()) : -12,
		fnt.GetWidth(), angle, angle, fnt.IsBold() ? FW_BOLD : FW_NORMAL,
		fnt.IsItalic(), fnt.IsUnderline(), fnt.IsStrikeout(),
		fnt.GetFace() == Font::SYMBOL ? SYMBOL_CHARSET : DEFAULT_CHARSET,
		fnt.IsTrueTypeOnly() ? OUT_TT_ONLY_PRECIS : OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		fnt.IsNonAntiAliased() ? NONANTIALIASED_QUALITY : DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,
		fnt.GetFaceName()
	);
#endif
	cache[0] = be;
	return be.hfont;
}

int sGetCW(HDC hdc, wchar *h, int n)
{
	SIZE sz;
	return GetTextExtentPoint32W(hdc, h, n, &sz) ? sz.cx : 0;
}

static void Win32_GetGlyphIndices(HDC hdc, LPCWSTR s, int n, LPWORD r, DWORD flag)
{
	typedef DWORD (WINAPI *GGIW)(HDC, LPCWSTR, int, LPWORD, DWORD);
	static GGIW fn;
	ONCELOCK
		if(HMODULE hDLL = LoadLibrary("gdi32"))
			fn = (GGIW) GetProcAddress(hDLL, "GetGlyphIndicesW");
	if(fn)
		fn(hdc, s, n, r, flag);
	else
		memset(r, 0, n * sizeof(WORD));
}

HDC Win32_IC()
{
	static HDC hdc;
	ONCELOCK {
		hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
	}
	return hdc;
}

CommonFontInfo GetFontInfoSys(Font font)
{
	CommonFontInfo fi;
	memset(&fi, 0, sizeof(fi));
	HFONT hfont = GetWin32Font(font, 0);
	if(hfont) {
		HDC hdc = Win32_IC();
		HFONT ohfont = (HFONT) ::SelectObject(hdc, hfont);
		TEXTMETRIC tm;
		::GetTextMetrics(hdc, &tm);
		fi.ascent = tm.tmAscent;
		fi.descent = tm.tmDescent;
		fi.external = tm.tmExternalLeading;
		fi.internal = tm.tmInternalLeading;
		fi.height = fi.ascent + fi.descent;
		fi.lineheight = fi.height + fi.external;
		fi.overhang = tm.tmOverhang;
		fi.avewidth = tm.tmAveCharWidth;
		fi.maxwidth = tm.tmMaxCharWidth;
		fi.firstchar = tm.tmFirstChar;
		fi.charcount = tm.tmLastChar - tm.tmFirstChar + 1;
		fi.default_char = tm.tmDefaultChar;
		fi.fixedpitch = (tm.tmPitchAndFamily & TMPF_FIXED_PITCH) == 0;
		fi.scaleable = tm.tmPitchAndFamily & TMPF_TRUETYPE;
		if(fi.scaleable) {
			ABC abc;
			GetCharABCWidths(hdc, 'o', 'o', &abc);
			fi.spacebefore = abc.abcA;
			fi.spaceafter = abc.abcC;
		}
		else
			fi.spacebefore = fi.spaceafter = 0;
		::SelectObject(hdc, ohfont);
	}
	return fi;
}

static Vector<FaceInfo> *sList;

static int CALLBACK Win32_AddFace(const LOGFONT *logfont, const TEXTMETRIC *, dword type, LPARAM param)
{
#ifdef PLATFORM_WINCE
	const wchar *facename = (const wchar *)param;
	if(facename && _wcsicmp(logfont->lfFaceName, facename))
		return 1;
#else
	const char *facename = (const char *)param;
	if(facename && stricmp(logfont->lfFaceName, facename))
		return 1;
#endif

	dword typ = 0;
	if((logfont->lfPitchAndFamily & 3) == FIXED_PITCH)
		typ |= Font::FIXEDPITCH;
	if(type & TRUETYPE_FONTTYPE)
		typ |= Font::SCALEABLE;
	if(!(logfont->lfCharSet == SYMBOL_CHARSET) && logfont->lfCharSet != 0)
		typ |= Font::LOCAL;
#ifdef PLATFORM_WINCE
	FontFaceInfo& f = sFontFace().Add(WString(logfont->lfFaceName).ToString());
	f.name = FromSystemCharset(logfont->lfFaceName);
#else
	FaceInfo& f = sList->Add();
	f.name = FromSystemCharset(logfont->lfFaceName);
	f.info = typ;
#endif
	return facename ? 0 : 1;
}

static int Win32_EnumFace(HDC hdc, const char *face)
{
#ifdef PLATFORM_WINCE
	return EnumFontFamilies(hdc, ToSystemCharset(face), Win32_AddFace, (LPARAM)~ToSystemCharset(face));
#else
	return EnumFontFamilies(hdc, face, Win32_AddFace, (LPARAM)face);
#endif
}

static void Win32_ForceFace(HDC hdc, const char *face, const char *aface)
{
	if(!aface)
		aface = "Arial";
	if(Win32_EnumFace(hdc, face) && Win32_EnumFace(hdc, aface))
		Panic("Missing font " + String(face));
}

Vector<FaceInfo> GetAllFacesSys()
{
	Vector<FaceInfo> list;
	sList = &list;
	list.Add().name = "STDFONT";
	list.Top().info = 0;
#ifdef PLATFORM_WINCE
	HDC hdc = CreateDC(NULL, NULL, NULL, NULL);
	Win32_ForceFace(hdc, "Tahoma", NULL);
	Win32_ForceFace(hdc, "Tahoma", "Tahoma");
	Win32_ForceFace(hdc, "Courier New", "Tahoma");
	Win32_ForceFace(hdc, "Tahoma", "Tahoma");
	Win32_ForceFace(hdc, "Tahoma", "Tahoma");
	Win32_ForceFace(hdc, "Tahoma", "Tahoma");
#else
	HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
	Win32_ForceFace(hdc, "Times New Roman", "Arial");
	Win32_ForceFace(hdc, "Arial", "Arial");
	Win32_ForceFace(hdc, "Courier New", "Arial");
	Win32_ForceFace(hdc, "Symbol", "Arial");
	Win32_ForceFace(hdc, "WingDings", "Arial");
	Win32_ForceFace(hdc, "Tahoma", "Arial");
#endif
	Win32_EnumFace(hdc, NULL);
	DeleteDC(hdc);
	return list;
}

#define GLYPHINFOCACHE 31

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
	static Font      fnt[GLYPHINFOCACHE];
	static int       pg[GLYPHINFOCACHE];
	static GlyphInfo li[GLYPHINFOCACHE][256];
	
	ONCELOCK {
		for(int i = 0; i < GLYPHINFOCACHE; i++)
			pg[i] = -1;
	}
	
	int page = chr >> 8;
	int q = CombineHash(font, page) % GLYPHINFOCACHE;
	
	if(fnt[q] != font || pg[q] != page) {
		fnt[q] = font;
		pg[q] = page;
		HFONT hfont = GetWin32Font(font, 0);
		if(!hfont) {
			GlyphInfo n;
			memset(&n, 0, sizeof(GlyphInfo));
			return n;
		}
		HDC hdc = Win32_IC();
		HFONT ohfont = (HFONT) ::SelectObject(hdc, hfont);
		int from = page << 8;
		GlyphInfo *t = li[q];
		if(page >= 32) {
			wchar h[3];
			h[0] = 'x';
			h[1] = 'x';
			h[2] = 'x';
			int w0 = sGetCW(hdc, h, 2);
			for(int i = 0; i < 256; i++) {
				h[1] = from + i;
				t[i].width = sGetCW(hdc, h, 3) - w0;
				t[i].lspc = t[i].rspc = 0;
			}
		}
		else {
			bool abca = false, abcw = false;
			Buffer<ABC> abc(256);
			abcw = ::GetCharABCWidths(hdc, from, from + 256 - 1, abc);
	#ifndef PLATFORM_WINCE
			if(!abcw)
				abca = ::GetCharABCWidthsA(hdc, from, from + 256 - 1, abc);
	#endif
			if(abcw)
			{
				for(ABC *s = abc, *lim = abc + 256; s < lim; s++, t++)
				{
					t->width = s->abcA + s->abcB + s->abcC;
					t->lspc = s->abcA;
					t->rspc = s->abcC;
				}
			}
			else
			{
				Buffer<int> wb(256);
	#ifdef PLATFORM_WINCE
				::GetCharWidth32(hdc, from, from + 256 - 1, wb);
	#else
				::GetCharWidthW(hdc, from, from + 256 - 1, wb);
	#endif
				for(int *s = wb, *lim = wb + 256; s < lim; s++, t++)
				{
					t->width = *s - GetFontInfoSys(font).overhang;
					if(abca)
					{
						ABC aa = abc[(byte)ToAscii(from++)];
						t->lspc = aa.abcA;
						t->rspc = aa.abcC;
					}
					else
						t->lspc = t->rspc = 0;
				}
			}
		}
#ifndef PLATFORM_WINCE
		WORD pos[256];
		WCHAR wch[256];
		for(int i = 0; i < 256; i++)
			wch[i] = from + i;
		Win32_GetGlyphIndices(hdc, wch, 256, pos, 1);
		for(int i = 0; i < 256; i++)
			if(pos[i] == 0xffff) {
				li[q][i].width = (int16)0x8000;
				li[q][i].lspc = li[q][i].rspc = 0;
			}
#endif
	}
	return li[q][chr & 255];
}

#endif

END_UPP_NAMESPACE
