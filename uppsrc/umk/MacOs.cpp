#include "umake.h"
#include <Draw/Draw.h>

#ifdef PLATFORM_COCOA

namespace Upp { // Silence linker errors

CommonFontInfo GetFontInfoSys(Font font)
{
	CommonFontInfo fi;
	return fi;
}

GlyphInfo  GetGlyphInfoSys(Font font, int chr)
{
	GlyphInfo gi;
	return gi;
}

Vector<FaceInfo> GetAllFacesSys()
{
	Vector<FaceInfo> r;
	return r;
}

String GetFontDataSys(Font font)
{
	return String();
}

void RenderCharacterSys(FontGlyphConsumer& sw, double x, double y, int chr, Font font)
{
}

};

#endif