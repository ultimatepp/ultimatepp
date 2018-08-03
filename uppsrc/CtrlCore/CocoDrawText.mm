#include "CocoMM.h"

#ifdef PLATFORM_COCOA

#define LLOG(x)
#define LTIMING(x)

namespace Upp {

CTFontRef CT_Font0(Font fnt, bool& synth)
{
	CFRef<CFStringRef> s = CFStringCreateWithCString(NULL, ~fnt.GetFaceName(), kCFStringEncodingUTF8);
    CFRef<CTFontRef> ctfont0 = CTFontCreateWithName(s, fnt.GetHeight(), NULL);
	if(fnt.IsItalic() || fnt.IsBold()) {
	    CTFontSymbolicTraits symbolicTraits = 0;
	    if(fnt.IsBold())
		    symbolicTraits |= kCTFontBoldTrait;
	    if(fnt.IsItalic())
			symbolicTraits |= kCTFontItalicTrait;
		CGAffineTransform transform = CGAffineTransformIdentity;
		CFRef<CTFontRef> ctfont = CTFontCreateCopyWithSymbolicTraits(ctfont0, fnt.GetHeight(),
		                                          &transform, symbolicTraits, symbolicTraits);
		if(ctfont)
			return ctfont.Detach();
		synth = true;
	}
	return ctfont0.Detach();
}

CTFontRef CT_Font(Font fnt)
{
	struct Entry {
		Font      font;
		CTFontRef ctfont = NULL;
		bool      synth = false;
		
		void Free() { if(ctfont) CFRelease(ctfont); ctfont = NULL; }
		
		Entry() { font.Height(-22222); }
		~Entry() { Free(); }
	};

	const int FONTCACHE = 64;
	static Entry cache[FONTCACHE];
	for(int i = 0; i < FONTCACHE; i++)
		if(cache[i].font == fnt)
			return cache[i].ctfont;
	Entry& e = cache[Random(FONTCACHE)];
	e.Free();
	e.font = fnt;
	e.ctfont = CT_Font0(fnt, e.synth);
	return e.ctfont;
}

GlyphInfo GetGlyphInfoSys(CTFontRef ctfont, int chr)
{
	GlyphInfo gi;
	gi.lspc = gi.rspc = 0;
	gi.width = 0x8000;
	if(ctfont) {
		LTIMING("GetGlyphInfoSys 2");
	    CGGlyph glyph_index;
	    UniChar h = chr;
		CTFontGetGlyphsForCharacters(ctfont, &h, &glyph_index, 1);
		if(glyph_index) {
		    CGSize advance;
			CTFontGetAdvancesForGlyphs(ctfont, kCTFontOrientationHorizontal, &glyph_index, &advance, 1);
			gi.width = ceil(advance.width);
			gi.lspc = gi.rspc = 0; // TODO! (using bounding box?)
			gi.glyphi = glyph_index;
		}
	}
	return gi;
}

CommonFontInfo GetFontInfoSys(Font font)
{
	CommonFontInfo fi;
	String path;
	CTFontRef ctfont = CT_Font(font);
	if(ctfont) {
	#if 0
		DDUMP(font);
	    DDUMP(CTFontGetAscent(ctfont));
	    DDUMP(CTFontGetDescent(ctfont));
	    DDUMP(CTFontGetLeading(ctfont));
	    DDUMP(CTFontGetXHeight(ctfont));
	    DDUMP(CTFontGetUnderlinePosition(ctfont));
		DDUMPHEX(CTFontGetSymbolicTraits(ctfont));
	#endif
		fi.ascent = ceil(CTFontGetAscent(ctfont));
		fi.descent = ceil(CTFontGetDescent(ctfont));
		fi.height = fi.ascent + fi.descent;
		fi.lineheight = fi.height; // TODO!
		fi.external = 0;
		fi.internal = 0;
		fi.overhang = 0;
		fi.maxwidth = GetGlyphInfoSys(ctfont, 'M').width; // TODO?
		fi.avewidth = GetGlyphInfoSys(ctfont, 'e').width;
		fi.default_char = '?';
		fi.fixedpitch = CTFontGetSymbolicTraits(ctfont) & kCTFontMonoSpaceTrait;
		fi.ttf = true;
// TODO: PATH for data
//		if(path.GetCount() < 250)
//			strcpy(fi.path, ~path);
//		else
			*fi.path = 0;
	}
	return fi;
}

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
	LTIMING("GetGlyphInfoSys");
	CTFontRef ctfont = CT_Font(font);
	return GetGlyphInfoSys(ctfont, chr);
}

