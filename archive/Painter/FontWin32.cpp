//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.4
// Copyright (C) 2002-2005 Maxim Shemanarev (http://www.antigrain.com)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all coM_PIes. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------

// Recycled for U++ by Miroslav Fidler 2008

#include "Painter.h"

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

inline double fx_to_dbl(const FIXED& p) {
	return double(p.value) + double(p.fract) * (1.0 / 65536.0);
}

void RenderCharPath(const char* gbuf, unsigned total_size, Painter& sw, double xx, double yy)
{
	const char* cur_glyph = gbuf;
	const char* end_glyph = gbuf + total_size;
	
	while(cur_glyph < end_glyph) {
		const TTPOLYGONHEADER* th = (TTPOLYGONHEADER*)cur_glyph;
		const char* end_poly = cur_glyph + th->cb;
		const char* cur_poly = cur_glyph + sizeof(TTPOLYGONHEADER);
		sw.Move(xx + fx_to_dbl(th->pfxStart.x), yy - fx_to_dbl(th->pfxStart.y));
		while(cur_poly < end_poly) {
			const TTPOLYCURVE* pc = (const TTPOLYCURVE*)cur_poly;
			if (pc->wType == TT_PRIM_LINE)
				for(int i = 0; i < pc->cpfx; i++)
					sw.Line(xx + fx_to_dbl(pc->apfx[i].x), yy - fx_to_dbl(pc->apfx[i].y));
			if (pc->wType == TT_PRIM_QSPLINE) {
				int u;
				for (u = 0; u < pc->cpfx - 1; u++) { // Walk through points in spline
					POINTFX pnt_b = pc->apfx[u];     // B is always the current point
					POINTFX pnt_c = pc->apfx[u+1];
					if (u < pc->cpfx - 2) {          // If not on last spline, compute C
						*(int*)&pnt_c.x = (*(int*)&pnt_b.x + *(int*)&pnt_c.x) / 2;
						*(int*)&pnt_c.y = (*(int*)&pnt_b.y + *(int*)&pnt_c.y) / 2;
					}
					sw.Quadratic(xx + fx_to_dbl(pnt_b.x), yy - fx_to_dbl(pnt_b.y),
					             xx + fx_to_dbl(pnt_c.x), yy - fx_to_dbl(pnt_c.y));
				}
			}
			cur_poly += sizeof(WORD) * 2 + sizeof(POINTFX) * pc->cpfx;
		}
		sw.Close();
		cur_glyph += th->cb;
    }
}

struct FontChar {
	Font fnt;
	int  chr;
	
	bool operator==(const FontChar& b) const { return fnt == b.fnt && chr == b.chr; }
	unsigned GetHashValue() const            { return CombineHash(fnt, chr); }
};

struct sMakeCharOutline : LRUCache<String, FontChar>::Maker {
	FontChar fc;

	FontChar Key() const     { return fc; }
	int      Make(String& s) const {
		static ScreenDraw w;
		w.SetFont(fc.fnt);
		GLYPHMETRICS gm;
		MAT2 m_matrix;
	    memset(&m_matrix, 0, sizeof(m_matrix));
	    m_matrix.eM11.value = 1;
	    m_matrix.eM22.value = 1;
   		s.Clear();
		int gsz = GetGlyphOutlineW(w.GetHandle(), fc.chr, GGO_NATIVE, &gm, 0, NULL, &m_matrix);
		if(gsz < 0)
			return 0;
		StringBuffer gb(gsz);
		gsz = GetGlyphOutlineW(w.GetHandle(), fc.chr, GGO_NATIVE, &gm, gsz, ~gb, &m_matrix);
		if(gsz < 0)
			return 0;
		s = gb;
		return gsz;
	}
};

void Painter::CharacterOp(double x, double y, int ch, Font fnt)
{
	PAINTER_TIMING("CharacterOp");
	String s;
	INTERLOCKED {
		static LRUCache<String, FontChar> cache;
		cache.Shrink(100000);
		sMakeCharOutline h;
		h.fc.fnt = fnt;
		h.fc.chr = ch;
		s = cache.Get(h);
	}
	RenderCharPath(s, s.GetLength(), *this, x, y + fnt.Info().GetAscent());
	EvenOdd(true);
}

#endif

END_UPP_NAMESPACE
