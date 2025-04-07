#include "Draw.h"

#define LLOG(x)     //  LOG(x)
#define LTIMING(x)  //  TIMING(x)

#if !defined(CUSTOM_FONTSYS) && defined(PLATFORM_COCOA)

#ifndef flagNOMM // Removes ObjectiveC and AppKit dependence in Draw (but disables Fonts)

#define Point NS_Point
#define Rect  NS_Rect
#define Size  NS_Size
#include <AppKit/AppKit.h>
#undef  Point
#undef  Rect
#undef  Size

namespace Upp {

namespace Detail { // TODO: following utilities are normally defined in CtrlCore, rename to avoid name clash

template <class T>
struct CFRef {
	T ptr;
	T operator~()   { return ptr; }
	operator T()    { return ptr; }
	T  operator->() { return ptr; }
	T  Detach()     { T h = ptr; ptr = NULL; return h; }
	CFRef(T p)      { ptr = p; }
	~CFRef()        { if(ptr) CFRelease(ptr); }
};

struct AutoreleasePool {
	NSAutoreleasePool *pool;

	AutoreleasePool() {
		pool = [[NSAutoreleasePool alloc] init];
	}
	~AutoreleasePool() {
	    [pool release];
	}
};

};

using namespace Detail;

WString ToWString(CFStringRef s)
{
	if(!s) return Null;
	CFIndex l = CFStringGetLength(s);
	if(!l) return Null;
	Buffer<char16> h(l);
    CFStringGetCharacters(s, CFRangeMake(0, l), (UniChar *)~h);
    return ToUtf32(~h, l);
}

String ToString(CFStringRef s)
{
	return ToWString(s).ToString();
}

CTFontRef CT_Font0(Font fnt, bool& synth)
{
	CFRef<CFStringRef> s = CFStringCreateWithCString(NULL, ~fnt.GetFaceName(), kCFStringEncodingUTF8);
    CFRef<CTFontRef> ctfont0 = CTFontCreateWithName(s, fnt.GetHeight(), NULL);
    synth = false;
	if(fnt.IsItalic() || fnt.IsBold()) {
	    CTFontSymbolicTraits t = 0;
	    if(fnt.IsBold())
		    t |= kCTFontBoldTrait;
	    if(fnt.IsItalic())
			t |= kCTFontItalicTrait;
		CFRef<CTFontRef> ctfont = CTFontCreateCopyWithSymbolicTraits(ctfont0, fnt.GetHeight(),
		                                                             NULL, t, t);
		if(ctfont)
			return ctfont.Detach();
		synth = true;
	}
	return ctfont0.Detach();
}

CTFontRef CT_Font(Font fnt, bool& synth)
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
		if(cache[i].font == fnt) {
			synth = cache[i].synth;
			return cache[i].ctfont;
		}
	Entry& e = cache[Random(FONTCACHE)];
	e.Free();
	e.font = fnt;
	e.ctfont = CT_Font0(fnt, synth);
	e.synth = synth;
	return e.ctfont;
}

CGGlyph GetCharGlyph(CTFontRef ctfont, int chr)
{
    CGGlyph glyph_index[2];
    Vector<char16> h = ToUtf16(chr);
	CTFontGetGlyphsForCharacters(ctfont, (UniChar *)h.begin(), glyph_index, h.GetCount());
	return glyph_index[0];
}

GlyphInfo GetGlyphInfoSys(CTFontRef ctfont, int chr, bool bold_synth, CGRect *bounds = NULL)
{
	GlyphInfo gi;
	gi.lspc = gi.rspc = 0;
	gi.width = 0x8000;
	if(ctfont) {
		LTIMING("GetGlyphInfoSys 2");
	    CGGlyph glyph_index = GetCharGlyph(ctfont, chr);
		if(glyph_index) {
		    CGSize advance;
			CTFontGetAdvancesForGlyphs(ctfont, kCTFontOrientationHorizontal, &glyph_index, &advance, 1);
			gi.width = ceil(advance.width);
			gi.lspc = gi.rspc = 0; // TODO! (using bounding box?)
			gi.glyphi = glyph_index;
			if(bold_synth)
				gi.width++;
			if(bounds)
				CTFontGetBoundingRectsForGlyphs(ctfont, kCTFontOrientationHorizontal,
				                                &glyph_index, bounds, 1);

		}
	}
	return gi;
}