Vector<FaceInfo> GetAllFacesSys()
{
	Index<String> facename;
	Index<bool>   fixedpitch;

	facename.Add("Arial"); // TODO: This should be default GUI font
	facename.Add("Times New Roman");
	facename.Add("Arial");
	facename.Add("Courier New");

	AutoreleasePool __;

    CFRef<CTFontCollectionRef> collection = CTFontCollectionCreateFromAvailableFonts(0);
    if(collection) {
	    CFRef<CFArrayRef> fonts = CTFontCollectionCreateMatchingFontDescriptors(collection);
	    if(fonts) {
			int count = CFArrayGetCount(fonts);
		    for(int i = 0; i < count; ++i) {
				CTFontDescriptorRef font = (CTFontDescriptorRef)CFArrayGetValueAtIndex(fonts, i);
				CFRef<CFStringRef> family_name = (CFStringRef)CTFontDescriptorCopyAttribute(font, kCTFontFamilyNameAttribute);
				facename.FindAdd(ToString(family_name));
		    }
		}
    }
    
	Vector<FaceInfo> r;
	for(String s : facename) {
        FaceInfo& fi = r.Add();
		fi.name = s;
		fi.info = Font::TTF;
		CFRef<CFStringRef> fs = CFStringCreateWithCString(NULL, ~s, kCFStringEncodingUTF8);
	    CFRef<CTFontRef> ctfont = CTFontCreateWithName(fs, 12, NULL);
	    if(CTFontGetSymbolicTraits(ctfont) & kCTFontMonoSpaceTrait)
	        fi.info |= Font::FIXEDPITCH;
    }
    
    return r;
}

String GetFontDataSys(Font font)
{	// TODO! CFUrl?
	return Null;
}

void RenderCharacterSys(FontGlyphConsumer& sw, double x, double y, int ch, Font fnt)
{
	// TODO!
}

void SystemDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font, Color ink,
                            int n, const int *dx)
{
//	DLOG("angle " << angle << ", text " << text << ", ink " << ink);
	Set(ink); // needs to be here because rotation saves context...
	if(angle) {
		// TODO: Fix this!
		CGContextSaveGState(cgHandle);
		CGContextRotateCTM(cgHandle, 3 * M_PI * angle / 1800);
		CGContextTranslateCTM(cgHandle, x, -y);
		DrawTextOp(0, 0, 0, text, font, ink, n, dx);
	    CGContextRestoreGState(cgHandle);
		return;
	}

	CFRef<CGFontRef> cgFont = CTFontCopyGraphicsFont(CT_Font(font), NULL);
   
	CGContextSetFont(cgHandle, cgFont);
	
	Point off = GetOffset();
	x += off.x;
	y = top - y - font.GetAscent() - off.y;

	Buffer<CGGlyph> g(n);
	Buffer<CGPoint> p(n);
	for(int i = 0; i < n; i++) {
		int chr = text[i];
		GlyphInfo f = GetGlyphInfo(font, chr);
		p[i].y = y;
		p[i].x = x;
		g[i] = f.glyphi;
		x += dx ? *dx++ : f.width;
	}

	CGContextSetFontSize(cgHandle, font.GetHeight());
    CGContextShowGlyphsAtPositions(cgHandle, g, p, n);
}

};

#endif