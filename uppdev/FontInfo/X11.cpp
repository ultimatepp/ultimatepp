#include "FontInfo.h"

#define LTIMING(x)

#ifdef PLATFORM_X11

static FT_Library sFTlib;

bool sInitFt(void)
{
	if(sFTlib)
		return true;
	return FT_Init_FreeType(&sFTlib) == 0;
}

FcPattern *CreateFcPattern(Font font, int angle)
{
	LTIMING("CreateXftFont");
	double sina, cosa;
	int hg = abs(font.GetHeight());
	if(hg == 0) hg = 10;
	String n = font.GetFaceName();
	const char *face = n;
	FcPattern *p = FcPatternCreate();
	FcPatternAddString(p, FC_FAMILY, (FcChar8*)face);
	FcPatternAddInteger(p, FC_SLANT, font.IsItalic() ? 110 : 0);
	FcPatternAddInteger(p, FC_PIXEL_SIZE, hg);
	FcPatternAddInteger(p, FC_WEIGHT, font.IsBold() ? 200 : 100);
	FcPatternAddBool(p, FC_MINSPACE, 1);
	if(angle) {
		FcMatrix mx;
		Draw::SinCos(angle, sina, cosa);
		mx.xx = cosa;
		mx.xy = -sina;
		mx.yx = sina;
		mx.yy = cosa;
		FcPatternAddMatrix(p, FC_MATRIX, &mx);
	}
	FcResult result;
	FcPattern *m = XftFontMatch(Xdisplay, Xscreenno, p, &result);
	FcPatternDestroy(p);
	return m;
}

FT_Face CreateFTFace(const FcPattern *pattern, String *rpath) {
	FT_Face	    face = NULL;

	int		    id;
	double	    dsize;
	double	    aspect;
	FcChar8    *filename;

	if(!sInitFt())
		return NULL;

	if(FcPatternGetString(pattern, FC_FILE, 0, &filename) != FcResultMatch)
		return NULL;

	if(rpath)
		*rpath = (const char *)filename;

	if(FcPatternGetDouble(pattern, FC_PIXEL_SIZE, 0, &dsize) != FcResultMatch)
		dsize = 16;

	if (FcPatternGetDouble(pattern, FC_ASPECT, 0, &aspect) != FcResultMatch)
		aspect = 1.0;

	FT_F26Dot6 ysize = (FT_F26Dot6) (dsize * 64.0);
	FT_F26Dot6 xsize = (FT_F26Dot6) (dsize * aspect * 64.0);

	if(FT_New_Face (sFTlib, (const char *)filename, 0, &face))
		return NULL;

	FT_Set_Char_Size(face, xsize, ysize, 0, 0);
	return face;
}

FT_Face FTFace(Font fnt, String *rpath = NULL)
{
	static FT_Face cached_face;
	static Font    cached_font;
	if(cached_font != fnt) {
		cached_font = fnt;
		if(cached_face)
			FT_Done_Face(cached_face);
		FcPattern *p = CreateFcPattern(cached_font, 0);
		cached_face = CreateFTFace(p, rpath);
		FcPatternDestroy(p);
	}
	return cached_face;
}

CommonFontInfo GetFontInfoSys(Font font)
{
	CommonFontInfo fi;

	FT_Face face = FTFace(font, &fi.path);
	if(face) {
		fi.ascent = face->size->metrics.ascender >> 6;
		fi.descent = -(face->size->metrics.descender >> 6);
		fi.height = fi.ascent + fi.descent;
		fi.lineheight = face->size->metrics.height >> 6;
		fi.external = 0;
		fi.internal = 0;
		fi.overhang = 0;
		fi.maxwidth = face->size->metrics.max_advance >> 6;
		fi.avewidth = fi.maxwidth;
		fi.default_char = '?';
		fi.fixedpitch = font.GetFaceInfo() & Font::FIXEDPITCH;
		int hg = abs(font.GetHeight());
		if(hg == 0) hg = 10;
		fi.underline_thickness = max(hg / 20, 1);
		fi.underline_position = max(hg / 15, int(fi.descent > 0));
	}
	return fi;
}

#define FLOOR(x)    ((x) & -64)
#define CEIL(x)	    (((x)+63) & -64)
#define TRUNC(x)    ((x) >> 6)
#define ROUND(x)    (((x)+32) & -64)

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
	GlyphInfo gi;
	FT_Face face = FTFace(font, NULL);
	gi.lspc = gi.rspc = 0;
	gi.width = 0x8000;
	if(face) {
		int glyph_index = FT_Get_Char_Index(face, chr);
		if(glyph_index &&
			(FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP) == 0 ||
			 FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT) == 0)) {
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
		"serif",
		"sans-serif",
		"monospace",
	};
	
	Vector<FaceInfo> list;
	for(int i = 0; i < __countof(basic_fonts); i++) {
		FaceInfo& fi = list.Add();
		f.name = basic_fonts[i];
		f.scaleable = true;
		f.fixed = i == 3 || i == 6;
		f.compose = sCheckComposed(basic_fonts[i]);
	}
	FcPattern *p = FcPatternCreate();
	FcObjectSet *os = FcObjectSetBuild(XFT_FAMILY, XFT_SPACING, XFT_SCALABLE, (void *)0);
	FcFontSet *fs = FcFontList(NULL, p, os);
	FcPatternDestroy(p);
	FcObjectSetDestroy(os);
	for(int i = 0; i < fs->nfont; i++) {
		FcChar8 *family = NULL;
		if(FcPatternGetString(fs->fonts[i], FC_FAMILY, 0, &family) == 0 && family) {
			FaceInfo& fi = list.Add();
			fi.name = (const char *)family;
			int iv;
			fi.fixed = FcPatternGetInteger(fs->fonts[i], FC_SPACING, 0, &iv) == 0 && iv == XFT_MONO;
			FcBool bv;
			fi.scalable = FcPatternGetBool(fs->fonts[i], FC_SCALABLE, 0, &bv) == 0 && bv;
		}
	}
	FcFontSetDestroy(fs);
	return list;
}

#endif
