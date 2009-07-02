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

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
	static Font      last;
	static GlyphInfo li[256];
	static int       lastpage = -1;
	
	int page = chr >> 8;
	if(page == lastpage)
		return li[chr & 255];
	
	HFONT hfont = CreateWin32Font(font, 0, 0);
	if(!hfont) {
		GlyphInfo n;
		memset(&n, 0, sizeof(GlyphInfo));
		return n;
	}
	HDC hdc = ScreenHDC();
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
	::SelectObject(hdc, ohfont);
	return li[chr & 255];
}

CommonFontInfo GetFontInfoSys(Font font)
{
	CommonFontInfo fi;
	memset(&fi, 0, sizeof(fi));
	HFONT hfont = CreateWin32Font(font, 0, 0);
	if(hfont) {
		HDC hdc = ScreenHDC();
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

#endif
