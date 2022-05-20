#include "FT_fontsys.h"

#include <plugin/FT_fontsys/ft2build.h>

#include <plugin/FT_fontsys/ftoutln.h>

#define LLOG(x)     //  DLOG(x)
#define LTIMING(x)  //  TIMING(x)

namespace Upp {

struct FtFontStyle {
	String      path;
	const byte *data;
	int         size;
	
	operator bool() const { return path.GetCount() || data; }
	
	FtFontStyle()         { data = NULL; size = 0; }
};

struct FtFontData : Moveable<FtFontData> {
	FtFontStyle style[4];
};

static Vector<FtFontData>& sFontData()
{
	return Single<Vector<FtFontData> >();
}

void sSetFont(int index, const String& path, const byte *data, int size, dword style)
{
	Vector<FtFontData>& fd = sFontData();
	FtFontStyle& f = fd.At(index).style[style & 3];
	f.path = path;
	f.data = data;
	f.size = size;
	if(!fd.At(index).style[0])
		sSetFont(index, path, data, size, 0);
	if(!fd.At(0).style[0])
		sSetFont(0, path, data, size, 0);
	InvalidateFontList();
}

void SetFileFont(int face, const char *path, dword style)
{
	sSetFont(face, path, NULL, 0, style);
}

void SetMemoryFont(int face, const byte *data, int size, dword style)
{
	sSetFont(face, Null, data, size, style);
}

void GetStdFontSys(String& name, int& height)
{
	name = "xxxx";
}

FtFontStyle GetFontStyle(Font font)
{
	Vector<FtFontData>& fd = sFontData();
	int face = font.GetFace();
	if(face >= 0 && face < fd.GetCount()) {
		const FtFontData& d = fd[face];
		int ii = font.IsBold() + 2 * font.IsItalic();
		if(d.style[ii])
			return d.style[ii];
		if(d.style[0])
			return d.style[0];
	}
	return fd.At(0).style[0];
}

extern String GetFontDataSysSys(Stream& in, int fonti, const char *table, int offset, int size);

String GetFontDataSys(Font font, const char *table, int offset, int size)
{
	FtFontStyle f = GetFontStyle(font);
	
	if(IsNull(f.path)) {
		if(table) {
			MemReadStream in(f.data, f.size);
			return GetFontDataSysSys(in, 0, table, offset, size);
		}
		return String(f.data, min((int)f.size, 100*1024*1024));
	}
	if(table) {
		FileIn in(f.path);
		return GetFontDataSysSys(in, 0, table, offset, size);
	}
	return LoadFile(f.path);
}

static FT_Library sFTlib;

EXITBLOCK
{
	if(sFTlib)
		FT_Done_FreeType(sFTlib);
}

bool sInitFt(void)
{
	if(sFTlib)
		return true;
	return FT_Init_FreeType(&sFTlib) == 0;
}

void FT_ITALIC(FT_GlyphSlot slot)
{
	FT_Matrix    transform;
	FT_Outline*  outline = &slot->outline;
	/* only oblique outline glyphs */
	if ( slot->format != FT_GLYPH_FORMAT_OUTLINE )
	  return;
	
	/* we don't touch the advance width */
	/* For italic, simply apply a shear transform, with an angle */
	/* of about 12 degrees.                                      */
	
	transform.xx = 0x10000L;
	transform.yx = 0x00000L;
	
	transform.xy = 0x03000L;
	transform.yy = 0x10000L;
	
	FT_Outline_Transform( outline, &transform );
}

void FT_BOLD(FT_GlyphSlot slot)
{
    FT_Face     face = slot->face;
    FT_Pos      amount;

    if(slot->format != FT_GLYPH_FORMAT_OUTLINE)
      return;

    /* some reasonable strength */
    amount = FT_MulFix(face->units_per_EM,
                       face->size->metrics.y_scale ) / 18;
	(void)FT_Outline_Embolden( &slot->outline, amount);

    if ( slot->advance.x )
      slot->advance.x += amount;

    if ( slot->advance.y )
      slot->advance.y += amount;

    slot->metrics.width        += amount;
    slot->metrics.height       += amount;
    slot->metrics.horiBearingY += amount;
    slot->metrics.horiAdvance  += amount;
    slot->metrics.vertBearingX -= amount / 2;
    slot->metrics.vertBearingY += amount;
    slot->metrics.vertAdvance  += amount;
}

FT_Face CreateFTFace(Font fnt)
{
	sInitFt();

	FtFontStyle f = GetFontStyle(fnt);
	
	if(!f)
		return NULL;

	FT_Face face;
	if(IsNull(f.path) ? FT_New_Memory_Face(sFTlib, f.data, f.size, 0, &face)
	                  : FT_New_Face(sFTlib, f.path, 0, &face))
		return NULL;

	FT_Set_Pixel_Sizes(face, 0, fnt.GetHeight());
	return face;
}

#define FONTCACHE 37

struct FtFaceEntry {
	Font    font;
	FT_Face face;
};

static FtFaceEntry ft_cache[FONTCACHE];

void ClearFtFaceCache()
{
	for(int i = 0; i < FONTCACHE; i++)
		ft_cache[i].font.Height(-30000);
}

FT_Face FTFace(Font fnt)
{
	LTIMING("FTFace");
	LLOG("FTFace " << fnt);
	ONCELOCK {
		ClearFtFaceCache();
	}
	FtFaceEntry be;
	be = ft_cache[0];
	for(int i = 0; i < FONTCACHE; i++) {
		FtFaceEntry e = ft_cache[i];
		if(i)
			ft_cache[i] = be;
		if(e.font == fnt && e.face) {
			if(i)
				ft_cache[0] = e;
			LLOG("Found " << e.face);
			return e.face;
		}
		be = e;
	}
	LTIMING("FTFace2");
	if(be.face) {
		LLOG("Removing " << be.font << " - " << (void *)be.face);
		FT_Done_Face(be.face);
	}
	be.font = fnt;
	be.face = CreateFTFace(be.font);
	ft_cache[0] = be;
	LLOG("Created " << be.face);
	return be.face;
}

CommonFontInfo GetFontInfoSys(Font font)
{
	sInitFt();
	CommonFontInfo fi;
	FT_Face face = FTFace(font);
	if(face) {
		fi.ascent = face->size->metrics.ascender >> 6;
		fi.descent = -(face->size->metrics.descender >> 6);
		fi.external = (face->size->metrics.height >> 6) - fi.ascent - fi.descent;
		fi.internal = 0;
		fi.overhang = 0;
		fi.maxwidth = face->size->metrics.max_advance >> 6;
		fi.avewidth = fi.maxwidth;
		fi.default_char = '?';
		fi.fixedpitch = font.GetFaceInfo() & Font::FIXEDPITCH;
		fi.ttf = true;//strcmp(FT_Get_X11_Font_Format(face), "TrueType") == 0; // ????
	}
	else
		memset(&fi, 0, sizeof(fi));

	return fi;
}

#define FLOOR(x)    ((x) & -64)
#define CEIL(x)	    (((x)+63) & -64)
#define TRUNC(x)    ((x) >> 6)
#define ROUND(x)    (((x)+32) & -64)

static bool sLoadGlyph(Font fnt, FT_Face face, int chr)
{
	int glyph_index = FT_Get_Char_Index(face, chr);
	if(!glyph_index)
		return false;
	if(FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP))
		return false;
	if(fnt.IsBold() && !(face->style_flags & FT_STYLE_FLAG_BOLD))
		FT_BOLD(face->glyph);
	if(fnt.IsItalic() && !(face->style_flags & FT_STYLE_FLAG_ITALIC))
		FT_ITALIC(face->glyph);
	return true;
}

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
	LTIMING("GetGlyphInfoSys");
	GlyphInfo gi;
	FT_Face face = FTFace(font);
	gi.lspc = gi.rspc = 0;
	gi.width = (int16)0x8000;
	LLOG("GetGlyphInfoSys " << font << " " << (char)chr << " " << FormatIntHex(chr));
	if(face) {
		LTIMING("GetGlyphInfoSys 2");
		if(sLoadGlyph(font, face, chr)) {
			FT_Glyph_Metrics& m = face->glyph->metrics;
			int left  = FLOOR(m.horiBearingX);
			int width = TRUNC(CEIL(m.horiBearingX + m.width) - left);				
			gi.width = TRUNC(ROUND(face->glyph->advance.x));
			gi.lspc = TRUNC(left);
			gi.rspc = gi.width - width - gi.lspc;
		}
	}
	return gi;
}

