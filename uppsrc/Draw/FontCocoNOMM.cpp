#include "Draw.h"

#define LLOG(x)     //  LOG(x)
#define LTIMING(x)  //  TIMING(x)

#if !defined(CUSTOM_FONTSYS) && defined(PLATFORM_COCOA) && defined(flagNOMM)

// NOMM serves mostly to build umk (or other console app) without Objective-C++

namespace Upp {

CommonFontInfo GetFontInfoSys(Font font)
{
	CommonFontInfo fi;
	return fi;
}

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
	GlyphInfo gi;
	gi.lspc = gi.rspc = 0;
	gi.width = 0x8000;
	gi.glyphi = 0;
	return gi;
}

Vector<FaceInfo> GetAllFacesSys()
{
	Vector<FaceInfo> r;
	return r;
}

String GetFontDataSys(Font font, const char *table, int offset, int size)
{ // read truetype or opentype table
	return Null;
}

void RenderCharacterSys(FontGlyphConsumer& sw, double x, double y, int chr, Font font)
{
}

};

#endif
