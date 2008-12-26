#include "SDraw.h"

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
//
// Arc generator. Produces at most 4 consecutive cubic bezier curves, i.e., 
// 4, 7, 10, or 13 vertices.
//
//----------------------------------------------------------------------------

// Recycled for U++ by Miroslav Fidler 2008

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

inline double fx_to_dbl(const FIXED& p) {
	return double(p.value) + double(p.fract) * (1.0 / 65536.0);
}

void RenderCharPath(const char* gbuf, unsigned total_size, SDraw& sw)
{
	bool flip_y = true;
    const char* cur_glyph = gbuf;
    const char* end_glyph = gbuf + total_size;
    double x, y;
    
    while(cur_glyph < end_glyph)
    {
        const TTPOLYGONHEADER* th = (TTPOLYGONHEADER*)cur_glyph;
        
        const char* end_poly = cur_glyph + th->cb;
        const char* cur_poly = cur_glyph + sizeof(TTPOLYGONHEADER);

        x = fx_to_dbl(th->pfxStart.x);
        y = fx_to_dbl(th->pfxStart.y);
        if(flip_y) y = -y;
        sw.MoveTo(x, y);

        while(cur_poly < end_poly)
        {
            const TTPOLYCURVE* pc = (const TTPOLYCURVE*)cur_poly;
            
            if (pc->wType == TT_PRIM_LINE)
            {
                int i;
                for (i = 0; i < pc->cpfx; i++)
                {
                    x = fx_to_dbl(pc->apfx[i].x);
                    y = fx_to_dbl(pc->apfx[i].y);
                    if(flip_y) y = -y;
                    sw.LineTo(x, y);
                }
            }
            
            if (pc->wType == TT_PRIM_QSPLINE)
            {
                int u;
                for (u = 0; u < pc->cpfx - 1; u++)  // Walk through points in spline
                {
                    POINTFX pnt_b = pc->apfx[u];    // B is always the current point
                    POINTFX pnt_c = pc->apfx[u+1];
                    
                    if (u < pc->cpfx - 2)           // If not on last spline, compute C
                    {
                        // midpoint (x,y)
                        *(int*)&pnt_c.x = (*(int*)&pnt_b.x + *(int*)&pnt_c.x) / 2;
                        *(int*)&pnt_c.y = (*(int*)&pnt_b.y + *(int*)&pnt_c.y) / 2;
                    }
                    
                    double x2, y2;
                    x  = fx_to_dbl(pnt_b.x);
                    y  = fx_to_dbl(pnt_b.y);
                    x2 = fx_to_dbl(pnt_c.x);
                    y2 = fx_to_dbl(pnt_c.y);
                    if(flip_y) { y = -y; y2 = -y2; }
                    sw.Quadratic(x, y, x2, y2);
                }
            }
            cur_poly += sizeof(WORD) * 2 + sizeof(POINTFX) * pc->cpfx;
        }
        cur_glyph += th->cb;
    }
}

void RenderCharacter(SDraw& sw, int ch, Font fnt)
{
	ScreenDraw w;
	w.SetFont(fnt);
	GLYPHMETRICS gm;
	MAT2 m_matrix;
    memset(&m_matrix, 0, sizeof(m_matrix));
    m_matrix.eM11.value = 1;
    m_matrix.eM22.value = 1;
	int gsz = GetGlyphOutlineW(w.GetHandle(), ch, GGO_NATIVE, &gm, 0, NULL, &m_matrix);
	if(gsz < 0)
		return;
	Buffer<char> gb(gsz);
	gsz = GetGlyphOutlineW(w.GetHandle(), ch, GGO_NATIVE, &gm, gsz, gb, &m_matrix);
	if(gsz < 0)
		return;
	RenderCharPath(gb, gsz, sw);
}

#endif

END_UPP_NAMESPACE
