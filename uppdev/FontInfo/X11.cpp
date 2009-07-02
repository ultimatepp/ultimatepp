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

	//!!!
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
/*
	fi->load_flags = FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP|FT_LOAD_NO_HINTING; //!!!Painter!!!

	fi->spacing = FC_PROPORTIONAL;

	if(FcPatternGetBool(pattern, FC_MINSPACE, 0, &fi->minspace) != FcResultMatch) {
		fi->minspace = FcFalse;
	}

	if(FcPatternGetInteger(pattern, FC_CHAR_WIDTH, 0, &fi->char_width) == FcResultMatch)
		fi->spacing = FC_MONO;
	else
		fi->char_width = 0;
*/
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
	gi.lspc = gi.rspc = gi.width = 0;
	if(face) {
		int glyph_index = FT_Get_Char_Index(face, chr);
		if(glyph_index &&
			(FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP) == 0 ||
			 FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT) == 0)) {
				FT_Glyph_Metrics& m = face->glyph->metrics;
			
				int left  = FLOOR( m.horiBearingX );
				int right = CEIL( m.horiBearingX + m.width );
				int top    = CEIL( m.horiBearingY );
				int bottom = FLOOR( m.horiBearingY - m.height );
				int width = TRUNC(right - left);				
				int height = TRUNC( top - bottom );

//				xftg->metrics.width = width;
//				xftg->metrics.height = height;
//				xftg->metrics.x = -TRUNC(left);
//				xftg->metrics.y = TRUNC(top);			
				
//			    xftg->metrics.xOff = TRUNC(ROUND(glyphslot->advance.x));
//			    xftg->metrics.yOff = -TRUNC(ROUND(glyphslot->advance.y));

				gi.width = TRUNC(ROUND(face->glyph->advance.x));
				gi.lspc = TRUNC(left);
				gi.rspc = gi.width - width - gi.lspc;
		}
	}
	return gi;
}

#if 0

