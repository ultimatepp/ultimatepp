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

#ifdef PLATFORM_POSIX
#include <fontconfig/fontconfig.h>
#include <fontconfig/fcfreetype.h>

namespace Upp {

FT_Face FTFace(Font fnt, String *rpath);

static inline double ft_dbl(int p)
{
    return double(p) / 64.0;
}

bool RenderOutline(const FT_Outline& outline, Painter& path, double xx, double yy)
{
	FT_Vector   v_last;
	FT_Vector   v_control;
	FT_Vector   v_start;
	FT_Vector*  point;
	FT_Vector*  limit;
	char*       tags;
	int   n;         // index of contour in outline
	char  tag;       // current point's state
	int   first = 0; // index of first point in contour
	for(n = 0; n < outline.n_contours; n++) {
		int  last = outline.contours[n];
		limit = outline.points + last;
		v_start = outline.points[first];
		v_last  = outline.points[last];
		v_control = v_start;
		point = outline.points + first;
		tags  = outline.tags  + first;
		tag   = FT_CURVE_TAG(tags[0]);
		if(tag == FT_CURVE_TAG_CUBIC) return false;
		if(tag == FT_CURVE_TAG_CONIC) {
			if(FT_CURVE_TAG(outline.tags[last]) == FT_CURVE_TAG_ON) {
				// start at last point if it is on the curve
				v_start = v_last;
				limit--;
			}
			else {
				// if both first and last points are conic,
				// start at their middle and record its position
				// for closure
				v_start.x = (v_start.x + v_last.x) / 2;
				v_start.y = (v_start.y + v_last.y) / 2;
				v_last = v_start;
			}
			point--;
			tags--;
		}
		path.Move(ft_dbl(v_start.x) + xx, -ft_dbl(v_start.y) + yy);
		while(point < limit) {
			point++;
			tags++;

			tag = FT_CURVE_TAG(tags[0]);
			switch(tag) {
			case FT_CURVE_TAG_ON:
				path.Line(ft_dbl(point->x) + xx, -ft_dbl(point->y) + yy);
				continue;
			case FT_CURVE_TAG_CONIC:
				v_control.x = point->x;
				v_control.y = point->y;
			Do_Conic:
				if(point < limit) {
					FT_Vector vec;
					FT_Vector v_middle;
					point++;
					tags++;
					tag = FT_CURVE_TAG(tags[0]);
					vec.x = point->x;
					vec.y = point->y;
					if(tag == FT_CURVE_TAG_ON) {
						path.Quadratic(ft_dbl(v_control.x) + xx, -ft_dbl(v_control.y) + yy,
						               ft_dbl(vec.x) + xx, -ft_dbl(vec.y) + yy);
						continue;
					}
					if(tag != FT_CURVE_TAG_CONIC) return false;
					v_middle.x = (v_control.x + vec.x) / 2;
					v_middle.y = (v_control.y + vec.y) / 2;
					path.Quadratic(ft_dbl(v_control.x) + xx, -ft_dbl(v_control.y) + yy,
					               ft_dbl(v_middle.x) + xx, -ft_dbl(v_middle.y) + yy);
					v_control = vec;
					goto Do_Conic;
				}
				path.Quadratic(ft_dbl(v_control.x) + xx, -ft_dbl(v_control.y) + yy,
				               ft_dbl(v_start.x) + xx, -ft_dbl(v_start.y) + yy);
				goto Close;

			default:
				FT_Vector vec1, vec2;
				if(point + 1 > limit || FT_CURVE_TAG(tags[1]) != FT_CURVE_TAG_CUBIC)
				    return false;
				vec1.x = point[0].x; 
				vec1.y = point[0].y;
				vec2.x = point[1].x; 
				vec2.y = point[1].y;
				point += 2;
				tags  += 2;
				if(point <= limit) {
					FT_Vector vec;
					vec.x = point->x;
					vec.y = point->y;
					path.Cubic(ft_dbl(vec1.x) + xx, -ft_dbl(vec1.y) + yy,
					           ft_dbl(vec2.x) + xx, -ft_dbl(vec2.y) + yy,
					           ft_dbl(vec.x) + xx, -ft_dbl(vec.y) + yy);
					continue;
				}
				path.Cubic(ft_dbl(vec1.x) + xx, -ft_dbl(vec1.y) + yy,
				           ft_dbl(vec2.x) + xx, -ft_dbl(vec2.y) + yy,
				           ft_dbl(v_start.x) + xx, -ft_dbl(v_start.y) + yy);
				goto Close;
			}
		}
	Close:
		path.Close();
		first = last + 1; 
    }
	return true;
}

void PaintCharacterSys(Painter& sw, double x, double y, int ch, Font fnt)
{
	DrawLock __;
	PAINTER_TIMING("CharacterOp");
	FT_Face face = FTFace(fnt, NULL);
	int glyph_index = FT_Get_Char_Index(face, ch);
	if(glyph_index && FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT) == 0)
		RenderOutline(face->glyph->outline, sw, x, y + fnt.GetAscent());
	sw.EvenOdd(true);
}

}

#endif
