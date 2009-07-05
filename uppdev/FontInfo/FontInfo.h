#ifndef _FontInfo_FontInfo_h_
#define _FontInfo_FontInfo_h_

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

CommonFontInfo GetFontInfoSys(Font font);

GlyphInfo      GetGlyphInfoSys(Font font, int chr);

Vector<FaceInfo> GetAllFacesSys();

void Std(Font& font);

bool      IsNormal(Font font, int chr);

GlyphInfo GetGlyphInfo(Font font, int chr);
void      GlyphMetrics(GlyphInfo& f, Font font, int chr);
GlyphInfo GetGlyphMetrics(Font font, int chr);

const CommonFontInfo& GetFontInfo(Font font);

bool Compose(Font fnt, int chr, ComposedGlyph& cs);
bool Replace(Font fnt, int chr, Font& rfnt);

void DoDrawText(Draw& w, int x, int y, int angle, const wchar *text, Font font, Color ink,
                int n, const int *dx);

#endif
