#ifndef _WinGl_FontGl_h_
#define _WinGl_FontGl_h_

struct OpenGLFont : Moveable<OpenGLFont>
{
	float scaleW;
	float scaleH;
	float lineHeight;
	float base;
	float scale;
	
	bool texturesUpdated;

	float vtx[1024 * 4 * 2];
	float crd[1024 * 4 * 2];

	struct CharInfo : Moveable<CharInfo>
	{
		int id;
		float x;
		float y;
		float width;
		float height;
		float xoffset;
		float yoffset;
		float xadvance;
		int page;
	};

	Vector<CharInfo> chars;
	VectorMap<int, VectorMap<int, float> > kerns;
	Vector<String> files;
	Array<byte> compiledFiles;
	Vector<int64> pages;

	OpenGLFont() : texturesUpdated(false), scale(1.0f)
	{}

	~OpenGLFont()
	{}

	void LoadBrc(const byte* xml, const byte* image);
	void Load(const String& fileName);
	void Parse(const char* xml, bool parsePages);
	void UpdateTextures();
	void BuildVertices();
};

Size GetTextSize(const wchar *text, const OpenGLFont& fi, int n);

#endif