CommonFontInfo GetFontInfoSys(Font font)
{
	CommonFontInfo fi;
	String path;
	bool synth;
	CTFontRef ctfont = CT_Font(font, synth);
	if(ctfont) {
	#if 0
		DDUMP(font);
	    DDUMP(CTFontGetSize(ctfont));
	    DDUMP(CTFontGetAscent(ctfont));
	    DDUMP(CTFontGetDescent(ctfont));
	    DDUMP();
	    DDUMP(CTFontGetXHeight(ctfont));
	    DDUMP(CTFontGetUnderlinePosition(ctfont));
	    DDUMP(MakeRect(CTFontGetBoundingBox(ctfont)));
		DDUMPHEX(CTFontGetSymbolicTraits(ctfont));
		DDUMP(font);
		CGRect cr = CTFontGetBoundingBox(ctfont);
		DDUMP(cr.origin.y);
		DDUMP(cr.size.height);
		DDUMP(cr.origin.y + cr.size.height);
	#endif
		fi.descent = ceil(CTFontGetDescent(ctfont));
		fi.ascent = ceil(CTFontGetAscent(ctfont));
		fi.external = ceil(CTFontGetLeading(ctfont));
		
		// Some MacOS fonts have really weird ascent/descents (namely stadard GUI font...)
		// let us fix it by testing typical charactes bounding boxes
		
		static WString descent_test = "yjgp";
		CGRect bb;
		for(int i = 0; i < descent_test.GetCount(); i++)
			if(GetGlyphInfoSys(ctfont, descent_test[i], synth && font.IsBold(), &bb).IsNormal())
				fi.descent = max(fi.descent, (int)ceil(-bb.origin.y));
		
		int ascent = fi.ascent;
		static WString ascent_test = "ÀÁÂÃÄË";
		for(int i = 0; i < ascent_test.GetCount(); i++)
			if(GetGlyphInfoSys(ctfont, ascent_test[i], synth && font.IsBold(), &bb).IsNormal())
				ascent = max(ascent, (int)ceil(bb.origin.y + bb.size.height));
		
		fi.ascent = ascent;

		fi.internal = 0;
		fi.overhang = 0;
		fi.maxwidth = GetGlyphInfoSys(ctfont, 'W', synth && font.IsBold()).width; // TODO?
		fi.avewidth = GetGlyphInfoSys(ctfont, 'e', synth && font.IsBold()).width;
		fi.default_char = '?';
		fi.fixedpitch = CTFontGetSymbolicTraits(ctfont) & kCTFontMonoSpaceTrait;
		fi.ttf = true;

		CFRef<CTFontDescriptorRef> fd = CTFontCopyFontDescriptor(ctfont);
	    CFRef<CFURLRef> url = (CFURLRef)CTFontDescriptorCopyAttribute(fd, kCTFontURLAttribute);
		CFRef<CFStringRef> path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
		String p = ToString(path);

		if(p.GetCount() < 250)
			strcpy(fi.path, ~p);
		else
			*fi.path = 0;
	}
	return fi;
}

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
	LTIMING("GetGlyphInfoSys");
	bool synth;
	CTFontRef ctfont = CT_Font(font, synth);
	return GetGlyphInfoSys(ctfont, chr, synth && font.IsBold());
}

