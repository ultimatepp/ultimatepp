#ifndef _WinGl_ResGl_h_
#define _WinGl_ResGl_h_

struct Texture : Moveable<Texture>
{
	Size sz;
	Point curpos;	
	VectorMap<int64, Rect> parts;
	void AddPart(int64 serialId, const Image& img);
};

struct Resources
{
	static int64 currentSerialId;
	static ArrayMap<int64, Texture> textures;
	static VectorMap<String, OpenGLFont> fonts;
	static int64 Bind(const Image& img, bool linear = false);
	static bool Bind(int64 serialId, bool force = false);
	static OpenGLFont& GetFont(const char* fontName, int fontHeight);
	static OpenGLFont& GetFontBrc(const char* fontName, const byte* fontDef, const byte* fontImage, int fontHeight);
	static OpenGLFont& GetFont(const Font& font);
};

#endif
