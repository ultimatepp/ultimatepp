#include "Draw.h"

#ifdef PLATFORM_POSIX
#include <fontconfig/fontconfig.h>
#include <fontconfig/fcfreetype.h>
#endif

NAMESPACE_UPP

#ifdef PLATFORM_POSIX

#define LLOG(x)     //  LOG(x)
#define LTIMING(x)  //  TIMING(x)

void GetStdFontSys(String& name, int& height)
{
	name = "xxxx";
}

static FT_Library sFTlib;

bool sInitFt(void)
{
	if(sFTlib)
		return true;
	return FT_Init_FreeType(&sFTlib) == 0;
}

FcPattern *CreateFcPattern(Font font)
{
	LTIMING("CreateXftFont");
	double sina, cosa;
	int hg = abs(font.GetHeight());
	if(hg == 0) hg = 10;
	String face = font.GetFaceName();
	FcPattern *p = FcPatternCreate();
	FcPatternAddString(p, FC_FAMILY, (FcChar8*)~face);
	FcPatternAddInteger(p, FC_SLANT, font.IsItalic() ? 110 : 0);
	FcPatternAddInteger(p, FC_PIXEL_SIZE, hg);
	FcPatternAddInteger(p, FC_WEIGHT, font.IsBold() ? 200 : 100);
	FcPatternAddBool(p, FC_MINSPACE, 1);
	FcConfigSubstitute(0, p, FcMatchPattern);
	FcDefaultSubstitute(p);
	FcResult result;
	FcPattern *m = FcFontMatch(0, p, &result);
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

	if(FT_New_Face(sFTlib, (const char *)filename, 0, &face))
		return NULL;

	FT_Set_Char_Size(face, xsize, ysize, 0, 0);
	return face;
}

#define FONTCACHE 37

struct FtFaceEntry {
	Font    font;
	FT_Face face;
	String  path;
};

static FtFaceEntry ft_cache[FONTCACHE];

void ClearFtFaceCache()
{
	for(int i = 0; i < FONTCACHE; i++)
		ft_cache[i].font.Height(-30000);
}

FT_Face (*FTFaceXft)(Font fnt, String *rpath);

FT_Face FTFace(Font fnt, String *rpath = NULL)
{
	LTIMING("FTFace");
	if(FTFaceXft)
		return (*FTFaceXft)(fnt, rpath);
	ONCELOCK {
		ClearFtFaceCache();
	}
	FtFaceEntry be;
	be = ft_cache[0];
	for(int i = 0; i < FONTCACHE; i++) {
		FtFaceEntry e = ft_cache[i];
		if(i)
			ft_cache[i] = be;
		if(e.font == fnt) {
			if(rpath)
				*rpath = e.path;
			if(i)
				ft_cache[0] = e;
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
	FcPattern *p = CreateFcPattern(fnt);
	be.face = CreateFTFace(p, &be.path);
	FcPatternDestroy(p);
	ft_cache[0] = be;
	if(rpath)
		*rpath = be.path;
	return be.face;
}

CommonFontInfo (*GetFontInfoSysXft)(Font font);

CommonFontInfo GetFontInfoSys(Font font)
{
	sInitFt();
	if(GetFontInfoSysXft)
		return (*GetFontInfoSysXft)(font);
	CommonFontInfo fi;
	String path;
	FT_Face face = FTFace(font, &path);
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
		if(path.GetCount() < 250)
			strcpy(fi.path, ~path);
		else
			*fi.path = 0;
	}
	return fi;
}

#define FLOOR(x)    ((x) & -64)
#define CEIL(x)	    (((x)+63) & -64)
#define TRUNC(x)    ((x) >> 6)
#define ROUND(x)    (((x)+32) & -64)

GlyphInfo (*GetGlyphInfoSysXft)(Font font, int chr);

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
	LTIMING("GetGlyphInfoSys");
	GlyphInfo gi;
	FT_Face face = FTFace(font, NULL);
	gi.lspc = gi.rspc = 0;
	gi.width = 0x8000;
	if(face) {
		LTIMING("GetGlyphInfoSys 2");
		int glyph_index = FT_Get_Char_Index(face, chr);
		if(glyph_index) {
			if(GetGlyphInfoSysXft)
				return (*GetGlyphInfoSysXft)(font, chr);
			if(FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP) == 0 ||
			   FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT) == 0) {
				FT_Glyph_Metrics& m = face->glyph->metrics;
				int left  = FLOOR(m.horiBearingX);
				int width = TRUNC(CEIL(m.horiBearingX + m.width) - left);				
				gi.width = TRUNC(ROUND(face->glyph->advance.x));
				gi.lspc = TRUNC(left);
				gi.rspc = gi.width - width - gi.lspc;
			}
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
	FcPattern *p = FcPatternCreate();
	FcObjectSet *os = FcObjectSetBuild(FC_FAMILY, FC_SPACING, FC_SCALABLE, (void *)0);
	FcFontSet *fs = FcFontList(NULL, p, os);
	FcPatternDestroy(p);
	FcObjectSetDestroy(os);
	for(int i = 0; i < fs->nfont; i++) {
		FcChar8 *family = NULL;
		if(FcPatternGetString(fs->fonts[i], FC_FAMILY, 0, &family) == 0 && family) {
			FaceInfo& fi = list.Add();
			fi.name = (const char *)family;
			fi.info = 0;
			int iv;
			if(FcPatternGetInteger(fs->fonts[i], FC_SPACING, 0, &iv) == 0 && iv == FC_MONO)
				fi.info |= Font::FIXEDPITCH;
			FcBool bv;
			if(FcPatternGetBool(fs->fonts[i], FC_SCALABLE, 0, &bv) == 0 && bv)
				fi.info |= Font::SCALEABLE;
		}
	}
	FcFontSetDestroy(fs);
	return list;
}

#endif

END_UPP_NAMESPACE