Vector<FaceInfo> GetAllFacesSys()
{
	static const char *basic_fonts[] = {
		"sans-serif",
		"serif",
		"sans-serif",
		"monospace",
	};
	
	Vector<FaceInfo> list;
	for(int i = 0; i < __countof(basic_fonts); i++) {
		FaceInfo& fi = list.Add();
		fi.name = basic_fonts[i];
		fi.info = (i == 3) ? Font::SCALEABLE|Font::FIXEDPITCH : Font::SCALEABLE;
	}

//	const Vector<FtFontData>& fd = sFontData();
//	for(int i = __countof(basic_fonts); i < fd.GetCount(); i++) {
//		FaceInfo& fi = list.Add();
//	}

	return list;
}

static inline double ft_dbl(int p)
{
    return double(p) / 64.0;
}

bool RenderOutline(const FT_Outline& outline, FontGlyphConsumer& path, double xx, double yy)
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
		path.Move(Pointf(ft_dbl(v_start.x) + xx, -ft_dbl(v_start.y) + yy));
		while(point < limit) {
			point++;
			tags++;

			tag = FT_CURVE_TAG(tags[0]);
			switch(tag) {
			case FT_CURVE_TAG_ON:
				path.Line(Pointf(ft_dbl(point->x) + xx, -ft_dbl(point->y) + yy));
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
						path.Quadratic(Pointf(ft_dbl(v_control.x) + xx, -ft_dbl(v_control.y) + yy),
						               Pointf(ft_dbl(vec.x) + xx, -ft_dbl(vec.y) + yy));
						continue;
					}
					if(tag != FT_CURVE_TAG_CONIC) return false;
					v_middle.x = (v_control.x + vec.x) / 2;
					v_middle.y = (v_control.y + vec.y) / 2;
					path.Quadratic(Pointf(ft_dbl(v_control.x) + xx, -ft_dbl(v_control.y) + yy),
					               Pointf(ft_dbl(v_middle.x) + xx, -ft_dbl(v_middle.y) + yy));
					v_control = vec;
					goto Do_Conic;
				}
				path.Quadratic(Pointf(ft_dbl(v_control.x) + xx, -ft_dbl(v_control.y) + yy),
				               Pointf(ft_dbl(v_start.x) + xx, -ft_dbl(v_start.y) + yy));
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
					path.Cubic(Pointf(ft_dbl(vec1.x) + xx, -ft_dbl(vec1.y) + yy),
					           Pointf(ft_dbl(vec2.x) + xx, -ft_dbl(vec2.y) + yy),
					           Pointf(ft_dbl(vec.x) + xx, -ft_dbl(vec.y) + yy));
					continue;
				}
				path.Cubic(Pointf(ft_dbl(vec1.x) + xx, -ft_dbl(vec1.y) + yy),
				           Pointf(ft_dbl(vec2.x) + xx, -ft_dbl(vec2.y) + yy),
				           Pointf(ft_dbl(v_start.x) + xx, -ft_dbl(v_start.y) + yy));
				goto Close;
			}
		}
	Close:
		path.Close();
		first = last + 1;
    }
	return true;
}

void RenderCharacterSys(FontGlyphConsumer& sw, double x, double y, int ch, Font fnt)
{
	FT_Face face = FTFace(fnt);
	if(sLoadGlyph(fnt, face, ch))
		RenderOutline(face->glyph->outline, sw, x, y + fnt.GetAscent());
}

}
