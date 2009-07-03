#include "FontInfo.h"

#ifdef PLATFORM_WIN32

HFONT CreateWin32Font(Font font, int angle, int chrset)
{
#ifdef PLATFORM_WINCE
	LOGFONT lfnt;
	Zero(lfnt);
	lfnt.lfHeight = font.GetHeight() ? -abs(font.GetHeight()) : -12;
	lfnt.lfWeight = font.IsBold() ? FW_BOLD : FW_NORMAL;
	lfnt.lfItalic = font.IsItalic();
	lfnt.lfUnderline = font.IsUnderline();
	lfnt.lfStrikeOut = font.IsStrikeout();
	wcscpy(lfnt.lfFaceName, ToSystemCharset(font.GetFaceName()));
	return CreateFontIndirect(&lfnt);
#else
	return CreateFont(
		font.GetHeight() ? -abs(font.GetHeight()) : -12,
		font.GetWidth(), angle, angle, font.IsBold() ? FW_BOLD : FW_NORMAL,
		font.IsItalic(), font.IsUnderline(), font.IsStrikeout(),
		chrset,
		font.IsTrueTypeOnly() ? OUT_TT_ONLY_PRECIS : OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		font.IsNonAntiAliased() ? NONANTIALIASED_QUALITY : DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE,
		font.GetFaceName()
	);
#endif
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

static HDC Win32_IC()
{
	static HDC hdc;
	ONCELOCK {
		hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
	}
	return hdc;
}

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
	static Font      last;
	static GlyphInfo li[256];

	static int       lastpage = -1;
	
	int page = chr >> 8;
	if(font != last || page != lastpage) {
		last = font;
		lastpage = page;
		HFONT hfont = CreateWin32Font(font, 0, 0);
		if(!hfont) {
			GlyphInfo n;
			memset(&n, 0, sizeof(GlyphInfo));
			return n;
		}
		HDC hdc = Win32_IC();
		HFONT ohfont = (HFONT) ::SelectObject(hdc, hfont);
		int from = page << 8;
		GlyphInfo *t = li;
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
				li[i].width = 0x8000;
				li[i].lspc = li[i].rspc = 0;
			}
		::SelectObject(hdc, ohfont);
#endif
	}
	return li[chr & 255];
}

CommonFontInfo GetFontInfoSys(Font font)
{
	CommonFontInfo fi;
	memset(&fi, 0, sizeof(fi));
	HFONT hfont = CreateWin32Font(font, 0, 0);
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
	if(logfont->lfCharSet == SYMBOL_CHARSET)
		typ |= Font::SYMBOLTYPE;
	else
	if(logfont->lfCharSet != 0)
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

#ifdef PLATFORM_WINCE
	HDC hdc = CreateDC(NULL, NULL, NULL, NULL);
	Win32_ForceFace(hdc, "Tahoma", NULL);
	Win32_ForceFace(hdc, "Tahoma", "Tahoma");
	Win32_ForceFace(hdc, "Courier New", "Tahoma");
	Win32_ForceFace(hdc, "Tahoma", "Tahoma");
	Win32_ForceFace(hdc, "Tahoma", "Tahoma");
	Win32_ForceFace(hdc, "Tahoma", "Tahoma");
	Win32_ForceFace(hdc, "Tahoma", "Tahoma");
	Win32_ForceFace(hdc, "Tahoma", "Tahoma");
	Win32_ForceFace(hdc, "Tahoma", "Tahoma");
#else
	HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
	Win32_ForceFace(hdc, "Arial", NULL);
	Win32_ForceFace(hdc, "Times New Roman", "Arial");
	Win32_ForceFace(hdc, "Courier New", "Arial");
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

#endif
