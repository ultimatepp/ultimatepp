#ifndef _Glyph_glyph_h_
#define _Glyph_glyph_h_

#include <Draw/Draw.h>
#include <Painter/Painter.h>

using namespace Upp;


Image  AutoCrop(const Image& m, RGBA c);
Image  RenderGlyph(Font fnt, int chr);
String CompressGlyph(const Image& m);
Image  DecompressGlyph(const String& g, Color c);

#endif