void
XftFontLoadGlyphs (Display	    *dpy,
		   XftFont	    *pub,
		   FcBool	    need_bitmaps,
		   _Xconst FT_UInt  *glyphs,
		   int		    nglyph)
{
    XftDisplayInfo  *info = _XftDisplayInfoGet (dpy, True);
    XftFontInt	    *font = (XftFontInt *) pub;
    FT_Error	    error;
    FT_UInt	    glyphindex;
    FT_GlyphSlot    glyphslot;
    XftGlyph	    *xftg;
    Glyph	    glyph;
    unsigned char   bufLocal[4096];
    unsigned char   *bufBitmap = bufLocal;
    int		    bufSize = sizeof (bufLocal);
    int		    size, pitch;
    unsigned char   bufLocalRgba[4096];
    unsigned char   *bufBitmapRgba = bufLocalRgba;
    int		    bufSizeRgba = sizeof (bufLocalRgba);
    int		    sizergba, pitchrgba, widthrgba;
    int		    width;
    int		    height;
    int		    left, right, top, bottom;
    int		    hmul = 1;
    int		    vmul = 1;
    FT_Bitmap	    ftbit;
    FT_Matrix	    matrix;
    FT_Vector	    vector;
    Bool	    subpixel = False;
    FT_Face	    face;

    if (!info)
	return;

    face = XftLockFace (&font->public);
    
    if (!face)
	return;

    matrix.xx = matrix.yy = 0x10000L;
    matrix.xy = matrix.yx = 0;

    while (nglyph--)
    {
	glyphindex = *glyphs++;
	xftg = font->glyphs[glyphindex];
	if (!xftg)
	    continue;
	
	if (XftDebug() & XFT_DBG_CACHE)
	    _XftFontValidateMemory (dpy, pub);
	/*
	 * Check to see if this glyph has just been loaded,
	 * this happens when drawing the same glyph twice
	 * in a single string
	 */
	if (xftg->glyph_memory)
	    continue;
	
	error = FT_Load_Glyph (face, glyphindex, font->info.load_flags);
	if (error)
	{
	    /*
	     * If anti-aliasing or transforming glyphs and
	     * no outline version exists, fallback to the
	     * bitmap and let things look bad instead of
	     * missing the glyph
	     */
	    if (font->info.load_flags & FT_LOAD_NO_BITMAP)
		error = FT_Load_Glyph (face, glyphindex,
				       font->info.load_flags & ~FT_LOAD_NO_BITMAP);
	    if (error)
		continue;
	}

#define FLOOR(x)    ((x) & -64)
#define CEIL(x)	    (((x)+63) & -64)
#define TRUNC(x)    ((x) >> 6)
#define ROUND(x)    (((x)+32) & -64)
		
	glyphslot = face->glyph;

	/*
	 * Compute glyph metrics from FreeType information
	 */
	if(font->info.transform && glyphslot->format != ft_glyph_format_bitmap) 
	{
	    /*
	     * calculate the true width by transforming all four corners.
	     */
	    int xc, yc;
	    left = right = top = bottom = 0;
	    for(xc = 0; xc <= 1; xc ++) {
		for(yc = 0; yc <= 1; yc++) {
		    vector.x = glyphslot->metrics.horiBearingX + xc * glyphslot->metrics.width;
		    vector.y = glyphslot->metrics.horiBearingY - yc * glyphslot->metrics.height;
		    FT_Vector_Transform(&vector, &font->info.matrix);   
		    if (XftDebug() & XFT_DBG_GLYPH)
			printf("Trans %d %d: %d %d\n", (int) xc, (int) yc, 
			       (int) vector.x, (int) vector.y);
		    if(xc == 0 && yc == 0) {
			left = right = vector.x;
			top = bottom = vector.y;
		    } else {
			if(left > vector.x) left = vector.x;
			if(right < vector.x) right = vector.x;
			if(bottom > vector.y) bottom = vector.y;
			if(top < vector.y) top = vector.y;
		    }

		}
	    }
	    left = FLOOR(left);
	    right = CEIL(right);
	    bottom = FLOOR(bottom);
	    top = CEIL(top);

	} else {
	    left  = FLOOR( glyphslot->metrics.horiBearingX );
	    right = CEIL( glyphslot->metrics.horiBearingX + glyphslot->metrics.width );

	    top    = CEIL( glyphslot->metrics.horiBearingY );
	    bottom = FLOOR( glyphslot->metrics.horiBearingY - glyphslot->metrics.height );
	}

	width = TRUNC(right - left);
	height = TRUNC( top - bottom );

	/*
	 * Try to keep monospace fonts ink-inside
	 * XXX transformed?
	 */
	if (font->info.spacing != FC_PROPORTIONAL && !font->info.transform)
	{
	    if (font->info.load_flags & FT_LOAD_VERTICAL_LAYOUT)
	    {
		if (TRUNC(bottom) > font->public.max_advance_width)
		{
		    int adjust;
    
		    adjust = bottom - (font->public.max_advance_width << 6);
		    if (adjust > top)
			adjust = top;
		    top -= adjust;
		    bottom -= adjust;
		    height = font->public.max_advance_width;
		}
	    }
	    else
	    {
		if (TRUNC(right) > font->public.max_advance_width)
		{
		    int adjust;
    
		    adjust = right - (font->public.max_advance_width << 6);
		    if (adjust > left)
			adjust = left;
		    left -= adjust;
		    right -= adjust;
		    width = font->public.max_advance_width;
		}
	    }
	}

	if (font->info.antialias)
	    pitch = (width * hmul + 3) & ~3;
	else
	    pitch = ((width + 31) & ~31) >> 3;

	size = pitch * height * vmul;

	xftg->metrics.width = width;
	xftg->metrics.height = height;
	xftg->metrics.x = -TRUNC(left);
	xftg->metrics.y = TRUNC(top);

	if (font->info.spacing != FC_PROPORTIONAL)
	{
	    {
		if (font->info.load_flags & FT_LOAD_VERTICAL_LAYOUT)
		{
		    xftg->metrics.xOff = 0;
		    xftg->metrics.yOff = -font->public.max_advance_width;
		}
		else
		{
		    xftg->metrics.xOff = font->public.max_advance_width;
		    xftg->metrics.yOff = 0;
		}
	    }
	}
	else
	{
	    xftg->metrics.xOff = TRUNC(ROUND(glyphslot->advance.x));
	    xftg->metrics.yOff = -TRUNC(ROUND(glyphslot->advance.y));
	}
	
	/*
	 * Use the glyph index as the wire encoding; it
	 * might be more efficient for some locales to map
	 * these by first usage to smaller values, but that
	 * would require persistently storing the map when
	 * glyphs were freed.
	 */
	glyph = (Glyph) glyphindex;

}

XftFont *
XftFontOpenPattern (Display *dpy, FcPattern *pattern)
{
    XftFontInfo	    info;
    XftFont	    *font;

    if (!XftFontInfoFill (dpy, pattern, &info))
	return 0;

    font = XftFontOpenInfo (dpy, pattern, &info);
    XftFontInfoEmpty (dpy, &info);
    return font;
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

	return li[chr & 255];
}
#endif

#endif
