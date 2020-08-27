#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#ifdef PLATFORM_WIN32

namespace Upp {
HFONT GetWin32Font(Font fnt, int angle);
};

int GlyphIndex(HDC hdc, int ch)
{
	WString h = ToUtf16(ch);
	GCP_RESULTSW gcp_results = { sizeof(GCP_RESULTS) };
	gcp_results.nGlyphs = 4;
	wchar gi[4];
	gcp_results.lpGlyphs = gi;
	GetCharacterPlacementW(hdc, ~h, h.GetCount(), 0, &gcp_results, GCP_GLYPHSHAPE);
	return (word)gi[0];
}

int GlyphIndex(Font fnt, int ch)
{
	RTIMING("GlyphIndex");
	static HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
	static Font current = Null;
	if(current == ch)
		return GlyphIndex(hdc, ch);
	HFONT hfont = GetWin32Font(fnt, 0);
	if(hfont) {
		current = fnt;
		::SelectObject(hdc, hfont);
		return GlyphIndex(hdc, ch);
	}
	return -1;
}

bool HasCodepoint(Font fnt, int ch)
{
	static Vector<int> tofu;
	int& q = tofu.At(fnt.GetFace(), Null);
	if(IsNull(q))
		q = GlyphIndex(fnt, 1); // this is 99.99% to produce tofu
	return ch == ' ' || GlyphIndex(fnt, ch) != q;
}

int GetAdvanceWidth(Font fnt, int ch)
{
	TIMING("Glyph");
	HFONT hfont = GetWin32Font(fnt, 0);
	VERIFY(hfont);
	int r = -1;
	if(hfont) {
		HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
		HFONT ohfont = (HFONT) ::SelectObject(hdc, hfont);
		GLYPHMETRICS gm;
		memset(&gm, 0, sizeof(gm));
		MAT2 m_matrix;
		memset8(&m_matrix, 0, sizeof(m_matrix));
		m_matrix.eM11.value = 1;
		m_matrix.eM22.value = 1;
		ch = GlyphIndex(hdc, ch);
		int gsz = GetGlyphOutlineW(hdc, ch, GGO_NATIVE|GGO_METRICS|GGO_GLYPH_INDEX, &gm, 0, NULL, &m_matrix);
		if(gsz != GDI_ERROR && gm.gmCellIncX != 75)
			r = gm.gmCellIncX;
		::SelectObject(hdc, ohfont);
		::DeleteDC(hdc);
	}
	return r;
}

double fx_to_dbl(const FIXED& p) {
	return double(p.value) + double(p.fract) * (1.0 / 65536.0);
}

Pointf fx_to_dbl(const Pointf& pp, const POINTFX& p) {
	return Pointf(pp.x + fx_to_dbl(p.x), pp.y - fx_to_dbl(p.y));
}

void RenderCharPath2(const char* gbuf, unsigned total_size, FontGlyphConsumer& sw, double xx, double yy)
{
	const char* cur_glyph = gbuf;
	const char* end_glyph = gbuf + total_size;
	Pointf pp(xx, yy);
	while(cur_glyph < end_glyph) {
		const TTPOLYGONHEADER* th = (TTPOLYGONHEADER*)cur_glyph;
		const char* end_poly = cur_glyph + th->cb;
		const char* cur_poly = cur_glyph + sizeof(TTPOLYGONHEADER);
		sw.Move(fx_to_dbl(pp, th->pfxStart));
		while(cur_poly < end_poly) {
			const TTPOLYCURVE* pc = (const TTPOLYCURVE*)cur_poly;
			if (pc->wType == TT_PRIM_LINE)
				for(int i = 0; i < pc->cpfx; i++)
					sw.Line(fx_to_dbl(pp, pc->apfx[i]));
			if (pc->wType == TT_PRIM_QSPLINE)
				for(int u = 0; u < pc->cpfx - 1; u++) {
					Pointf b = fx_to_dbl(pp, pc->apfx[u]);
					Pointf c = fx_to_dbl(pp, pc->apfx[u + 1]);
					if (u < pc->cpfx - 2)
						c = Mid(b, c);
					sw.Quadratic(b, c);
				}
			cur_poly += sizeof(WORD) * 2 + sizeof(POINTFX) * pc->cpfx;
		}
		sw.Close();
		cur_glyph += th->cb;
    }
}

void RenderCodepoint(FontGlyphConsumer& sw, double x, double y, int ch, Font fnt)
{
	HFONT hfont = GetWin32Font(fnt, 0);
	VERIFY(hfont);
	if(hfont) {
		HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
		HFONT ohfont = (HFONT) ::SelectObject(hdc, hfont);
		GLYPHMETRICS gm;
		MAT2 m_matrix;
		memset8(&m_matrix, 0, sizeof(m_matrix));
		m_matrix.eM11.value = 1;
		m_matrix.eM22.value = 1;
		ch = GlyphIndex(hdc, ch);
		int gsz = GetGlyphOutlineW(hdc, ch, GGO_NATIVE|GGO_UNHINTED|GGO_GLYPH_INDEX, &gm, 0, NULL, &m_matrix);
		if(gsz >= 0) {
			StringBuffer gb(gsz);
			gsz = GetGlyphOutlineW(hdc, ch, GGO_NATIVE|GGO_UNHINTED|GGO_GLYPH_INDEX, &gm, gsz, ~gb, &m_matrix);
			if(gsz >= 0)
				RenderCharPath2(~gb, gsz, sw, x, y + fnt.GetAscent());
		}
		::SelectObject(hdc, ohfont);
		::DeleteDC(hdc);
	}
}

void DrawCodepoint(Draw& w, int x, int y, int ch, Font font)
{
	if(SystemDraw *sw = dynamic_cast<SystemDraw *>(&w)) {
		WString h = ToUtf16(ch);
		HFONT hfont = GetWin32Font(font, 0);
		HDC hdc = sw->BeginGdi();
		HGDIOBJ orgfont = ::SelectObject(hdc, hfont);
		::SetTextColor(hdc, sw->GetColor(Black()));
		::ExtTextOutW(hdc, 100, 200, 0, NULL, ~h, 2, NULL);
		::SelectObject(hdc, orgfont);
		sw->EndGdi();
	}
}

#endif
