#include "GLDraw.h"

#define LTIMING(x) // RTIMING(x)

#define LLOG(x) // DLOG(x)

namespace Upp {

struct ImageGLData {
	Image        img;
	GLuint       texture_id;
	
	void Init(const Image& img);
	~ImageGLData();
};

void ImageGLData::Init(const Image& img)
{
	LTIMING("CreateTexture");
	Size sz = img.GetSize();

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sz.cx, sz.cy, 0, GL_BGRA, GL_UNSIGNED_BYTE, ~img);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	LLOG("Texture id created: " << texture_id);
	SysImageRealized(img);
}

static LRUCache<ImageGLData, Tuple2<uint64, uint64> > sTextureCache;
static bool sReset;

ImageGLData::~ImageGLData()
{
	SysImageReleased(img);
	if(!sReset)
		glDeleteTextures(1, &texture_id);
	LLOG("Texture id deleted: " << texture_id);
}

void GLDraw::ClearCache()
{
	sReset = false;
	sTextureCache.Clear();
}

void GLDraw::ResetCache()
{
	sReset = true;
	sTextureCache.Clear();
	sReset = false;
}

struct ImageGLDataMaker : LRUCache<ImageGLData, Tuple2<uint64, uint64> >::Maker {
	Image         img;
	uint64        context;

	virtual Tuple2<uint64, uint64>  Key() const                      { return MakeTuple<uint64, uint64>(img.GetSerialId(), context); }
	virtual int                     Make(ImageGLData& object) const  { object.Init(img); return img.GetLength(); }
};

GLuint GetTextureForImage(const Image& img, uint64 context)
{
	sTextureCache.Shrink(4 * 1024 * 768, 1000);
	ImageGLDataMaker m;
	m.img = img;
	m.context = context;
	return sTextureCache.Get(m).texture_id;
}

};