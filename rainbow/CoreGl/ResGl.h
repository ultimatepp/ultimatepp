#ifndef _CoreGl_ResGl_h_
#define _CoreGl_ResGl_h_

struct TextureResource : Moveable<TextureResource>
{
	int64 serialId;
	int64 atlasSerialId;
	int realWidth;
	int realHeight;
	int width;
	int height;
	int x;
	int y;
	
	TextureResource(int w = 0, int h = 0)
	{
		serialId = -1;
		atlasSerialId = -1;
		realWidth = w;
		realHeight = h;
		width = w;
		height = h;
		x = 0;
		y = 0;
	}
	
	String ToString() const
	{
		return Format("x: %d, y: %d, w: %d, h:%d (%d, %d) - %ld", x, y, width, height, realWidth, realHeight, serialId);
	}
};

struct AtlasResource : Moveable<AtlasResource>
{
	int width;
	int height;
	bool linear;
	Vector<Image> parts;
	Image Make(ArrayMap<int64, TextureResource>& textures);
};

struct ImageResource : Moveable<ImageResource>
{
	int type;
	bool linear;
	
	//direct image
	Image img;
	//brc image
	const byte* data;
	int length;
	//disk image
	String fileName;
};

struct Resources
{
	enum {
		NEAREST_FILTERING = 1,
		LINEAR_FILTERING = 2,
		FORCE_FILTERING = 4,
		AUTO_ATLAS = 8,
		FORCE_BIND = 16
	};
	
	Vector<ImageResource> staticImages;
	VectorMap<String, AtlasResource> staticAtlases;
	Vector<Font> staticFonts;
	
	TextureResource autoAtlas;
	ImageBuffer autoAtlasBuffer;
	Image autoAtlasImage;
	int px, py, maxh;
	
	int bindedTextures;
	
	int64 currentSerialId;
	ArrayMap<int64, TextureResource> textures;
	ArrayMap<String, OpenGLFont> fonts;
	
	const TextureResource& CreateTexture(const Image& img, int width = -1, int height = -1);
	void CreateSubTexture(const TextureResource& t, const Image& img, int x, int y);
	
	const TextureResource& Bind(const Image& img, int opts = NEAREST_FILTERING);
	bool Bind(int64 serialId, int opts = NEAREST_FILTERING);
	void Add(const Image& img, bool linear = false);
	void Add(Iml* images, bool linear = false);
	void Add(const Font& fnt);
	void AddAtlas(const char* atlasName, const Image& img);
	void AddAtlas(const char* atlasName, Iml* images);
	OpenGLFont& GetFont(const char* fontName, int fontHeight, bool preload = false, const byte* fontDef = NULL, const byte** imagesData = NULL, const int* imagesSize = NULL, int imagesCount = 0);
	OpenGLFont& GetFont(const Font& font, bool preload = false);
	void SetTextureFiltring(int opts);
	
	void BindStatic();
	
	Resources();
};

extern Resources resources;


#endif
