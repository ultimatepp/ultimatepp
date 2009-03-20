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

double fx_to_dbl(const FIXED& p) {
	return double(p.value) + double(p.fract) * (1.0 / 65536.0);
}

Pointf fx_to_dbl(const Pointf& pp, const POINTFX& p) {
	return Pointf(pp.x + fx_to_dbl(p.x), pp.y - fx_to_dbl(p.y));
}

void RenderCharPath(const char* gbuf, unsigned total_size, Painter& sw, double xx, double yy)
{
	PAINTER_TIMING("RenderCharPath");
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

void PaintCharacter(Painter& sw, const Pointf& p, int ch, Font fnt)
{
	PAINTER_TIMING("CharacterOp");
	static ScreenDraw w;
	w.SetFont(fnt);
	GLYPHMETRICS gm;
	MAT2 m_matrix;
	memset(&m_matrix, 0, sizeof(m_matrix));
	m_matrix.eM11.value = 1;
	m_matrix.eM22.value = 1;
	int gsz = GetGlyphOutlineW(w.GetHandle(), ch, GGO_NATIVE, &gm, 0, NULL, &m_matrix);
	if(gsz < 0)
		return;
	StringBuffer gb(gsz);
	gsz = GetGlyphOutlineW(w.GetHandle(), ch, GGO_NATIVE, &gm, gsz, ~gb, &m_matrix);
	if(gsz < 0)
		return;
	RenderCharPath(~gb, gsz, sw, p.x, p.y + fnt.Info().GetAscent());
	sw.EvenOdd(true);
}

#endif

END_UPP_NAMESPACE