Vector<FaceInfo> GetAllFacesSys()
{
	Index<String> facename;

	facename.Add("Arial"); // This is default GUI font, changed afterward
	facename.Add("Times New Roman");
	facename.Add("Arial");
	facename.Add("Courier New");

	int oi = facename.GetCount();

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

	Vector<String> h = facename.PickKeys();
	Sort(SubRange(h, oi, h.GetCount() - oi));

	Vector<FaceInfo> r;
	for(String s : h) {
		FaceInfo& fi = r.Add();
		fi.name = s;
		fi.info = Font::TTF;
		CFRef<CFStringRef> fs = CFStringCreateWithCString(NULL, ~s, kCFStringEncodingUTF8);
		CFRef<CTFontRef> ctfont = CTFontCreateWithName(fs, 12, NULL);
		dword traits = CTFontGetSymbolicTraits(ctfont);
		fi.info = Font::SCALEABLE;
		if(traits & kCTFontMonoSpaceTrait)
			fi.info |= Font::FIXEDPITCH;
		switch(traits & kCTFontClassMaskTrait) {
		case kCTFontOldStyleSerifsClass:
		case kCTFontTransitionalSerifsClass:
		case kCTFontModernSerifsClass:
		case kCTFontClarendonSerifsClass:
		case kCTFontSlabSerifsClass:
		case kCTFontFreeformSerifsClass:
			fi.info |= Font::SERIFSTYLE;
		    break;
		case kCTFontScriptsClass:
			fi.info |= Font::SCRIPTSTYLE;
		    break;
		}
	}

	return r;
}

String GetFontDataSys(Font font, const char *table, int offset, int size)
{ // read truetype or opentype table
	FileIn in(font.Fi().path);
	int q = in.Get32be();
	if(q == 0x74746366) { // font collection
		in.Get32(); // skip major/minor version
		int nfonts = in.Get32be();
		if(font.Fi().fonti >= nfonts)
			return Null;
		in.SeekCur(font.Fi().fonti * 4);
		int offset = in.Get32be();
		if(offset < 0 || offset >= in.GetSize())
			return Null;
		in.Seek(offset);
		q = in.Get32be();
	}
	if(q != 0x74727565 && q != 0x00010000 && q != 0x4f54544f) // 0x4f54544f means CCF font!
		return Null;
	int n = in.Get16be();
	in.Get32();
	in.Get16();
	while(n--) {
		if(in.IsError() || in.IsEof()) return Null;
		String tab = in.Get(4);
		in.Get32();
		int off = in.Get32be();
		int len = in.Get32be();
		if(tab == table) {
			if(off < 0 || len < 0 || off + len > in.GetSize())
				return Null;
			len = min(len - offset, size);
			if(len < 0)
				return Null;
			in.Seek(off + offset);
			return in.Get(len);
		}
	}
	return Null;
}

struct sCGPathTarget {
	double x, y;
	FontGlyphConsumer *sw;
};

static void convertCGPathToQPainterPath(void *info, const CGPathElement *e)
{
	auto t = (sCGPathTarget *)info;
	switch(e->type) {
	case kCGPathElementMoveToPoint:
		t->sw->Move(Pointf(e->points[0].x + t->x, e->points[0].y + t->y));
		break;
	case kCGPathElementAddLineToPoint:
		t->sw->Line(Pointf(e->points[0].x + t->x, e->points[0].y + t->y));
		break;
	case kCGPathElementAddQuadCurveToPoint:
		t->sw->Quadratic(Pointf(e->points[0].x + t->x, e->points[0].y + t->y),
	                     Pointf(e->points[1].x + t->x, e->points[1].y + t->y));
		break;
	case kCGPathElementAddCurveToPoint:
		t->sw->Cubic(Pointf(e->points[0].x + t->x, e->points[0].y + t->y),
	                 Pointf(e->points[1].x + t->x, e->points[1].y + t->y),
	                 Pointf(e->points[2].x + t->x, e->points[2].y + t->y));
		break;
	case kCGPathElementCloseSubpath:
		t->sw->Close();
		break;
	}
}

void RenderCharacterSys(FontGlyphConsumer& sw, double x, double y, int chr, Font font)
{
	CGAffineTransform cgMatrix = CGAffineTransformIdentity;
	cgMatrix = CGAffineTransformScale(cgMatrix, 1, -1);
	bool synth;
	CTFontRef ctfont = CT_Font(font, synth);
    CGGlyph glyph_index = GetCharGlyph(ctfont, chr);
    CFRef<CGPathRef> cgpath = CTFontCreatePathForGlyph(ctfont, glyph_index, &cgMatrix);
    sCGPathTarget t;
    t.x = x;
    t.y = y + font.GetAscent();
    t.sw = &sw;
    CGPathApply(cgpath, &t, convertCGPathToQPainterPath);
}

};

#endif

#endif