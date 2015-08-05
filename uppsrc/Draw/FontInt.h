#ifndef _Draw_FontInt_h_
#define _Draw_FontInt_h_

// Implementation header

struct FaceInfo : Moveable<FaceInfo> {
	String name;
	dword  info;
};

struct CommonFontInfo {
	int  ascent;
	int  descent;
	int  external;
	int  internal;
	int  height;
	int  lineheight;
	int  overhang;
	int  avewidth;
	int  maxwidth;
	int  firstchar;
	int  charcount;
	int  default_char;
	int  spacebefore;
	int  spaceafter;
	bool fixedpitch;
	bool scaleable;
	bool ttf;

	char path[256]; // optional
};

class Font;

struct GlyphInfo {
	int16 width;
	int16 lspc;
	int16 rspc;
	word  glyphi; // optional, not available in Win32
	
	bool IsNormal() const     { return (word)width != 0x8000; }
	bool IsComposed() const   { return !IsNormal() && (lspc == -1 || lspc == -11); }
	bool IsComposedLM() const { return !IsNormal() && lspc == -11; }
	bool IsReplaced() const   { return !IsNormal() && lspc >= 0; }
	bool IsMissing() const    { return !IsNormal() && lspc == -2; }
};

void      Std(Font& font);
GlyphInfo GetGlyphInfo(Font font, int chr);
const     CommonFontInfo& GetFontInfo(Font font);
bool      IsNormal(Font font, int chr);
void      GlyphMetrics(GlyphInfo& f, Font font, int chr);

void      InvalidateFontList();

// Platform specific font interface

CommonFontInfo   GetFontInfoSys(Font font);
GlyphInfo        GetGlyphInfoSys(Font font, int chr);
Vector<FaceInfo> GetAllFacesSys();
String           GetFontDataSys(Font font);

void             RenderCharacterSys(FontGlyphConsumer& sw, double x, double y, int ch, Font fnt);

#endif
